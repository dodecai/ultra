#pragma once

#include <Ultra/Renderer/Camera.h>

namespace Ultra {

class SceneCamera: public CameraNew {
public:
    enum class ProjectionType {
        Perspective     = 0,
        Orthographic    = 1
    };

public:
    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetOrthographic(float size, float nearClip, float farClip);
    void SetPerspective(float verticalFOV, float near, float far);

    void SetViewportSize(uint32_t width, uint32_t height);

    float GetOrthographicSize() const { return OrthographicSize; }
    void SetOrthographicSize(float size) { OrthographicSize = size; RecalculateProjection(); }

    float GetPerspectiveVerticalFOV();
    void SetPerspectiveVerticalFOV(float size);

    float GetFarClip();
    float GetNearClip();
    void SetFarClip(float value);
    void SetNearClip(float value);

    ProjectionType GetProjectionType();
    void SetProjectionType(ProjectionType type);

private:
    void RecalculateProjection();

private:
    ProjectionType Type = ProjectionType::Orthographic;
    float AspectRatio = 0.0f;

    float OrthographicSize = 10.0f;
    float OrthographicNear = -32.0f;
    float OrthographicFar = 32.0f;

    float PerspectiveFOV = glm::radians(45.0f);
    float PerspectiveNear = 0.01f;
    float PerspectiveFar = 1024.0f;
};

}
