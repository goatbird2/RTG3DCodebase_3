#pragma once
#include "core.h"
#include <vector>
#include <stdio.h>
#include <string>
#include "RenderPass.h"
#include <glm/glm.hpp>

using namespace std;
class Scene;

using namespace glm;

//Base class of a GameObject
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	//load me from the file
	virtual void Load(ifstream& _file);

	//update the GameObject
	//TODO: possibly pass keyboard / mouse stuff down here for player controls?
	virtual void Tick(float _dt);

	virtual void PreRender();//set up any shader values needed for this object
	virtual void Render();//render this object

	//various getters and setters
	void SetName(string _name) { m_name = _name; }
	string GetName() { return m_name; }
	GLuint GetShaderProg() { return m_ShaderProg; }

	//ET: getter for position (needed for transparency)
	glm::vec3 GetPos() const { return m_pos; }


	//scene maybe needed for more involved cameras to connect to relvant GOs and lights/shaders etc
	virtual void Init(Scene* _scene);

	//this GameObject should be drawn in THIS render pass
	RenderPass GetRP() { return m_RP; }
	void MoveX(float _dir);
	void MoveY(float _dir);
	void RotateZ(float _dir);
	void RotateY(float _dir);
	void MoveZ(float _dir);


	//ET: For Heartbeat Pulse of objects
	bool m_isPulsing = false;
	string m_Pulsing ;
	float m_pulseSpeed = 3.0f; //ET: Speed
	float m_pulseAmplitude = 0.3f; //ET: "Strenght"
	float m_pulseBaseScale = 0.2f; //ET: Base scale
	float m_pulseTime = 0.0f; //ET: goes up over time

	//ET: For getting the rotation of the tracked object to then change the camera
	float GetYaw() const { return m_rot.y; }

	//ET: For moving the object in a local direction
	//ET: aka, I want keys to move the object in the direction it is facing
	void MoveRelative(const glm::vec3& localDir, float amount);



protected:

	string m_name;
	string m_type;
	string m_renderName;

	vec3		m_scale_incr = vec3(0.001f, 0.001f, 0.001f);
	float		m_num_pos;
	vec3		m_pos;
	vec3		m_rot;
	vec3		m_scale;
	vec3		m_rot_incr;

	std::vector<vec3> m_posList;


	glm::mat4	m_worldMatrix;

	GLuint m_ShaderProg;

	RenderPass m_RP = RP_OPAQUE;
};

