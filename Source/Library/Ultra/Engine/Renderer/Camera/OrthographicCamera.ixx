module;

export module Ultra.Engine.OrthographicCamera;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math;

export namespace Ultra {

class OrthographicCamera {
public:
    // Default
    OrthographicCamera() = default;
    OrthographicCamera(float left, float right, float bottom, float top):
        mProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
        mViewMatrix(1.0f) {
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }
    ~OrthographicCamera() = default;

    // Accessors
    const glm::vec3 &GetPosition() const { return mPosition; }
    const glm::mat4 &GetProjectionMatrix() const { return mProjectionMatrix; }
    const glm::vec3 &GetRotation() const { return mRotation; }
    const glm::mat4 &GetViewMatrix() const { return mViewMatrix; }
    const glm::mat4 &GetViewProjectionMatrix() const { return mViewProjectionMatrix; }

    // Mutators
    void SetPosition(const glm::vec3 &position) {
        mPosition = position;
        RecalculateViewMatrix();
    }
    void SetProjection(float left, float right, float bottom, float top) {
        mProjectionMatrix = { glm::ortho(left, right, bottom, top, -1.0f, 1.0f) };
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
        RecalculateViewMatrix();
    }
    void SetRotation(const glm::vec3 &rotation) {
        mRotation = rotation;
        RecalculateViewMatrix();
    }

private:
    void RecalculateViewMatrix() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition) *
            glm::rotate(glm::mat4(1.0f), mRotation.x, glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), mRotation.y, glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), mRotation.z, glm::vec3(0, 0, 1));
        mViewMatrix = glm::inverse(transform);
        mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
    }

private:
    glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };
    glm::vec3 mRotation = { 0.0f, 0.0f, 0.0f };

    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;
    glm::mat4 mViewProjectionMatrix;
};

}
