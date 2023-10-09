export module Ultra.Scene.Camera;

import Ultra.Core;
import Ultra.Math;
import Ultra.Renderer.Camera;

export namespace Ultra {

class SceneCamera: public Camera {
public:
    enum class ProjectionType {
        Perspective = 0,
        Orthographic = 1
    };

public:
    SceneCamera() { RecalculateProjection(); }
    virtual ~SceneCamera() = default;

    ProjectionType GetProjectionType() { return mType; }
    float GetFarClip() {
        switch (mType) {
            case ProjectionType::Perspective:   { return mPerspectiveFar; }
            case ProjectionType::Orthographic:  { return mOrthographicFar; }
        }
    }
    float GetNearClip() {
        switch (mType) {
            case ProjectionType::Perspective:   { return mPerspectiveNear; }
            case ProjectionType::Orthographic:  { return mOrthographicNear; }
        }
    }
    float GetOrthographicSize() const { return mOrthographicSize; }
    float GetPerspectiveVerticalFOV() { return mPerspectiveFOV;  }

    void SetProjectionType(ProjectionType type) {
        mType = type;
        RecalculateProjection();
    }
    void SetFarClip(float value) {
        switch (mType) {
            case ProjectionType::Perspective:   { mPerspectiveFar = value;   break; }
            case ProjectionType::Orthographic:  { mOrthographicFar = value;  break; }
        }
        RecalculateProjection();
    }
    void SetNearClip(float value) {
        switch (mType) {
            case ProjectionType::Perspective:   { mPerspectiveNear = value;  break; }
            case ProjectionType::Orthographic:  { mOrthographicNear = value; break; }
        }
        RecalculateProjection();
    }
    void SetOrthographic(float size, float nearClip, float farClip) {
        mType = ProjectionType::Orthographic;

        mOrthographicSize = size;
        mOrthographicNear = nearClip;
        mOrthographicFar = farClip;

        RecalculateProjection();
    }
    void SetOrthographicSize(float size) {
        mOrthographicSize = size;
        RecalculateProjection();
    }
    void SetPerspective(float verticalFOV, float near, float far) {
        mType = ProjectionType::Perspective;

        mPerspectiveFOV = verticalFOV;
        mPerspectiveNear = near;
        mPerspectiveFar = far;

        RecalculateProjection();
    }
    void SetPerspectiveVerticalFOV(float size) {
        mPerspectiveFOV = size;
        RecalculateProjection();
    }
    void SetViewportSize(uint32_t width, uint32_t height) {
        mAspectRatio = (float)width / (float)height;
        RecalculateProjection();
    }

private:
    void RecalculateProjection() {
        switch (mType) {
            case ProjectionType::Perspective: {
                mProjection = glm::perspective(mPerspectiveFOV, mAspectRatio, mPerspectiveNear, mPerspectiveFar);
                break;
            }

            case ProjectionType::Orthographic: {
                float left = -mOrthographicSize * mAspectRatio * 0.5f;
                float right = mOrthographicSize * mAspectRatio * 0.5f;
                float bottom = -mOrthographicSize * 0.5f;
                float top = mOrthographicSize * 0.5f;

                mProjection = glm::ortho(left, right, bottom, top, mOrthographicNear, mOrthographicFar);
                break;
            }

            default: {
                break;
            }
        }
    }

private:
    ProjectionType mType = ProjectionType::Orthographic;
    float mAspectRatio = 0.0f;

    float mOrthographicSize = 10.0f;
    float mOrthographicNear = -32.0f;
    float mOrthographicFar = 32.0f;

    float mPerspectiveFOV = glm::radians(45.0f);
    float mPerspectiveNear = 0.01f;
    float mPerspectiveFar = 1024.0f;
};

}
