#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Core.h"

namespace Ultra {

SceneCamera::SceneCamera() {
    RecalculateProjection();
}

void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
    OrthographicSize = size;
    OrthographicNear = nearClip;
    OrthographicFar = farClip;

    RecalculateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    AspectRatio = (float)width / (float)height;

    RecalculateProjection();
}

void SceneCamera::RecalculateProjection() {
    float left = -OrthographicSize * AspectRatio * 0.5f;
    float right = OrthographicSize * AspectRatio * 0.5f;
    float bottom = -OrthographicSize * 0.5f;
    float top = OrthographicSize * 0.5f;

    Projection = glm::ortho(left, right, bottom, top, OrthographicNear, OrthographicFar);
}

}
