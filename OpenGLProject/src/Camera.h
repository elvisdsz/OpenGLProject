#pragma once

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

class Camera
{
private:
	static constexpr float m_MovementSpeed = 2.0f;
	static constexpr float m_ZoomSpeed = 1.0f;
	static constexpr float m_OrientationSensitivity = 0.1f;

	static constexpr float Z_NEAR = 0.1f;
	static constexpr float Z_FAR = 1000.0f;

	glm::vec3 m_Position;
	glm::vec3 m_Orientation;
	float m_Width, m_Height;
	float m_AspectRatio;

	float m_ZoomUnits;

	glm::mat4 m_CamMatrix;

	// Euler angles for camera orientation
	float m_Yaw; // Yaw is initialized facing -Z
	float m_Pitch; // Pitch is initialized facing the horizon

public:
	Camera(glm::vec3 position, float width, float height);
	~Camera();

	void OnUpdate(GLFWwindow* window);
	glm::mat4 GetCameraMatrix() const;
};