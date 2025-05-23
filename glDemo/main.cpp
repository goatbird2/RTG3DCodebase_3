
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "GUClock.h"
#include "PrincipleAxes.h"
#include "shader_setup.h"
#include "helper.h"
#include "AIMesh.h"
#include "Cube.h"
#include "Scene.h"


using namespace std;
using namespace glm;

//testforsomethingtesttesttest


#pragma region Global variables

GUClock* g_gameClock = nullptr;

// Mouse tracking
bool				g_mouseDown = false;
double				g_prevMouseX, g_prevMouseY;

// Global Example objects
// shouldn't really be anything in here for the final submission
ArcballCamera* g_mainCamera = nullptr;
CGPrincipleAxes* g_principleAxes = nullptr;
Cube* g_cube = nullptr;

GLuint g_flatColourShader;

GLuint g_texDirLightShader;
vec3 g_DLdirection = vec3(0.0f, 1.0f, 0.0f);
vec3 g_DLcolour = vec3(1.0f, 1.0f, 1.0f);
vec3 g_DLambient = vec3(0.2f, 0.2f, 0.2f);

AIMesh* g_creatureMesh = nullptr;
vec3 g_beastPos = vec3(2.0f, 0.0f, 0.0f);
float g_beastRotation = 0.0f;
AIMesh* g_planetMesh = nullptr;

int g_showing = 0;
int g_NumExamples = 3;

bool jPressed = false;
bool lPressed = false;
bool rPressed = false;

//Global Game Object
Scene* g_Scene = nullptr;

// Window size
const unsigned int g_initWidth = 512;
const unsigned int g_initHeight = 512;

#pragma endregion


// Function prototypes
void renderScene();
void updateScene();
void resizeWindow(GLFWwindow* _window, int _width, int _height);
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos);
void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods);
void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset);
void mouseEnterHandler(GLFWwindow* _window, int _entered);


