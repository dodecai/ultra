export module Ultra.Renderer.PerspectiveCamera;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math;

#pragma warning(push)
#pragma warning(disable: 4201)

export namespace Ultra {

enum class CameraDirection {
    Left,       // X-Axis--
    Right,      // X-Axis++

    Down,       // Y-Axis--
    Up,         // Y-Axis++

    Backward,   // Z-Axis++
    Forward,    // Z-Axis--
};

class PerspectiveCamera {
public:
    PerspectiveCamera() = default;
    PerspectiveCamera(glm::mat4 projection): mProjectionMatrix(projection) {}
    PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip) {
        mProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        mPitch = 90.0f;
        mViewMatrix = glm::mat4 { 1.0f };
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }
    ~PerspectiveCamera() = default;

     // Accessors
    const glm::vec3 GetDirection(CameraDirection direction) const {
        switch (direction) {
            // X-Axis
            case CameraDirection::Left:
            case CameraDirection::Right: {
                return glm::rotate(glm::quat(glm::vec3(-mPitch, -mYaw, -mRoll)), glm::vec3(1.0f, 0.0f, 0.0f));
            }

            // Y-Axis
            case CameraDirection::Down:
            case CameraDirection::Up: {
                return glm::rotate(glm::quat(glm::vec3(-mPitch, -mYaw, -mRoll)), glm::vec3(0.0f, 1.0f, 0.0f));
            }

            // Z-Axis
            case CameraDirection::Backward:
            case CameraDirection::Forward: {
                return glm::rotate(glm::quat(glm::vec3(-mPitch, -mYaw, -mRoll)), glm::vec3(0.0f, 0.0f, 1.0f));
            }

            default: {
                return glm::vec3{ 0.0f };
            }
        }
    }
    const glm::vec3 &GetPosition() const { return mPosition; }
    const glm::mat4 &GetProjectionMatrix() const { return mProjectionMatrix; }
    const glm::mat4 &GetViewMatrix() const { return mViewMatrix; }
    const glm::mat4 &GetViewProjectionMatrix() const { return mViewProjectionMatrix; }

    // Mutators
    void SetPosition(const glm::vec3 &position) {
        mPosition = position;
        RecalculateViewMatrix();
    }
    void SetProjection(float fov, float aspectRatio, float nearClip, float farClip) {
        mProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }
    // Pitch around X-axis
    void SetPitch(float rotation) {
        mPitch = rotation;
        RecalculateViewMatrix();
    }
    // Roll around Z-axis
    void SetRoll(float rotation) {
        mRoll = rotation;
        RecalculateViewMatrix();
    }
    // Yaw around Y-axis
    void SetYaw(float rotation) {
        mYaw = rotation;
        RecalculateViewMatrix();
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void RecalculateViewMatrix() {
        auto transform = glm::translate(glm::mat4(1.0f), mPosition) *
            glm::rotate(glm::mat4(1.0f), glm::radians(mRoll), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(mPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(mYaw), glm::vec3(0.0f, 1.0f, 0.0f));

        mViewMatrix = glm::inverse(transform);
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }

private:
    glm::vec3 mPosition = {};
    float mRoll = 0.0f;
    float mPitch = 0.0f;
    float mYaw = 0.0f;

    glm::mat4 mProjectionMatrix = {};
    glm::mat4 mViewMatrix = {};
    glm::mat4 mViewProjectionMatrix = {};
};

}

#pragma warning(pop)
