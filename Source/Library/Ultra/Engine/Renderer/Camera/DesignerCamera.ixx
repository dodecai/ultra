module;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

export module Ultra.Engine.DesignerCamera;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Engine.Camera;
import Ultra.System.Input;
import Ultra.Utility.Timer;

export namespace Ultra {

class DesignerCamera: public Camera {
public:
    DesignerCamera() = default;
    DesignerCamera(float fov, float aspectRatio, float nearClip, float farClip):
        mFOV(fov),
        mAspectRatio(aspectRatio),
        mNearClip(nearClip),
        mFarClip(farClip),
        Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {
        UpdateView();
    }
    ~DesignerCamera() = default;

    void Update(Timestamp delta) {
        if (Input::GetMouseButtonState(MouseButton::Wheel)) {

        }
        if (Input::GetKeyState(KeyCode::LAlt)) {
            auto [x, y] = Input::GetMousePosition();
            const glm::vec2 &mouse { x, y };
            glm::vec2 delta = (mouse - mInitialMousePosition) * 0.003f;
            mInitialMousePosition = mouse;

            if (Input::GetMouseButtonState(MouseButton::Middle)) Pan(delta);
            if (Input::GetMouseButtonState(MouseButton::Left)) Rotate(delta);
            if (Input::GetMouseButtonState(MouseButton::Right)) Zoom(delta.y);
        } else {
            // ToDo: Works everywhere, but eats the performance...
            auto [x, y] = Input::GetMousePosition();
            const glm::vec2 &mouse { x, y };
            mInitialMousePosition = mouse;
        }
        UpdateView();
    }

    inline float GetDistance() const { return mDistance; }
    inline void SetDistance(float distance) { mDistance = distance; }

    inline void SetViewportSize(float width, float height) { mViewportWidth = width; mViewportHeight = height; UpdateProjection(); }
    const glm::mat4 &GetViewMatrix() const { return mViewMatrix; }
    glm::mat4 GetViewProjection() const { return mProjection * mViewMatrix; }

    glm::vec3 GetUpDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    glm::vec3 GetRightDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    glm::vec3 GetForwardDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }
    glm::vec3 GetPosition() const { return mPosition; }
    glm::quat GetOrientation() const {
        return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
    }
    float GetNearPoint() const { return mNearClip; }
    float GetFarPoint() const { return mFarClip; }

    float GetPitch() const { return mPitch; }
    float GetYaw() const { return mYaw; }

private:
    void UpdateProjection() {
        mAspectRatio = mViewportWidth / mViewportHeight;
        mProjection = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearClip, mFarClip);
    }
    void UpdateView() {
        // Lock the camera's rotation
        // mYaw = mPitch = 0.0f;
        mPosition = CalculatePosition();
        mViewMatrix = glm::lookAt(mPosition, mFocalPoint, glm::vec3(0.0f, 1.0f, 0.0f));
        //glm::quat orientation = GetOrientation();
        //mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation);
        //mViewMatrix = glm::inverse(mViewMatrix);
    }

    void Pan(const glm::vec2 &delta) {
        auto [xSpeed, ySpeed] = PanSpeed();
        mFocalPoint += -GetRightDirection() * delta.x * xSpeed * mDistance;
        mFocalPoint += GetUpDirection() * delta.y * ySpeed * mDistance;
    }
    void Rotate(const glm::vec2 &delta) {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        mYaw += yawSign * delta.x * RotationSpeed();
        mPitch += delta.y * RotationSpeed();
    }
    void Zoom(float delta) {
        mDistance -= delta * ZoomSpeed();

        if (mDistance < 1.0f) {
            mFocalPoint += GetForwardDirection();
            mDistance = 1.0f;
        }
    }

    glm::vec3 CalculatePosition() const {
        return mFocalPoint - GetForwardDirection() * mDistance;
    }

    std::pair<float, float> PanSpeed() const {
        float x = std::min(mViewportWidth / 1000.0f, 2.4f); // max 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(mViewportHeight / 1000.0f, 2.4f); // max 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
        return { xFactor, yFactor };
    }

    float RotationSpeed() const {
        return 0.8f;
    }
    float ZoomSpeed() const {
        float distance = mDistance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f);
        return speed;
    }

private:
    float mAspectRatio = 1.334f;
    float mFOV = 45.0f;
    float mNearClip = 0.1f;
    float mFarClip = 1000.0f;

    glm::mat4 mViewMatrix = {};
    glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };
    glm::vec3 mFocalPoint = { 0.0f, 0.0f, 0.0f };
    glm::vec2 mInitialMousePosition = { 0.0f, 0.0f };

    float mDistance = 5.0f;
    float mPitch = 0.0f;
    float mYaw = 0.0f;

    float mViewportWidth = 800;
    float mViewportHeight = 600;
};

}
