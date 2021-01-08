#include "DesignerCamera.h"

#include "Omnia/System/Input.h"

namespace Ultra {

DesignerCamera::DesignerCamera(float fov, float aspectRatio, float nearClip, float farClip):
    mFOV(fov),
    mAspectRatio(aspectRatio),
    mNearClip(nearClip),
    mFarClip(farClip),
    Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {
    UpdateView();
}

void DesignerCamera::UpdateProjection() {
    mAspectRatio = mViewportWidth / mViewportHeight;
    Projection = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearClip, mFarClip);
}

void DesignerCamera::UpdateView() {
    // Lock the camera's rotation
    // mYaw = mPitch = 0.0f;
    mPosition = CalculatePosition();

    glm::quat orientation = GetOrientation();
    mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation);
    mViewMatrix = glm::inverse(mViewMatrix);
}

std::pair<float, float> DesignerCamera::PanSpeed() const {
    float x = std::min(mViewportWidth / 1000.0f, 2.4f); // max 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(mViewportHeight / 1000.0f, 2.4f); // max 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
    return {xFactor, yFactor};
}

float DesignerCamera::RotationSpeed() const {
    return 0.8f;
}

float DesignerCamera::ZoomSpeed() const {
    float distance = mDistance * 0.2f;
    distance = std::max(distance, 0.0f);
    float speed = distance * distance;
    speed = std::min (speed, 100.0f);
    return speed;
}

void DesignerCamera::Update(Timestamp delta) {
    if (Input::GetKeyState(KeyCode::LAlt)) {
        auto [x, y] = Input::GetMousePosition();
        const glm::vec2 &mouse { x, y };
        glm::vec2 delta = (mouse - mInitialMousePosition) * 0.003f;
        mInitialMousePosition = mouse;

        if (Input::GetMouseButtonState(MouseButton::Middle)) Pan(delta);
        if (Input::GetMouseButtonState(MouseButton::Left)) Rotate(delta);
        if (Input::GetMouseButtonState(MouseButton::Right)) Zoom(delta.y);

        // ToDo: On Mouse Scroll!
    } else {
        // ToDo: Works everywhere, but eats the performance...
        auto [x, y] = Input::GetMousePosition();
        const glm::vec2 &mouse { x, y };
        mInitialMousePosition = mouse;
    }
    UpdateView();
}

void DesignerCamera::Pan(const glm::vec2 &delta) {
    auto [xSpeed, ySpeed] = PanSpeed();
    mFocalPoint += -GetRightDirection() * delta.x * xSpeed * mDistance;
    mFocalPoint += GetUpDirection() * delta.y * ySpeed * mDistance;
}

void DesignerCamera::Rotate(const glm::vec2 &delta) {
    float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    mYaw += yawSign * delta.x * RotationSpeed();
    mPitch += delta.y * RotationSpeed();
}

void DesignerCamera::Zoom(float delta) {
    mDistance -= delta * ZoomSpeed();

    if (mDistance < 1.0f) {
        mFocalPoint += GetForwardDirection();
        mDistance = 1.0f;
    }
}

glm::vec3 DesignerCamera::GetUpDirection() const {
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 DesignerCamera::GetRightDirection() const {
    return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 DesignerCamera::GetForwardDirection() const {
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 DesignerCamera::CalculatePosition() const {
    return mFocalPoint - GetForwardDirection() * mDistance;
}

glm::quat DesignerCamera::GetOrientation() const {
    return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
}

}

