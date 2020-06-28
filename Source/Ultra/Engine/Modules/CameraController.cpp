#include "CameraController.h"

#include <Omnia/System/Input.h>

namespace Ultra {

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
