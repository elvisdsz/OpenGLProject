#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"

Camera::Camera(glm::vec3 position)
	: m_Position(position)
{}

Camera::~Camera()
{
}

void Camera::OnUpdate(GLFWwindow* window) const
{
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Position += m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Position += m_MovementSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Position += m_MovementSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Position += m_MovementSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
	}

	//m_Position = movement;
}

glm::mat4 Camera::GetCameraMatrix() const
{
	return glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
