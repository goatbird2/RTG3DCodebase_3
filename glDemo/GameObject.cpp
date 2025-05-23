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
	StringHelp::Float(_file, "NUMPOS", m_num_pos);

	//ET: So that list is cleared and not filled with old data, aka, not drawing new instances
	m_posList.clear();
	
	if (m_num_pos <= 1) {
		StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	}
	else if (m_num_pos > 1) {
		for (int i = 0; i < m_num_pos; i++)
		{
			StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
			m_posList.push_back(m_pos);
		}
	}
	
	
	/*StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);*/

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

//ET: MoveRelative Funtion
//ET: This function allows us to move the object in a local direction
//ET: aka, I want keys to move the object in the direction it is facing
void GameObject::MoveRelative(const glm::vec3& localDir, float amount)
{
	// Only use yaw for horizontal movement
	float yaw = glm::radians(m_rot.y);
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0, 1, 0));
	glm::vec3 worldDir = glm::vec3(rot * glm::vec4(localDir, 0.0f));
	m_pos += worldDir * amount;
}


void GameObject::PreRender()
{
	// Setup model transform
	GLint pLocation;
	Helper::SetUniformLocation(m_ShaderProg, "modelMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&m_worldMatrix);

	///ETR: tried to make instanciated GO rotate, didnt work tho
	/*if (m_ShaderProg == 0) return;
	glUseProgram(m_ShaderProg);

	GLint pLocation;
	Helper::SetUniformLocation(m_ShaderProg, "modelMatrix", &pLocation);
	if (pLocation < 0) return;

	if (!m_posList.empty()) {
		for (const auto& pos : m_posList) {
			glm::mat4 worldMatrix = glm::translate(glm::mat4(1.0f), pos);
			worldMatrix = glm::rotate(worldMatrix, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
			worldMatrix = glm::rotate(worldMatrix, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
			worldMatrix = glm::rotate(worldMatrix, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
			worldMatrix = glm::scale(worldMatrix, m_scale);

			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&worldMatrix);

			Render();
		}
	}
	else {
		glm::mat4 worldMatrix = glm::translate(glm::mat4(1.0f), m_pos);
		worldMatrix = glm::rotate(worldMatrix, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		worldMatrix = glm::rotate(worldMatrix, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		worldMatrix = glm::rotate(worldMatrix, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
		worldMatrix = glm::scale(worldMatrix, m_scale);

		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&worldMatrix);

		Render();
	}
}*/
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
void GameObject::RotateZ(float _dir)
{
	m_rot.z += _dir;
	
}
void GameObject::MoveZ(float _dir)
	{
	m_pos.z += _dir;

}
void GameObject::RotateY(float _dir)
{
	m_rot.y += _dir;

}