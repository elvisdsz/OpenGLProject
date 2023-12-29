#pragma once

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

class Camera
{
private:
	mutable glm::vec3 m_Position;
	static constexpr float m_MovementSpeed = 2.0f;

public:
	Camera(glm::vec3 position);
	~Camera();

	void OnUpdate(GLFWwindow* window) const;
	glm::mat4 GetCameraMatrix() const;
};