#include "ExampleGO.h"
#include "AIModel.h"
#include "stringHelp.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"

ExampleGO::ExampleGO()
{

}

ExampleGO::~ExampleGO()
{

}

void ExampleGO::Load(ifstream& _file)
{
	GameObject::Load(_file);
	StringHelp::String(_file, "MODEL", m_ModelName);
	StringHelp::String(_file, "TEXTURE", m_TexName);
	StringHelp::String(_file, "SHADER", m_ShaderName);

}
//ET: CAN USE THIS TO MAKE BEHAVIOURS FOR CUBES
void ExampleGO::Tick(float _dt)
{
	GameObject::Tick(_dt);

	

	


}

void ExampleGO::PreRender()
{
	GameObject::PreRender();

	//only thing I need to do is tell the shader about my texture

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//TODO: this does sort of replicate stuff in the AIMesh class, could we make them more compatible.

	//TODO: NORMAL MAPS!
}

void ExampleGO::Render()
{
	//ET:orginially here: m_model->Render();

	if (m_num_pos <= 1) //ET: if only one position
	{
		m_worldMatrix = glm::translate(mat4(1.0), m_pos);
		m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

		m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale));

		PreRender();
		m_model->Render();
	}
	else //ET: if multiple positions
	{
		for (vec3 pos : m_posList)
		{
			m_worldMatrix = glm::translate(mat4(1.0), pos);

			m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale));

			PreRender();
			m_model->Render();
		}
	}

	


}

void ExampleGO::Init(Scene* _scene)
{
	m_ShaderProg = _scene->GetShader(m_ShaderName)->GetProg();
	m_texture = _scene->GetTexture(m_TexName)->GetTexID();
	m_model = _scene->GetModel(m_ModelName);
}
