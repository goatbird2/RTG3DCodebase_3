
// included header files
#include "core.h" // ET: holds engine-wide definitions and settings
#include "GameObject.h" // ET: Declaration of this class
#include "stringHelp.h" // ET: Used to read data from text files aka. the recipe
#include "helper.h" // ET: Contains utility functions (like finding variable in shader)

using namespace glm;

// Constructor
GameObject::GameObject()
{
	// ET: When a GameObject is created, its type is set to "GAMEOBJECT"
	m_type = "GAMEOBJECT";
}

// Deconstructor
GameObject::~GameObject()
{
	// ET: currently empty
}

// ET: "unpacks/loads" 3D objects from save or level files
void GameObject::Load(ifstream& _file)
{
	// ET: uses "StringHelp::" functions to read different things:
	// name of object
	StringHelp::String(_file, "NAME", m_name);

	// object position
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);

	// objects rotation angle
	StringHelp::Float3(_file, "ROT", m_rot.x, m_rot.y, m_rot.z);

	// scale/size of object
	StringHelp::Float3(_file, "SCALE", m_scale.x, m_scale.y, m_scale.z);

	// rot inc -> spinning behaviour
	// ET: aka, how often objects rotates per frame
	StringHelp::Float3(_file, "ROT INC", m_rot_incr.x, m_rot_incr.y, m_rot_incr.z);
}


// ET: Does the actual transform updates:
// ET: Builds the model matrix for position, rotation and scale
// ET: Called every frame and updates object's state
void GameObject::Tick(float _dt)
{
	// ET: applies m_rot_incr (rotation increment) to m_rot
	m_rot += m_rot_incr;

	// ET: Translation (moting to position)
	// ET: (vec 3 instead of vec4? but thats what was in cube.cpp?)
	m_worldMatrix = glm::translate(mat4(1.0), vec3(m_pos));

	// ET: rotation in degrees:
	
	// rotate around x
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));

	// rotate around y
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));

	// rotate around z
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));


	// ET: scale
	m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale));
}


// ET: Uploads the modelMextrix to shader
// ET: Happens before render so GPU knows how to do it correctly
void GameObject::PreRender()
{
	// Setup model transform
	GLint pLocation;
	Helper::SetUniformLocation(m_ShaderProg, "modelMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&m_worldMatrix);
}


// ET: Base version is empty
// ET: It's meant to be overridden by a specific object type
void GameObject::Render()
{
	//I have nothing to draw
}

// ET: For set up if needed (linking to lights, scene)
void GameObject::Init(Scene* _scene)
{
	//I have nothing to link up to
}
