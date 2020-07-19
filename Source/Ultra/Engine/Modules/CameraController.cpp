#include "CameraController.h"

#include <Omnia/System/Input.h>

namespace Ultra {

CameraController::CameraController(float apsectRatio):
	AspectRatio { apsectRatio },
	Bounds { -AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel },
	Camera { Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top } {
	ZoomLevel -= 1.0f * 0.25f;
	ZoomLevel = std::max(ZoomLevel, 0.25f);
	Camera.SetProjection(Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top);
}

void CameraController::Update(Timestamp deltaTime) {
	// Left or Right
	if (Input::GetKeyState(KeyCode::KeyA)) {
		CameraPosition.x -= CameraMoveSpeed * deltaTime;
	} else if (Input::GetKeyState(KeyCode::KeyD)) {
		CameraPosition.x += CameraMoveSpeed * deltaTime;
	}

	// Up or Down
	if (Input::GetKeyState(KeyCode::KeyW)) {
		CameraPosition.y += CameraMoveSpeed * deltaTime;
	} else if (Input::GetKeyState(KeyCode::KeyS)) {
		CameraPosition.y -= CameraMoveSpeed * deltaTime;
	}

	//// Backward or Forward
	//if (Input::GetKeyState(KeyCode::KeyY)) {
	//	CameraPosition.z += CameraMoveSpeed * deltaTime;
	//} else if (Input::GetKeyState(KeyCode::KeyX)) {
	//	CameraPosition.z -= CameraMoveSpeed * deltaTime;
	//}

	// Rotation
	if (Input::GetKeyState(KeyCode::KeyQ)) {
		CameraRotation -= CameraRotationSpeed * deltaTime;
	} else if (Input::GetKeyState(KeyCode::KeyE)) {
		CameraRotation += CameraRotationSpeed * deltaTime;
	}
	Camera.SetRoll(CameraRotation);

	Camera.SetPosition(CameraPosition);
	CameraMoveSpeed = ZoomLevel;
}

void CameraController::Resize(float width, float height) {
	AspectRatio = width / height;
	CalculateView();
}

bool CameraController::Move(CameraMovement direction, float delta) {
	float velocity = MovementSpeed *delta;
	float currentSpeed = Acceleration;
	if (currentSpeed < MovementSpeed) currentSpeed += Acceleration;

	switch(direction) {
		case CameraMovement::Backward:	Camera.Position -= Camera.Forward * velocity;	break;
		case CameraMovement::Down:		Camera.Position += Camera.Up * velocity;		break;
		case CameraMovement::Forward:	Camera.Position += Camera.Forward * velocity;	break;
		case CameraMovement::Left:		Camera.Position -= Camera.Right * velocity;	break;
		case CameraMovement::Right:	Camera.Position += Camera.Right * velocity;	break;
		case CameraMovement::Up:		Camera.Position -= Camera.Up * velocity;		break;
		default:					return false;					break;
	}

	// Stick at Ground Level - make sure the user stays at the ground level
	//Position.y = 0.0f; // <-- this one-liner keeps the user at the ground level (xz plane)

	return true;
}

bool CameraController::Move(CameraMovement direction, float xoffset, float yoffset, bool constrainPitch) {
	switch(direction) {
		case CameraMovement::Sphere:
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Camera.Yaw += xoffset;
			Camera.Pitch += yoffset;

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if(constrainPitch) {
				if(Camera.Pitch > 89.0f)
					Camera.Pitch = 89.0f;
				if(Camera.Pitch < -89.0f)
					Camera.Pitch = -89.0f;
			}

			// Update Front, Right and Up Vectors using the updated Euler angles
			Camera.CalculateViewMatrix();
			break;

		case CameraMovement::Zoom:
			// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis

			if(ZoomLevel >= 1.0f && ZoomLevel <= 45.0f)
				ZoomLevel -= yoffset;
			if(ZoomLevel <= 1.0f)
				ZoomLevel = 1.0f;
			if(ZoomLevel >= 45.0f)
				ZoomLevel = 45.0f;
			break;

		default:
			return false;
			break;
	}

	return true;
}

void CameraController::Event(void *event) {}
void CameraController::KeyboardEvent(KeyboardEventData &data) {}

void CameraController::MouseEvent(MouseEventData &data) {
	ZoomLevel -= data.DeltaWheelY * 0.32f;
	ZoomLevel = std::max(ZoomLevel, 0.12f);
	CalculateView();
}

void CameraController::CalculateView() {
	Bounds = { -AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel };
	Camera.SetProjection(Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top);
}






/**
* @brief Orthographic Camera
*/

OrthographicCameraController::OrthographicCameraController(float apsectRatio, bool rotation):
	AspectRatio { apsectRatio },
	Bounds { -AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel },
	Camera { Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top },
	Rotation{ rotation } {
	ZoomLevel -= 1.0f * 0.25f;
	ZoomLevel = std::max(ZoomLevel, 0.25f);
	Camera.SetProjection(Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top);
}

void OrthographicCameraController::Update(Timestamp deltaTime) {
	this->deltaTime = deltaTime;
	// Left or Right
	if (Input::GetKeyState(KeyCode::KeyA)) {
		CameraPosition.x -= CameraMoveSpeed * deltaTime;
	} else if (Input::GetKeyState(KeyCode::KeyD)) {
		CameraPosition.x += CameraMoveSpeed * deltaTime;
	}

	// Up or Down
	if (Input::GetKeyState(KeyCode::KeyW)) {
		CameraPosition.y += CameraMoveSpeed * deltaTime;
	} else if (Input::GetKeyState(KeyCode::KeyS)) {
		CameraPosition.y -= CameraMoveSpeed * deltaTime;
	}

	// Rotation
	if (Rotation) {
		if (Input::GetKeyState(KeyCode::KeyQ)) {
			CameraRotation -= CameraRotationSpeed * deltaTime;
		} else if (Input::GetKeyState(KeyCode::KeyE)) {
			CameraRotation += CameraRotationSpeed * deltaTime;
		}
		Camera.SetRotation(CameraRotation);
	}

	Camera.SetPosition(CameraPosition);
	CameraMoveSpeed = ZoomLevel;
}

void OrthographicCameraController::Resize(float width, float height) {
	AspectRatio = width / height;
	CalculateView();
}


void OrthographicCameraController::Event(void *event) {}
void OrthographicCameraController::KeyboardEvent(KeyboardEventData &data) {}

void OrthographicCameraController::MouseEvent(MouseEventData &data) {
	ZoomLevel -= data.DeltaWheelY * 0.32f;
	ZoomLevel = std::max(ZoomLevel, 0.12f);
	CalculateView();
}

void OrthographicCameraController::CalculateView() {
	Bounds = { -AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel };
	Camera.SetProjection(Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top);
}

}
