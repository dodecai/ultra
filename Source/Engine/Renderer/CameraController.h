#pragma once

#include "Camera.h"
#include <Omnia/Utility/Timer.h>

#include <Omnia/UI/Event.h>

namespace Ultra {

using namespace Omnia;

class OrthographicCameraController {
    float AspectRatio;
    bool Rotation;
    float ZoomLevel = 1.0f;
    float deltaTime = 0.0f;

    OrthographicCamera Camera;

    glm::vec3 CameraPosition = { 0.0f, 0.0f, 0.0f };
    float CameraRotation = 0.0f;
    
    float CameraRotationSpeed = 180.0f;
    float CameraMoveSpeed = 12.0f;
    
public:
    OrthographicCameraController(float apsectRatio, bool rotation = false);

    void Update(Timestamp deltaTime);

    void Resize(float width, float height);

    OrthographicCamera &GetCamera() { return Camera; }
    const OrthographicCamera &GetCamera() const { return Camera; }

    const float GetZoomLevel() const { return ZoomLevel; }
    void SetZoomLevel(float level) { ZoomLevel = level; }


    void Event(void *event);
    void KeyboardEvent(KeyboardEventData &data);
    void MouseEvent(MouseEventData &data);
};

}
