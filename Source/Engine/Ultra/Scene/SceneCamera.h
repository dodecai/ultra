#pragma once

#include <Ultra/Renderer/Camera.h>

namespace Ultra {

class SceneCamera: public CameraNew {
public:
    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetOrthographic(float size, float nearClip, float farClip);
    void SetViewportSize(uint32_t width, uint32_t height);

    float GetOrthographicSize() const { return OrthographicSize; }
    void SetOrthographicSize(float size) { OrthographicSize = size; RecalculateProjection(); }

private:
    void RecalculateProjection();

private:
    float AspectRatio = 0.0f;

    float OrthographicSize = 10.0f;
    float OrthographicNear = -1.0f;
    float OrthographicFar = 1.0f;
};

}
