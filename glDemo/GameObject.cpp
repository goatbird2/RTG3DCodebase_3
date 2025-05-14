#include "core.h"
#include "GameObject.h"
#include "stringHelp.h"
#include "helper.h"

using namespace glm;

GameObject::GameObject()
{
	m_type = "GAMEOBJECT";
}

GameObject::~GameObject()
{
}

void GameObject::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "ROT", m_rot.x, m_rot.y, m_rot.z);
	StringHelp::Float3(_file, "SCALE", m_scale.x, m_scale.y, m_scale.z);
	StringHelp::Float3(_file, "ROT INC", m_rot_incr.x, m_rot_incr.y, m_rot_incr.z);
	StringHelp::String(_file, "PULSE", m_Pulsing);
	if (m_Pulsing == "TRUE") {
		m_isPulsing = true;
	}
	else {
		m_isPulsing = false;
	}

	//ET: add for transparency
	StringHelp::String(_file, "RENPASS", m_renderName); //ET: will "look in the manifest for that exact word"
	if (m_renderName == "RP_OPAQUE") {
		m_RP = RP_OPAQUE;
	}
	else if (m_renderName == "RP_TRANSPARENT") {
		m_RP = RP_TRANSPARENT;
	}
}

void GameObject::Tick(float _dt)
{
	m_rot += m_rot_incr;

	//m_scale += m_scale_incr;



	//ET: Heartbeat pulse

	// Update pulse time
	if (m_isPulsing)
	{
		m_pulseTime += _dt * m_pulseSpeed;
		float pulse = sin(m_pulseTime) * m_pulseAmplitude + m_pulseBaseScale;
		m_scale = vec3(pulse, pulse, pulse);
	}

	// ET: Calculation word matrix

	m_worldMatrix = glm::translate(mat4(1.0), vec3(m_pos));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

	m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale));


	//ET: HEARTBEAT PULSE
	if (m_isPulsing)
	{
		m_pulseTime += _dt;
		//float scale = m_pulseBaseScale + sin(m_pulseTime * m_pulseSpeed) * m_pulseAmplitude;
		//SetScale(glm::vec3(scale)); //ET: evenly in all directions
	}
}

void GameObject::PreRender()
{
	// Setup model transform
	GLint pLocation;
	Helper::SetUniformLocation(m_ShaderProg, "modelMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&m_worldMatrix);
}

void GameObject::Render()
{
	//I have nothing to draw
}

void GameObject::Init(Scene* _scene)
{
	//I have nothing to link up to
}
void GameObject::MoveX(float _dir)
{
	m_pos.x += _dir;
}
void GameObject::MoveY(float _dir)
{
	m_pos.y += _dir;
}
void GameObject::Rotate(float _dir)
{
	m_rot.z += _dir;
	
}