#pragma once

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

class Camera
{
private:
	static constexpr float m_MovementSpeed = 2.0f;
	static constexpr float m_ZoomSpeed = 1.0f;

	glm::vec3 m_Position;
	glm::vec3 m_Orientation, m_UpDirection;
	float m_Width, m_Height;
	float m_AspectRatio;

	float m_ZoomUnits;

	glm::mat4 m_CamMatrix;

	// Euler angles for camera orientation
	float m_Yaw = -90.0f; // Yaw is initialized facing -Z
	float m_Pitch = 0.0f; // Pitch is initialized facing the horizon
	const float m_Sensitivity = 0.1f; // for m_Orientation speed

	static constexpr float Z_NEAR = 0.1f;
	static constexpr float Z_FAR = 1000.0f;

public:
	Camera(glm::vec3 position, float width, float height);
	~Camera();

	void OnUpdate(GLFWwindow* window);
	glm::mat4 GetCameraMatrix() const;
};