#include "Camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(glm::vec3 position, float width, float height)
	: m_Position(position), m_Orientation(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_Width(width), m_Height(height), m_AspectRatio(width/height), m_ZoomUnits(0.0f),
	m_MousePrevX(0.f), m_MousePrevY(0.f), m_MouseStarted(false),
	m_Yaw(-90.0f), m_Pitch(0.0f) // Yaw is initialized facing -Z and pitch to facing the horizon
{
	const glm::mat4 viewMatrix = glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 projMatrix = glm::ortho(0.0f, m_Width, 0.0f, m_Height, Z_NEAR, Z_FAR);
	m_CamMatrix = projMatrix * viewMatrix;
}

Camera::~Camera()
{
}

void Camera::OnUpdate(GLFWwindow* window, float mouseOffsetX, float mouseOffsetY)
{
	bool modified = false;

	// Lateral movements
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Position += m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
		modified = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Position += m_MovementSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);
		modified = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Position += m_MovementSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
		modified = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Position += m_MovementSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
		modified = true;
	}

	// Zoom
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		m_ZoomUnits += m_ZoomSpeed;
		modified = true;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		m_ZoomUnits -= m_ZoomSpeed;
		modified = true;
	}

	
	// Orientation
	/*bool orientationChanged = false;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		m_Yaw += m_MovementSpeed;
		orientationChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		m_Yaw -= m_MovementSpeed;
		orientationChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		m_Pitch -= m_MovementSpeed;
		orientationChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		m_Pitch += m_MovementSpeed;
		orientationChanged = true;
	}

	if(orientationChanged)
	{
		// Clamp pitch to avoid flipping the camera
		if (m_Pitch > 89.0f) m_Pitch = 89.0f;
		if (m_Pitch < -89.0f) m_Pitch = -89.0f;

		m_Orientation.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Orientation.y = sin(glm::radians(m_Pitch));
		m_Orientation.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Orientation = glm::normalize(m_Orientation);

		modified = true;
	}*/

	if (HandleMouse(mouseOffsetX, mouseOffsetY))
	{
		modified = true;
	}

	// Update matrices
	if (modified)
	{
		UpdateCameraMatrix();
	}
}

void Camera::UpdateCameraMatrix()
{
	const glm::mat4 viewMatrix = glm::lookAt(m_Position, m_Position + m_Orientation, glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 projMatrix = glm::ortho(m_ZoomUnits * m_AspectRatio, m_Width - (m_ZoomUnits * m_AspectRatio), m_ZoomUnits, m_Height - m_ZoomUnits, Z_NEAR, Z_FAR);
	m_CamMatrix = projMatrix * viewMatrix;
}

glm::mat4 Camera::GetCameraMatrix() const
{
	return m_CamMatrix;
}

bool Camera::HandleMouse(float xpos, float ypos)
{
	if (!m_MouseStarted)
	{
		m_MousePrevX = xpos;
		m_MousePrevY = ypos;
		m_MouseStarted = true;
	}

	float xOffset = xpos - m_MousePrevX;
	float yOffset = m_MousePrevY - ypos;

	if (xOffset == 0.f && yOffset == 0.f)
		return false;

	m_MousePrevX = xpos;
	m_MousePrevY = ypos;

	xOffset *= m_MouseSensitivity;
	yOffset *= m_MouseSensitivity;

	m_Yaw += xOffset;
	m_Pitch += yOffset;

	// Clamp pitch to avoid flipping the camera
	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	m_Orientation.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Orientation.y = sin(glm::radians(m_Pitch));
	m_Orientation.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Orientation = glm::normalize(m_Orientation);

	return true;
}
