#include "Camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"

Camera::Camera(float width, float height, glm::vec3 position /* = DEFAULT_STARTING_POSITION */)
	: m_Position(position), m_CameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_Width(width), m_Height(height), m_AspectRatio(width/height), m_ZoomUnits(0.0f),
	m_MousePrevX(0.f), m_MousePrevY(0.f), m_MouseStarted(false),
	m_Yaw(-90.0f), m_Pitch(0.0f) // Yaw is initialized facing -Z and pitch to facing the horizon
{
	const glm::mat4 viewMatrix = glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 projMatrix = IS3D ?
								  glm::perspective(glm::radians(FOVY), m_Width / m_Height, Z_NEAR, Z_FAR)
								: glm::ortho(0.0f, m_Width, 0.0f, m_Height, Z_NEAR, Z_FAR);
	                            
	m_CamMatrix = projMatrix * viewMatrix;
}

Camera::~Camera() {}

void Camera::OnUpdate(GLFWwindow* window, float mouseOffsetX, float mouseOffsetY)
{
	bool modified = false;

	
	if (IS3D)
	{
		// walk - WASD movements
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Position += m_MovementSpeed * m_CameraFront;
			modified = true;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Position -= m_MovementSpeed * m_CameraFront;
			modified = true;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Position -= m_MovementSpeed * glm::normalize(glm::cross(m_CameraFront, m_CameraUp));
			modified = true;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Position += m_MovementSpeed * glm::normalize(glm::cross(m_CameraFront, m_CameraUp));
			modified = true;
		}

		// vertical movement - EQ
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			m_Position += m_MovementSpeed * m_CameraUp;
			modified = true;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_Position -= m_MovementSpeed * m_CameraUp;
			modified = true; 
		}
	}
	else
	{
		// Lateral movements suitable for 2D
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Position += m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
			modified = true;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Position += m_MovementSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
			modified = true;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Position += m_MovementSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);
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
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		if (HandleMouse(mouseOffsetX, mouseOffsetY))
		{
			modified = true;
		}
	}
	else if (m_MouseStarted) // clear previous mouse position on release
	{
		m_MouseStarted = false;
	}

	// Update matrices
	if (modified)
	{
		UpdateCameraMatrix();
	}
}

void Camera::UpdateCameraMatrix()
{
	const glm::mat4 viewMatrix = glm::lookAt(m_Position, m_Position + m_CameraFront, m_CameraUp);
	const glm::mat4 projMatrix = IS3D ?
								  glm::perspective(glm::radians(FOVY), m_Width / m_Height, Z_NEAR, Z_FAR)
								: glm::ortho(m_ZoomUnits * m_AspectRatio, m_Width - (m_ZoomUnits * m_AspectRatio), m_ZoomUnits, m_Height - m_ZoomUnits, Z_NEAR, Z_FAR);

	m_CamMatrix = projMatrix * viewMatrix;
}

const glm::mat4 Camera::GetCameraMatrix() const
{
	return m_CamMatrix;
}

void Camera::OnImGuiRender() const
{
	ImGui::Begin("Camera Parameters");
	ImGui::Text("Position: (%.4f, %.4f, %.4f)", m_Position.x, m_Position.y, m_Position.z);
	if (!IS3D)
	{
		ImGui::Text("Zoom: %.4f", m_ZoomUnits);
	}
	ImGui::End();
}

const glm::vec3 Camera::GetCameraPosition() const
{
	return m_Position;
}

const glm::vec3 Camera::GetCameraUp()
{
	return m_CameraUp;
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

	m_CameraFront.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CameraFront.y = sin(glm::radians(m_Pitch));
	m_CameraFront.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CameraFront = glm::normalize(m_CameraFront);

	return true;
}
