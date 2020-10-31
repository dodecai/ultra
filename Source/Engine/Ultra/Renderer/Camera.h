#pragma once

#include <glm/glm.hpp>

#include "Ultra.pch"
#include "Ultra/Core.h"

namespace Ultra {

enum class CameraTypes {
    Orthographic    = 0,
    Perspective     = 1
};

class Camera {
public:
    Camera() = default;
    Camera(const glm::mat4 &projection): Projection { projection } {}
    virtual ~Camera() = default;

    const float GetExposure() const { return Exposure; }
    const glm::mat4 &GetProjection() const { return Projection; }

    void SetProjection(const glm::mat4 &projection) { Projection = projection; }

protected:
    float Exposure = 1.0f;
    glm::mat4 Projection = glm::mat4(1.0f);
};


// ToDo: CleanUp old stuff
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

class CameraController;

// ToDo: All-In-One camera class vs specific class
class PerspectiveCamera {
	friend CameraController;

	// Properties
	CameraTypes Type = CameraTypes::Orthographic;

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
	PerspectiveCamera(glm::mat4 projection);
	PerspectiveCamera(float left, float right, float buttom, float top);

	// Accessors
	const glm::vec3 GetDirection(CameraDirection direction) const;
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
