#pragma once

#include <Omnia/UI/Event.h>
#include <Omnia/Utility/Timer.h>

#include "Engine/Renderer/Camera.h"

namespace Ultra {

using namespace Omnia;

struct OrthographicCameraBounds
{
    float Left, Right;
    float Bottom, Top;

    float GetWidth() { return Right - Left; }
    float GetHeight() { return Top - Bottom; }
};

class OrthographicCameraController {
    float AspectRatio;
    bool Rotation;
    float ZoomLevel = 1.0f;
    float deltaTime = 0.0f;

    OrthographicCameraBounds Bounds;
    OrthographicCamera Camera;

    glm::vec3 CameraPosition = { 0.0f, 0.0f, 0.0f };
    float CameraRotation = 0.0f;
    
    float CameraRotationSpeed = 180.0f;
    float CameraMoveSpeed = 12.0f;
    
public:
    OrthographicCameraController(float apsectRatio, bool rotation = false);

    void Update(Timestamp deltaTime);

    void Resize(float width, float height);

    const OrthographicCameraBounds& GetBounds() const { return Bounds; }

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
