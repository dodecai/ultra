#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace core {

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
	// Properties
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	static inline const float ACCELERATION = 0.1f;
	static inline const float YAW = -90.0f;
	static inline const float PITCH = 0.0f;
	static inline const float SPEED = 2.5f;
	static inline const float SENSITIVITY = 0.1f;
	static inline const float ZOOM = 45.0f;
	

	// States
	float Acceleration;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	
	float Yaw;
	float Pitch;
	float Roll;

	// Calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors() {
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}

public:
	// Camera Directions: Abstraction of several possible options for camera movement.
	enum class Direction {
		Backward,
		Down,
		Forward,
		Left,
		Right,
		Sphere,
		Up,
		Zoom
  };

	// Constructors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = Camera::YAW, float pitch = PITCH): Front(glm::vec3(0.0f, 0.0f, -1.0f)), Acceleration(ACCELERATION), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		Roll = 0.0f;
		UpdateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch): Front(glm::vec3(0.0f, 0.0f, -1.0f)), Acceleration(ACCELERATION),MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		Roll = 0.0f;
		UpdateCameraVectors();
	}

	// Accessors
	glm::vec3 GetPosition() const { return Position; };
	float GetZoom() const { return Zoom; };
	// - Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }

	// Camera Movement: Processes input received from any input system.
	bool Move(Direction direction, float delta) {
		float velocity = MovementSpeed *delta;
		float currentSpeed = Acceleration;
		if (currentSpeed < MovementSpeed) currentSpeed += Acceleration;

		switch(direction) {
			case Direction::Backward:	Position -= Front * velocity;	break;
			case Direction::Down:		Position += Up * velocity;		break;
			case Direction::Forward:	Position += Front * velocity;	break;
			case Direction::Left:		Position -= Right * velocity;	break;
			case Direction::Right:		Position += Right * velocity;	break;
			case Direction::Up:			Position -= Up * velocity;		break;
			default:					return false;					break;
		}
		
		// Stick at Ground Level - make sure the user stays at the ground level
		//Position.y = 0.0f; // <-- this one-liner keeps the user at the ground level (xz plane)

		return true;
	}
	
	// Camera Movement: Processes input received from an mouse input system. Expects the offset value in both x and y directions.
	bool Move(Direction direction, float xoffset, float yoffset, GLboolean constrainPitch = true) {

		switch(direction) {
			case Direction::Sphere:
				xoffset *= MouseSensitivity;
				yoffset *= MouseSensitivity;

				Yaw += xoffset;
				Pitch += yoffset;

				// Make sure that when pitch is out of bounds, screen doesn't get flipped
				if(constrainPitch) {
					if(Pitch > 89.0f)
						Pitch = 89.0f;
					if(Pitch < -89.0f)
						Pitch = -89.0f;
				}

				// Update Front, Right and Up Vectors using the updated Euler angles
				UpdateCameraVectors();
				break;

			case Direction::Zoom:
				// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	
				if(Zoom >= 1.0f && Zoom <= 45.0f)
					Zoom -= yoffset;
				if(Zoom <= 1.0f)
					Zoom = 1.0f;
				if(Zoom >= 45.0f)
					Zoom = 45.0f;
				break;

			default:
				return false;
				break;
		}

		return true;
	}
};

}

#endif