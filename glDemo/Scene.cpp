#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp> 
#include <algorithm>
#include "GameObject.h"
#include "CameraFactory.h"
#include "Camera.h"
#include "LightFactory.h"
#include "Light.h"
#include "ModelFactory.h"
#include "model.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObjectFactory.h"
#include <assert.h>
#include "ArcballCamera.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	//TODO: We are being really naught and not deleting everything as we finish
	//what shoudl really go here and in similar places throughout the code base?
}

//tick all my Game Objects, lights and cameras
void Scene::Update(float _dt)
{
	//update all lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	//update all cameras
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	//update all GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(_dt);
	}
}

void Scene::AddGameObject(GameObject* _new)
{
	m_GameObjects.push_back(_new);
}

//I want THAT Game Object by name
GameObject* Scene::GetGameObject(string _GOName)
{
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if ((*it)->GetName() == _GOName)
		{
			return (*it);
		}
	}
	printf("Unknown Game Object NAME : %s \n", _GOName.c_str());
	assert(0);
	return nullptr;
}

Camera* Scene::GetCamera(string _camName)
{
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		if ((*it)->GetName() == _camName)
		{
			return (*it);
		}
	}
	printf("Unknown Camera NAME : %s \n", _camName.c_str());
	assert(0);
	return nullptr;
}
//ET: This is the function that allows us to move the active camera
Camera* Scene::GetCurrentCamera() const  
{  
   return m_useCamera;  
}

Light* Scene::GetLight(string _lightName)
{
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		if ((*it)->GetName() == _lightName)
		{
			return (*it);
		}
	}
	printf("Unknown Light NAME : %s \n", _lightName.c_str());
	assert(0);
	return nullptr;
}

Texture* Scene::GetTexture(string _texName)
{
	for (list<Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		if ((*it)->GetName() == _texName)
		{
			return (*it);
		}
	}
	printf("Unknown Texture NAME : %s \n", _texName.c_str());
	assert(0);
	return nullptr;
}

Model* Scene::GetModel(string _modelName)
{
	for (list<Model*>::iterator it = m_Models.begin(); it != m_Models.end(); it++)
	{
		if ((*it)->GetName() == _modelName)
		{
			return (*it);
		}
	}
	printf("Unknown Model NAME : %s \n", _modelName.c_str());
	assert(0);
	return nullptr;
}

Shader* Scene::GetShader(string _shaderName)
{
	for (list<Shader*>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); it++)
	{
		if ((*it)->GetName() == _shaderName)
		{
			return (*it);
		}
	}
	printf("Unknown Shader NAME : %s \n", _shaderName.c_str());
	assert(0);
	return nullptr;
}


//Render Everything
void Scene::Render()
{
	// RENDERPASS: OPAQUE

	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if ((*it)->GetRP() & RP_OPAQUE)
		{
			//set shader program using
			GLuint SP = (*it)->GetShaderProg();
			glUseProgram(SP);

			//set up for uniform shader values for current camera
			m_useCamera->SetRenderValues(SP);

			//loop through setting up uniform shader values for anything else
			SetShaderUniforms(SP);

			//set any uniform shader values for the actual model
			(*it)->PreRender();

			//actually render the GameObject
			(*it)->Render();
		}


		// RENDER PASS: TRANSPARENT
		//ET: This is where we are going to render the transparent objects, sorted from back-to front

		/// Old version: doesn't work for blending
		//
		//if ((*it)->GetRP() & RP_TRANSPARENT)
		//{

		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//	glDepthMask(GL_FALSE);

		//	//set shader program using
		//	GLuint SP = (*it)->GetShaderProg();
		//	glUseProgram(SP);

		//	//set up for uniform shader values for current camera
		//	m_useCamera->SetRenderValues(SP);

		//	//loop through setting up uniform shader values for anything else
		//	SetShaderUniforms(SP);

		//	//set any uniform shader values for the actual model
		//	(*it)->PreRender();

		//	//actually render the GameObject
		//	(*it)->Render();

		//	glDepthMask(GL_TRUE);
		//	glDisable(GL_BLEND);
		//}
	}

	// New Version 
	// ET: Collects Transparent objects, sorts them, renders them based on depth:


	// Temporary list for transparent objects
	std::vector<GameObject*> transparentObjects;

	// Collection of transparent objects
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		if ((*it)->GetRP() & RP_TRANSPARENT)
		{
			transparentObjects.push_back(*it);
		}
	}

	// Sorting collected objects based on distance to camera
	//ET: = from back (far) to front (close)
	std::sort(transparentObjects.begin(), transparentObjects.end(),
		[this](GameObject* a, GameObject* b) {
			glm::vec3 camPos = m_useCamera->GetPos(); //has to be vec3
			glm::vec3 aPos = a->GetPos();
			glm::vec3 bPos = b->GetPos();

			float distA = glm::distance(camPos, aPos);
			float distB = glm::distance(camPos, bPos);

			return distA > distB; // weiter weg zuerst
		});


	// rendering of transparent objects in "correct" order
	for (GameObject* go : transparentObjects)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE); //ET: prevents overriding of depth

		GLuint SP = go->GetShaderProg();
		glUseProgram(SP);

		m_useCamera->SetRenderValues(SP);
		SetShaderUniforms(SP);
		go->PreRender();
		go->Render();

		glDepthMask(GL_TRUE); //ET: reactivates
		glDisable(GL_BLEND);
	}
}

