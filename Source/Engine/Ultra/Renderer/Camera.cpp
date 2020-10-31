#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

static constexpr float PI = 3.14159f;

namespace Ultra {

PerspectiveCamera::PerspectiveCamera(glm::mat4 projection):
	ProjectionMatrix (projection) {
}



PerspectiveCamera::PerspectiveCamera(float left, float right, float buttom, float top) {
	switch (Type) {
		case CameraTypes::Orthographic: {
			ProjectionMatrix = glm::ortho(left, right, buttom, top, -1.0f, 1.0f);
			break;
		}
		case CameraTypes::Perspective: {
			ProjectionMatrix = glm::perspectiveFov(glm::radians(45.0f), right - left, buttom - top, 0.1f, 1000000.0f);
			Pitch = 90.0f;
			break;
		}
	}
	ViewMatrix = glm::mat4{ 1.0f };
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

const glm::vec3 PerspectiveCamera::GetDirection(CameraDirection direction) const {
	switch (direction) {
		// X-Axis
		case CameraDirection::Left:
		case CameraDirection::Right: {
			return glm::rotate(glm::quat(glm::vec3(-Pitch, -Yaw, -Roll)), glm::vec3(1.0f, 0.0f, 0.0f));
		}

		// Y-Axis
		case CameraDirection::Down:
		case CameraDirection::Up: {
			return glm::rotate(glm::quat(glm::vec3(-Pitch, -Yaw, -Roll)), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		// Z-Axis
		case CameraDirection::Backward:
		case CameraDirection::Forward: {
			return glm::rotate(glm::quat(glm::vec3(-Pitch, -Yaw, -Roll)), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		
		default: {
			return glm::vec3{ 0.0f };
		}
	}
}

const glm::vec3 &PerspectiveCamera::GetPosition() const {
	return Position;
}


void PerspectiveCamera::SetProjection(float left, float right, float buttom, float top) {
	ProjectionMatrix = { glm::ortho(left, right, buttom, top, -1.0f, 1.0f) };
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void PerspectiveCamera::CalculateViewMatrix() {
	// Calculate the new Front vector
	//glm::vec3 front;
	//front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	//front.y = sin(glm::radians(Pitch));
	//front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	//Front = glm::normalize(front);
	//// Also re-calculate the Right and Up vector
	////Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	////Up = glm::normalize(glm::cross(Right, Front));
	//glm::mat4 transform =  glm::lookAt(Position, Position + Front, Up);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(Pitch),	glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(Yaw),	glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(Roll),	glm::vec3(0, 0, 1));

	ViewMatrix = glm::inverse(transform);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}



/**
* @brief Orthographic Camera
*/

OrthographicCamera::OrthographicCamera(float left, float right, float buttom, float top):
	ProjectionMatrix { glm::ortho(left, right, buttom, top, -1.0f, 1.0f) },
	ViewMatrix { 1.0f } {
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void OrthographicCamera::SetProjection(float left, float right, float buttom, float top) {
	ProjectionMatrix = { glm::ortho(left, right, buttom, top, -1.0f, 1.0f) };
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void OrthographicCamera::CalculateViewMatrix() {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0, 0, 1));
	ViewMatrix = glm::inverse(transform);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

}
