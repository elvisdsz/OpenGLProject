#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"

Camera::Camera(glm::vec3 position)
	: m_Position(position)
{}

Camera::~Camera()
{
}

void Camera::OnUpdate(float deltaTime)
{

}

glm::mat4 Camera::GetCameraMatrix()
{
	return glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
