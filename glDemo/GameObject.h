
// ET: ensures that file only includes once when the compiler is working -> avoids errors
#pragma once

// Included files
#include "core.h" // ET: Holds engine-specific definitions and settings
#include <stdio.h> // ET: Basic input/output
#include <string> // ET: Lets us use std::string
#include "RenderPass.h" // ET: Defines different rendering categories

using namespace std;
class Scene;

using namespace glm;

//Base class definition of a GameObject
class GameObject
{
// ET: Accessmodifyers allow us to define, who get's access to what: "public" - everyone can, "private" - only class itself, "protected" - like private but inheriting classes can
public:

	// Contstructor
	// ET: Sets up default values
	GameObject();

	// Deconstructor
	// ET: "cleans up" when object is deleted
	virtual ~GameObject();

	// ET: Virtual functions = are meant to be overridden by a subclass (OOP standard):

	// ET: Loads object info from a file
	virtual void Load(ifstream& _file);

	//update the GameObject
	// ET: Updates the logic per frame
	//TODO: possibly pass keyboard / mouse stuff down here for player controls?
	virtual void Tick(float _dt);

	virtual void PreRender();//set up any shader values needed for this object

	virtual void Render();//render this object

	//various getters and setters
	void SetName(string _name) { m_name = _name; }
	string GetName() { return m_name; }
	GLuint GetShaderProg() { return m_ShaderProg; }

	//scene maybe needed for more involved cameras to connect to relvant GOs and lights/shaders etc
	virtual void Init(Scene* _scene);

	//this GameObject should be drawn in THIS render pass
	RenderPass GetRP() { return m_RP; }

// ET: Internal variables
protected:

	string m_name;// ET: Name
	string m_type;// ET: Type
 
	vec3		m_pos;// ET: Position
	vec3		m_rot;// ET: Rotation
	vec3		m_scale;// ET: Scale
	vec3		m_rot_incr;// ET: How much it spins per frame

	// ET: Transformation matrix used to position and transform object in the world
	glm::mat4	m_worldMatrix;

	// ET: Shader program used by this object
	GLuint m_ShaderProg;

	// ET: Render cetegory of the object
	RenderPass m_RP = RP_OPAQUE;
};

