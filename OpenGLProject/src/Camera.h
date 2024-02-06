#pragma once

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

class Camera
{
private:
	static constexpr bool  IS3D = true;
	static constexpr float FOVY = 60.0f;
	static constexpr glm::vec3 DEFAULT_STARTING_POSITION = { 0.0f, 0.0f, IS3D ? 700.0f : 1.0f };

	static constexpr float m_MovementSpeed = 2.0f;
	static constexpr float m_ZoomSpeed = 1.0f;
	static constexpr float m_MouseSensitivity = 0.1f;

	static constexpr float Z_NEAR = 0.1f;
	static constexpr float Z_FAR = 1000.0f;

	glm::vec3 m_Position;
	glm::vec3 m_CameraFront;
	glm::vec3 m_CameraUp;
	float m_Width, m_Height;
	float m_AspectRatio;

	float m_ZoomUnits;

	glm::mat4 m_CamMatrix;

	// Mouse position
	float m_MousePrevX, m_MousePrevY;
	bool m_MouseStarted;

	// Euler angles for camera orientation
	float m_Yaw; // Yaw is initialized facing -Z
	float m_Pitch; // Pitch is initialized facing the horizon

	void UpdateCameraMatrix();

public:
	Camera(float width, float height, glm::vec3 position = DEFAULT_STARTING_POSITION);
	~Camera();

	void OnUpdate(GLFWwindow* window, float mouseOffsetX = 0.f, float mouseOffsetY = 0.f);
	void OnImGuiRender() const;

	glm::vec3 GetCameraPosition() const;
	glm::mat4 GetCameraMatrix() const;
	bool HandleMouse(float xpos, float ypos);

};