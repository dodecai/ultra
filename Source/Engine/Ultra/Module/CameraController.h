#pragma once

#include "Ultra.pch"
#include "Ultra/Core.h"

#include <Omnia/UI/Event.h>
#include <Omnia/Utility/Timer.h>

#include "Ultra/Renderer/Camera.h"

namespace Ultra {

class CameraController {
    float AspectRatio;
    glm::vec3 CameraPosition = { 0.0f, 0.0f, 0.0f };
    float CameraRotation = 0.0f;
    float ZoomLevel = 1.0f;

    CameraBounds Bounds;
    Camera Camera;

    float CameraAcceleration = 0.1f;
    float CameraSensitivity = 0.1f;
    float CameraSpeed = 2.5f;
    float CameraZoomSpeed = 2.5f;

    float CameraRotationSpeed = 180.0f;
    float CameraMoveSpeed = 12.0f;

    // States
    float Acceleration;
    float MovementSpeed;
    float MouseSensitivity;

    // Camera Movement: Processes input received from any input system.
    bool Move(CameraMovement direction, float delta);

    // Camera Movement: Processes input received from an mouse input system. Expects the offset value in both x and y directions.
    bool Move(CameraMovement direction, float xoffset, float yoffset, bool constrainPitch = true);

public:
    CameraController(float apsectRatio, bool rotation = false);

    void Update(Timestamp deltaTime);
    void Resize(float width, float height);

    Ultra::Camera &GetCamera() { return Camera; }
    const Ultra::Camera &GetCamera() const { return Camera; }

    const CameraBounds& GetBounds() const { return Bounds; }
    float GetZoomLevel() const { return ZoomLevel; };

    void SetZoomLevel(float level) { ZoomLevel = level; CalculateView(); }

    void Event(void *event);
    void KeyboardEvent(KeyboardEventData &data);
    void MouseEvent(MouseEventData &data);

private:
    void CalculateView();
};






class OrthographicCameraController {
    float AspectRatio;
    bool Rotation;
    float ZoomLevel = 1.0f;
    float deltaTime = 0.0f;

    CameraBounds Bounds;
    OrthographicCamera Camera;

    glm::vec3 CameraPosition = { 0.0f, 0.0f, 0.0f };
    float CameraRotation = 0.0f;
    
    float CameraRotationSpeed = 180.0f;
    float CameraMoveSpeed = 12.0f;
    
public:
    OrthographicCameraController(float apsectRatio, bool rotation = false);

    void Update(Timestamp deltaTime);

    void Resize(float width, float height);

    const CameraBounds& GetBounds() const { return Bounds; }

    OrthographicCamera &GetCamera() { return Camera; }
    const OrthographicCamera &GetCamera() const { return Camera; }

    const float GetZoomLevel() const { return ZoomLevel; }
    void SetZoomLevel(float level) { ZoomLevel = level; CalculateView(); }


    void Event(void *event);
    void KeyboardEvent(KeyboardEventData &data);
    void MouseEvent(MouseEventData &data);

private:
    void CalculateView();
};

}
