#pragma once

#include "glm/glm.hpp"

class Camera
{
private:
	glm::vec3 m_Position;

public:
	Camera(glm::vec3 position);
	~Camera();

	void OnUpdate(float deltaTime);
	glm::mat4 GetCameraMatrix();
};