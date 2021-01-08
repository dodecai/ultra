#pragma once

#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Ultra {

class DesignerCamera: public Camera {
public:
    DesignerCamera() = default;
    DesignerCamera(float fov, float aspectRatio, float nearClip, float farClip);
    virtual ~DesignerCamera() = default;

    void Update(Timestamp delta);

    inline float GetDistance() const { return mDistance; }
    inline void SetDistance(float distance) { mDistance = distance; }

    inline void SetViewportSize(float width, float height) { mViewportWidth = width; mViewportHeight = height; UpdateProjection(); }

    const glm::mat4 &GetViewMatrix() const { return mViewMatrix; }
    glm::mat4 GetViewProjection() const { return Projection * mViewMatrix; }

    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;
    const glm::vec3 &GetPosition() const { return mPosition; }
    glm::quat GetOrientation() const;

    float GetPitch() const { return mPitch; }
    float GetYaw() const { return mYaw; }

//private:
    void UpdateProjection();
    void UpdateView();

    void Pan(const glm::vec2 &delta);
    void Rotate(const glm::vec2 &delta);
    void Zoom(float delta);

    glm::vec3 CalculatePosition() const;

    std::pair<float, float> PanSpeed() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;

private:
    float mFOV = 45.0f;
    float mAspectRatio = 1.778f;
    float mNearClip = 0.1f;
    float mFarClip = 1000.0f;

    glm::mat4 mViewMatrix = {};
    glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };
    glm::vec3 mFocalPoint = { 0.0f, 0.0f, 0.0f };

    glm::vec2 mInitialMousePosition = { 0.0f, 0.0f };

    float mDistance = 10.0f;
    float mPitch = 0.0f;
    float mYaw = 0.0f;

    float mViewportWidth = 1280;
    float mViewportHeight = 720;
};

}
