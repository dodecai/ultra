#pragma once

#include <glm/glm.hpp>

#include "Ultra.pch"
#include "Core.h"

namespace Ultra {

struct CameraBounds {
	float Left, Right;
	float Bottom, Top;

	float GetWidth() { return Right - Left; }
	float GetHeight() { return Top - Bottom; }
};

enum class CameraDirection {
	Left,       // X-Axis--
	Right,      // X-Axis++

	Down,       // Y-Axis--
	Up,         // Y-Axis++

	Backward,   // Z-Axis++
	Forward,    // Z-Axis--
};

enum class CameraMovement {
	// 2D
	Left,
	Up,
	Right,
	Down,
	Zoom,

	// >2.5D
	Sphere,

	// 3D
	Backward,
	Forward,
};

enum class CameraType {
	Orthographic,
	Perspective
};

class CameraController;

class Camera {
	friend CameraController;

	// Properties
	CameraType Type = CameraType::Orthographic;

	glm::mat4 ProjectionMatrix = {};
	glm::mat4 ViewMatrix = {};
	glm::mat4 ViewProjectionMatrix = {};

	glm::vec3 Position = {};
	glm::vec3 Forward = {};
	glm::vec3 Up = {};
	glm::vec3 Right = {};
	glm::vec3 World = {};

	float Roll	= 0.0f;		// Roll	 ... around z-axis
	float Pitch = 0.0f;		// Pitch ... around x-axis
	float Yaw	= 0.0f;		// Yaw	 ... around y-Axis

	// Calculates the front vector from the Camera's (updated) Euler Angles
	void CalculateViewMatrix();

public:
	// Constructors
	Camera(glm::mat4 projection);
	Camera(float left, float right, float buttom, float top);

	// Accessors
	const glm::vec3 &GetDirection(CameraDirection direction) const;
	const glm::vec3 &GetPosition() const;

	const glm::mat4 &GetProjectionMatrix() const { return ProjectionMatrix; }
	const glm::mat4 &GetViewMatrix() const { return ViewMatrix; }
	const glm::mat4 &GetViewProjectionMatrix() const { return ViewProjectionMatrix; }

	// Mutators
	void SetPosition(const glm::vec3 &position) { Position = position; CalculateViewMatrix(); }
	void SetProjection(float left, float right, float buttom, float top);

	void SetRoll(float rotation) { Roll = rotation; }
	void SetPitch(float rotation) { Pitch = rotation; }
	void SetYaw(float rotation) { Yaw = rotation; }
};

// ToDo: All-In-One camera class vs specific class
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



class CameraNew {
public:
    CameraNew() = default;
    CameraNew(const glm::mat4 &projection): Projection { projection } {}
    virtual ~CameraNew() = default;

    const glm::mat4 &GetProjection() const { return Projection; };

protected:
    glm::mat4 Projection = glm::mat4(1.0f);
};


}