int main()
{
	//
	// 1. Initialisation
	//

	g_gameClock = new GUClock();

#pragma region OpenGL and window setup

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(g_initWidth, g_initHeight, "GDV5001", NULL, NULL);

	// Check window was created successfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, resizeWindow); // resize window callback
	glfwSetKeyCallback(window, keyboardHandler); // Keyboard input callback
	glfwSetCursorPosCallback(window, mouseMoveHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetScrollCallback(window, mouseScrollHandler);
	glfwSetCursorEnterCallback(window, mouseEnterHandler);

	// Initialise glew
	glewInit();


	// Setup window's initial size
	resizeWindow(window, g_initWidth, g_initHeight);

#pragma endregion

	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black
	glClearDepth(1.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);


	//ET: Enables Depth buffer (z buffer)
	// ET: Checks every time open GL wants to draw a new pixel "Is this new pixel closer to the camera than the pixel already drawn here?"
	// Yes: new pixel replaces old one
	// NO: New pixel gets ignored
	// -> This is how objects get drawn in correct distance
	// rule is set specifically in GL_LEQUAL below
	glEnable(GL_DEPTH_TEST);

	//ET: allows us to calculate half-transparent Pixel correctly with BG
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// ET: LEQUAL = Less than equal
	// ET: "Only draw a pixel, if its depth (=distance to cam) is smaller or equal to what is already there"
	glDepthFunc(GL_LEQUAL);

	//
	// Setup the Example Objects
	//

	g_texDirLightShader = setupShaders(string("Assets\\Shaders\\texture-directional.vert"), string("Assets\\Shaders\\texture-directional.frag"));
	g_flatColourShader = setupShaders(string("Assets\\Shaders\\flatColour.vert"), string("Assets\\Shaders\\flatColour.frag"));

	g_mainCamera = new ArcballCamera(0.0f, 0.0f, 1.98595f, 55.0f, 1.0f, 0.1f, 500.0f);

	g_principleAxes = new CGPrincipleAxes();

	g_cube = new Cube();



	g_creatureMesh = new AIMesh(string("Assets\\beast\\beast.obj"));
	if (g_creatureMesh) {
		g_creatureMesh->addTexture(string("Assets\\beast\\beast_texture.bmp"), FIF_BMP);
	}

	g_planetMesh = new AIMesh(string("Assets\\gsphere.obj"));
	if (g_planetMesh) {
		g_planetMesh->addTexture(string("Assets\\Textures\\Hodges_G_MountainRock1.jpg"), FIF_JPEG);
	}

	//
	//Set up Scene class
	//

	g_Scene = new Scene();

	ifstream manifest;
	manifest.open("manifest.txt");

	g_Scene->Load(manifest);
	g_Scene->Init();

	manifest.close();


	//
	// Main loop
	// 

	while (!glfwWindowShouldClose(window))
	{
		updateScene();
		renderScene();						// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		glfwPollEvents();					// Use this version when animating as fast as possible

		// update window title
		char timingString[256];
		sprintf_s(timingString, 256, "CIS5013: Average fps: %.0f; Average spf: %f", g_gameClock->averageFPS(), g_gameClock->averageSPF() / 1000.0f);
		glfwSetWindowTitle(window, timingString);
	}

	glfwTerminate();

	if (g_gameClock)
	{
		g_gameClock->stop();
		g_gameClock->reportTimingData();
	}

	return 0;
}


// renderScene - function to render the current scene
void renderScene()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 cameraTransform = g_mainCamera->projectionTransform() * g_mainCamera->viewTransform();

	mat4 cameraProjection = g_mainCamera->projectionTransform();
	mat4 cameraView = g_mainCamera->viewTransform() * translate(identity<mat4>(), -g_beastPos);

// Render principle axes - no modelling transforms so just use cameraTransform
	if (true)
	{
		// Render axes 
		glUseProgram(g_flatColourShader);
		GLint pLocation;
		Helper::SetUniformLocation(g_flatColourShader, "viewMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraView);
		Helper::SetUniformLocation(g_flatColourShader, "projMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraProjection);
		Helper::SetUniformLocation(g_flatColourShader, "modelMatrix", &pLocation);
		mat4 modelTransform = identity<mat4>();
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

		//g_principleAxes->render();
	}

	//switch (g_showing)
	//{
	/*case 0:
	{
		glUseProgram(g_texDirLightShader);*/

	//	GLint pLocation;
	//	Helper::SetUniformLocation(g_texDirLightShader, "viewMatrix", &pLocation);
	//	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraView);
	//	Helper::SetUniformLocation(g_texDirLightShader, "projMatrix", &pLocation);
	//	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraProjection);
	//	Helper::SetUniformLocation(g_texDirLightShader, "texture", &pLocation);
	//	glUniform1i(pLocation, 0); // set to point to texture unit 0 for AIMeshes
	//	Helper::SetUniformLocation(g_texDirLightShader, "DIRDir", &pLocation);
	//	glUniform3fv(pLocation, 1, (GLfloat*)&g_DLdirection);
	//	Helper::SetUniformLocation(g_texDirLightShader, "DIRCol", &pLocation);
	//	glUniform3fv(pLocation, 1, (GLfloat*)&g_DLcolour);
	//	Helper::SetUniformLocation(g_texDirLightShader, "DIRAmb", &pLocation);
	//	glUniform3fv(pLocation, 1, (GLfloat*)&g_DLambient);
	//	if (g_creatureMesh) {

	//		// Setup transforms
	//		Helper::SetUniformLocation(g_texDirLightShader, "modelMatrix", &pLocation);
	//		mat4 modelTransform = glm::translate(identity<mat4>(), g_beastPos) * eulerAngleY<float>(glm::radians<float>(g_beastRotation));
	//		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

	//		g_creatureMesh->setupTextures();
	//		g_creatureMesh->render();
	//	}

	//	if (g_planetMesh) {

	//		// Setup transforms
	//		Helper::SetUniformLocation(g_texDirLightShader, "modelMatrix", &pLocation);
	//		mat4 modelTransform = glm::translate(identity<mat4>(), vec3(4.0, 4.0, 4.0));
	//		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

	//		g_planetMesh->setupTextures();
	//		g_planetMesh->render();
	//	}
	//}
	//break;

	//case 1:
	//{
	//	// Render cube 
	//	glUseProgram(g_flatColourShader);
	//	GLint pLocation;
	//	Helper::SetUniformLocation(g_flatColourShader, "viewMatrix", &pLocation);
	//	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraView);
	//	Helper::SetUniformLocation(g_flatColourShader, "projMatrix", &pLocation);
	//	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraProjection);
	//	Helper::SetUniformLocation(g_flatColourShader, "modelMatrix", &pLocation);
	//	mat4 modelTransform = glm::translate(identity<mat4>(), vec3(2.0, 0.0, 2.0));
	//	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

	//	g_cube->render();
	//	break;
	/*}
	case 2:*/
		g_Scene->Render();
	//}

}


// Function called to animate elements in the scene
void updateScene() 
{
	float tDelta = 0.0f;

	if (g_gameClock) {

		g_gameClock->tick();
		tDelta = (float)g_gameClock->gameTimeDelta();
	}

	g_Scene->Update(tDelta);
}


#pragma region Event handler functions
//none of this is currently passed to the Game object
//probably a good idea to do that

void resizeWindow(GLFWwindow* _window, int _width, int _height)  
{  
   if (g_mainCamera) {  
       g_mainCamera->setAspect((float)_width / (float)_height);  
   }  
   if (g_Scene && g_Scene->GetCurrentCamera()) {  
       g_Scene->GetCurrentCamera()->setAspect((float)_width / (float)_height);
   }  
   glViewport(0, 0, _width, _height); // Draw into entire window  
}


// Function to call to handle keyboard input
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	if (_action == GLFW_PRESS) {

		//ET: check which key was pressed and then do...
		switch (_key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(_window, true);
			break;

			//ET: We no longer want to use space to cycle trough the different "views" but want to use it to cycle trough the cameras isntead!
		case GLFW_KEY_SPACE:
			/*g_showing++;
			g_showing = g_showing % g_NumExamples;*/
			g_Scene->CycleCamera();


		//ET: We are adding a bunch of GLFW_PRESS keyss to expand keyboard handler for our camera controls! 
		

		case GLFW_KEY_S:
			//g_Scene->MoveActiveCamera(glm::vec3(0.0f, 0.0f, -0.5f)); //ET: BACKWARDS Non Targetbound CAMERA
			g_Scene->GetCurrentCamera()->MoveLocal(glm::vec3(0, 0, -1), 0.5f); //ET: Move camera backwards relative to view

			break;

		case GLFW_KEY_W:
			//g_Scene->MoveActiveCamera(glm::vec3(0.0f, 0.0f, 0.5f)); //ET: FORWARDS Non Targetbound CAMERA
			g_Scene->GetCurrentCamera()->MoveLocal(glm::vec3(0, 0, 1), 0.5f); //ET: Move camera forwards relative to view
			break;

		case GLFW_KEY_D:
			//g_Scene->MoveActiveCamera(glm::vec3(-0.5f, 0.0f, 0.0f)); //ET: RIGHT Non Targetbound CAMERA
			g_Scene->GetCurrentCamera()->MoveLocal(glm::vec3(1, 0, 0), 0.5f); //ET: Move camera right relative to view
			break;

		case GLFW_KEY_A:
			//g_Scene->MoveActiveCamera(glm::vec3(0.5f, 0.0f, 0.0f)); //ET: UPWARDS Non Targetbound CAMERA
			g_Scene->GetCurrentCamera()->MoveLocal(glm::vec3(-1, 0, 0), 0.5f); //ET: Move camera left relative to view
			break;

		case GLFW_KEY_Q:
			g_Scene->MoveActiveCamera(glm::vec3(0.0f, 0.5f, 0.0f)); //ET: FORWARDS Non Targetbound CAMERA
			g_Scene->GetCurrentCamera()->MoveLocal(glm::vec3(0, 1, 0), 0.5f); //ET: Move camera up relative to view
			break;

		case GLFW_KEY_E:
			//g_Scene->MoveActiveCamera(glm::vec3(0.0f, -0.5f, 0.0f)); //ET: LEFT Non Targetbound CAMERA
			g_Scene->GetCurrentCamera()->MoveLocal(glm::vec3(0, -1, 0), 0.5f); //ET: Move camera down relative to view
			break;

		case GLFW_KEY_L:
			lPressed = true;
			break;

		case GLFW_KEY_J:
			jPressed = true;
			break;

		case GLFW_KEY_R:
			rPressed = true;
			break;

		case GLFW_KEY_UP:
			if (jPressed)
			{
				
				std::cout << "j and up pressed " << std::endl;
				//g_Scene->ManipulateObject(3, 1.0f);
				g_Scene->ManipulateObject(0, 0.5f); // Forwards Beast
			
			}
			else if (lPressed)
			{
				
				std::cout << "l and up pressed " << std::endl;
				//g_Scene->ManipulateObject(1, 1.0f);
				g_Scene->ManipulateObject(3, 0.5f);

			}
			break;

		case GLFW_KEY_DOWN:
			if (jPressed)
			{
				std::cout << "j and down pressed " << std::endl;
				//g_Scene->ManipulateObject(3, -1.0f);
				g_Scene->ManipulateObject(0, -0.5f); // Backwards Beast
			}
			else if (lPressed)
			{
				std::cout << "l and down pressed " << std::endl;
				//g_Scene->ManipulateObject(1, -1.0f);
				g_Scene->ManipulateObject(3, -0.5f); // Down Beast
			}
			break;


		case GLFW_KEY_RIGHT:
			if (jPressed)
			{

				std::cout << "j and right pressed " << std::endl;
				//g_Scene->ManipulateObject(0, -1.0f);
				g_Scene->ManipulateObject(1, -0.5f); // Right Beast
			}

			else if (rPressed)
			{
				std::cout << "r and down pressed " << std::endl;
				g_Scene->ManipulateObject(2, -45.0f);
			}
			break;

		case GLFW_KEY_LEFT:
			if (jPressed)
			{

				std::cout << "j and left pressed " << std::endl;
				//g_Scene->ManipulateObject(0, 1.0f);
				g_Scene->ManipulateObject(1, 0.5f); // Left Beast
			}

			else if (rPressed)
			{

				std::cout << "r and up pressed " << std::endl;
				g_Scene->ManipulateObject(2, 45.0f);
			}
			break;
			
		default:
		{
		}
		}
	}
	else if (_action == GLFW_RELEASE) 
	{
		// handle key release events
		switch (_key)
		{

		case GLFW_KEY_L:
			lPressed = false;
			break;

		case GLFW_KEY_J:
			jPressed = false;
			break;

		case GLFW_KEY_R:
			rPressed = false;
			break;
		default:
		{
		}
		}
	}
}


void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos) 
{
	if (g_mouseDown) {

		//float tDelta = gameClock->gameTimeDelta();

		float dx = float(_xpos - g_prevMouseX);// *360.0f * tDelta;
		float dy = float(_ypos - g_prevMouseY);// *360.0f * tDelta;

		if (g_mainCamera)
			g_mainCamera->rotateCamera(-dy, -dx);

		g_prevMouseX = _xpos;
		g_prevMouseY = _ypos;

		g_Scene->MouseMoved(-dy, -dx);
	}
}

void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods) 
{
	if (_button == GLFW_MOUSE_BUTTON_LEFT) 
	{
		if (_action == GLFW_PRESS) 
		{
			g_mouseDown = true;
			glfwGetCursorPos(_window, &g_prevMouseX, &g_prevMouseY);
		}
		else if (_action == GLFW_RELEASE) 
		{
			g_mouseDown = false;
		}
	}
}

void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset) {

	if (g_mainCamera) 
	{
		if (_yoffset < 0.0)
			g_mainCamera->scaleRadius(1.1f);
		else if (_yoffset > 0.0)
			g_mainCamera->scaleRadius(0.9f);
	}
}

void mouseEnterHandler(GLFWwindow* _window, int _entered) 
{
}

#pragma endregion