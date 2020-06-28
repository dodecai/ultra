#pragma once

#include <glm/glm.hpp>

#include "Ultra.pch"
#include "Core.h"

namespace Ultra {

class OrthographicCamera {
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ViewProjectionMatrix;

	glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
	float Rotation = 0.0f;

public:
	OrthographicCamera(float left, float right, float buttom, float top);

	const glm::mat4 &GetProjectionMatrix() const { return ProjectionMatrix; }
	const glm::mat4 &GetViewMatrix() const { return ViewMatrix; }
	const glm::mat4 &GetViewProjectionMatrix() const { return ViewProjectionMatrix; }

	const glm::vec3 &GetPosition() const { return Position; }
	const float &GetRotation() const { return Rotation; }

	void SetProjection(float left, float right, float buttom, float top);
	void SetPosition(const glm::vec3 &position) { Position = position; CalculateViewMatrix(); }
	void SetRotation(const float rotation) { Rotation = rotation; CalculateViewMatrix(); }
	
private:
	void CalculateViewMatrix();
};

}
