#include "Camera.h"
#include "helper.h"
#include <fstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "stringHelp.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::Camera()
{
	m_type = "CAMERA";

	m_theta = 0.0f;
	m_phi = 0.0f;
	m_radius = 15.0f;

	m_fovY = 55.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 500.0f;
}

/////////////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
}

/////////////////////////////////////////////////////////////////////////////////////
// Init() - 
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Init(float _screenWidth, float _screenHeight, Scene* _scene)
{
	///TODO: move the calculation of the Projection Matrix to Camera::Tick
	/// so that we can do the same rescaling of the aspect ratio to match the current window
	//m_aspect_ratio = _screenWidth / _screenHeight;
	//m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);

	std::cout << "Initializing camera with screen width: " << _screenWidth << " and height: " << _screenHeight << std::endl;

	// ET: Check if the screen dimensions are valid (greater than zero)
	if (_screenWidth > 0 && _screenHeight > 0) {
		m_aspect_ratio = _screenWidth / _screenHeight;
	}
	else {
		m_aspect_ratio = 1.0f;
		//ET If not, set a default valid aspect ratio to avoid invalid calculations
		m_aspect_ratio = 1.0f;  // Default 1:1 aspect ratio (square)
	}

	// Set the projection matrix using the initial aspect ratio
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_nearPlane, m_farPlane);
}

//ET: Used to set the camera to follow a target
void Camera::SetFollowTarget(GameObject* target) {
	m_followTarget = target;
}

/////////////////////////////////////////////////////////////////////////////////////
// Tick() - Update the camera's view and projection matrices
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Tick(float _dt)
{
	// Follow target logic
	//ET: If following a target, update the camera position and look-at point
	//ET: We do this, so that the camera can follow a target (creature)
	//ET: This will override any manual camera movement when a follow target is set!
	if (m_followTarget) {
		glm::vec3 targetPos = m_followTarget->GetPos();
		float yaw = glm::radians(m_followTarget->GetYaw()); //ET: Get the yaw of the tracked object

		//ET: We can adjust the offset as needed!!!
		glm::vec3 localOffset(0.0f, 5.0f, -4.0f); // Example offset values, adjust as needed.

		// ET: Rotate the offset by the creature's yaw
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rot * glm::vec4(localOffset, 1.0f));

		m_pos = targetPos + rotatedOffset;
		m_lookAt = targetPos;

		// Recalculate aspect ratio (we could also pass it as an argument if the screen size changes)
		// Here, we're just keeping the logic from the original code:
		// ET: Ensuring aspect ratio is valid by checking screen dimensions
		// COMMENTED OUT: Using m_aspect_ratio instead of recalculating it here
		// if (_screenWidth > 0 && _screenHeight > 0) {
		//    m_aspect_ratio = _screenWidth / _screenHeight;
		// }
		// else {
		//    m_aspect_ratio = 1.0f;  // Default to a 1:1 aspect ratio if either dimension is zero or invalid
		// }

		// We already have m_aspect_ratio set from Init(), no need to do it again here:
		// float aspectRatio = m_aspect_ratio > 0 ? m_aspect_ratio : 1.0f;

	}
	else {
		// ET: Free camera logic (spherical coordinates)
		// Calculate theta and phi in radians
		const float theta = glm::radians<float>(m_theta);
		const float phi = glm::radians<float>(m_phi);
		m_lookAt = m_pos + glm::vec3(sinf(phi) * cosf(theta), sinf(theta), cosf(phi) * cosf(theta));
	}

	// Update the view matrix using the camera position and look-at target
	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, glm::vec3(0, 1, 0));

	// Recalculate the projection matrix with the current aspect ratio
	// COMMENTED OUT: Aspect ratio is already stored in m_aspect_ratio
	// m_projectionMatrix = glm::perspective(glm::radians(m_fovY), aspectRatio, m_nearPlane, m_farPlane);
	m_projectionMatrix = glm::perspective(glm::radians(m_fovY), m_aspect_ratio, m_nearPlane, m_farPlane);

	// Optional: You can print the view matrix or lookAt to verify camera positions
	// cout << glm::to_string(m_lookAt) << endl;
}

/////////////////////////////////////////////////////////////////////////////////////
// Load() - Load camera settings from a file
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Load(ifstream& _file)
{
	// Load camera settings from the file, like position, look-at, FOV, near/far planes
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "LOOKAT", m_lookAt.x, m_lookAt.y, m_lookAt.z);
	StringHelp::Float(_file, "FOV", m_fov);
	StringHelp::Float(_file, "NEAR", m_near);
	StringHelp::Float(_file, "FAR", m_far);
}

/////////////////////////////////////////////////////////////////////////////////////
// SetRenderValues() - Set camera-related values in the shaders
/////////////////////////////////////////////////////////////////////////////////////
void Camera::SetRenderValues(unsigned int _prog)
{
	GLint loc;

	// Set the camera's view matrix in the shader
	if (Helper::SetUniformLocation(_prog, "viewMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(GetView()));

	// Set the camera's projection matrix in the shader
	if (Helper::SetUniformLocation(_prog, "projMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(GetProj()));

	// Set the camera's position in the shader
	if (Helper::SetUniformLocation(_prog, "camPos", &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetPos()));
}

/////////////////////////////////////////////////////////////////////////////////////
// rotateCamera() - Rotate the camera based on user input (theta and phi)
/////////////////////////////////////////////////////////////////////////////////////
void Camera::rotateCamera(float _dTheta, float _dPhi)
{
	// Update the camera's spherical coordinates (theta and phi) to rotate the camera
	m_theta += _dTheta;
	m_phi += _dPhi;

	// You can calculate derived values if necessary (e.g., updating position)
	// calculateDerivedValues();
}


void Camera::setAspect(float _aspect) {

	this->m_aspect_ratio = _aspect;
	
}