void Scene::SetShaderUniforms(GLuint _shaderprog)
{
	//everything needs to know about all the lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->SetRenderValues(_shaderprog);
	}

}

void Scene::Load(ifstream& _file)
{
	string dummy;

	//load Cameras
	_file >> dummy >> m_numCameras; _file.ignore(256, '\n');
	cout << "CAMERAS : " << m_numCameras << endl;
	for (int i = 0; i < m_numCameras; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Camera* newCam = CameraFactory::makeNewCam(type);
		newCam->Load(_file);

		m_Cameras.push_back(newCam);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Lights
	_file >> dummy >> m_numLights; _file.ignore(256, '\n');
	cout << "LIGHTS : " << m_numLights << endl;
	for (int i = 0; i < m_numLights; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Light* newLight = LightFactory::makeNewLight(type);
		newLight->Load(_file);

		m_Lights.push_back(newLight);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Models
	_file >> dummy >> m_numModels; _file.ignore(256, '\n');
	cout << "MODELS : " << m_numModels << endl;
	for (int i = 0; i < m_numModels; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');

		//ET: Scene creates new AI Model 

		//ET: type = "AI" dynamically allocates an AIModel
		Model* newModel = ModelFactory::makeNewModel(type);
		//ET: reads FILE: Assets//....obj fiel as directed to via manifest decleration
		newModel->Load(_file);

		m_Models.push_back(newModel);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Textures
	_file >> dummy >> m_numTextures; _file.ignore(256, '\n');
	cout << "TEXTURES : " << m_numTextures << endl;
	for (int i = 0; i < m_numTextures; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		m_Textures.push_back(new Texture(_file));

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Shaders
	_file >> dummy >> m_numShaders; _file.ignore(256, '\n');
	cout << "SHADERS : " << m_numShaders << endl;
	for (int i = 0; i < m_numShaders; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		m_Shaders.push_back(new Shader(_file));

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load GameObjects
	_file >> dummy >> m_numGameObjects; _file.ignore(256, '\n');
	cout << "GAMEOBJECTS : " << m_numGameObjects << endl;
	for (int i = 0; i < m_numGameObjects; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		GameObject* newGO = GameObjectFactory::makeNewGO(type);
		newGO->Load(_file);

		m_GameObjects.push_back(newGO);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}


}

void Scene::Init()
{
	//initialise all cameras
	//scene is passed down here to allow for linking of cameras to game objects
	int count = 0;
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
	{
		(*it)->Init(100, 100, this);

		//if a camera is called MAIN
		//this will be the starting camera used
		if ((*it)->GetName() == "MAIN")
		{
			m_useCamera = (*it);
			m_useCameraIndex = count;
		}
		count++;
	}

	//if no MAIN camera just use the first one
	if (!m_useCamera)
	{
		m_useCamera = (*m_Cameras.begin());
		m_useCameraIndex = 0;
	}

	//set up links between everything and GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Init(this);
	}

	// Make CAMPC camera follow the BEAST
	Camera* campcCam = GetCamera("CAMPC");
	GameObject* beast = GetGameObject("BEAST");
	if (campcCam && beast) {
		campcCam->SetFollowTarget(beast);
	}
}

//ET: Method for adding camera controls
//ET: Works as "doorway" to move the camera from "outside" (Like main.cpp)
void Scene::MoveActiveCamera(glm::vec3 movement) {
	
	if (m_useCamera) {
		m_useCamera->Move(movement);
	}
}


void Scene::CycleCamera()
{
	m_useCameraIndex++;
	m_useCameraIndex = m_useCameraIndex % m_numCameras;

	auto it = m_Cameras.begin();
	std::advance(it, m_useCameraIndex);

	m_useCamera = *it;

	//ET: Debug-log to see, which camera is active
	std::cout << "Switched to camera: " << m_useCamera->GetName() << std::endl;


}

void Scene::MouseMoved(float dY, float dX) 
{
	if (m_useCamera) 
	{
		m_useCamera->rotateCamera(dY, dX);

	}
	if (m_useCamera->GetType() == "ARCBALL")
	{
		//ET: Arcball camera movement
		ArcballCamera* cam = static_cast<ArcballCamera*>(m_useCamera);
		cam->rotateCamera(dY,dX);
	}
}

void Scene::ManipulateObject(int type, float dir)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		if ((*it)->GetName() == "BEAST")
		{
			///ET: Old logic, where movement was not object-relative
			/*
			if (type == 0)
			{
				(*it)->MoveX(dir);
			}
			else if (type == 1)
			{
				(*it)->MoveY(dir);
			}
			else if (type == 2)
			{
				(*it)->RotateZ(dir);
			}
			else if (type == 3)
			{
				(*it)->MoveZ(dir);
			}
			else if (type == 4)
			{
				(*it)->RotateY(dir);
			}
			*/

			// type: 0 = forward/back, 1 = left/right, 2 = yaw, 3 = up/down
			if (type == 0) // Forward/backward (J+Up/J+Down)
				(*it)->MoveRelative(glm::vec3(0, 0, 1), dir); // Forward in local space
			else if (type == 1) // Left/right (J+Left/J+Right)
				(*it)->MoveRelative(glm::vec3(1, 0, 0), dir); // Left in local space
			else if (type == 2) // Yaw (R+Left/R+Right)
				(*it)->RotateY(dir);
			else if (type == 3) // Up/down (L+Up/L+Down)
				(*it)->MoveY(dir);
		}
	}
}
