#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Ultra/Core.h"

namespace Ultra {

SceneCamera::SceneCamera() {
    RecalculateProjection();
}

void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
    Type = ProjectionType::Orthographic;

    OrthographicSize = size;
    OrthographicNear = nearClip;
    OrthographicFar = farClip;

    RecalculateProjection();
}

void SceneCamera::SetPerspective(float verticalFOV, float near, float far) {
    Type = ProjectionType::Perspective;

    PerspectiveFOV = verticalFOV;
    PerspectiveNear = near;
    PerspectiveFar = far;

    RecalculateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    AspectRatio = (float)width / (float)height;

    RecalculateProjection();
}

float SceneCamera::GetPerspectiveVerticalFOV() {
    return PerspectiveFOV;
}

void SceneCamera::SetPerspectiveVerticalFOV(float size) {
    PerspectiveFOV = size;
    RecalculateProjection();
}


float SceneCamera::GetFarClip() {
    switch (Type) {
        case ProjectionType::Perspective: {
            return PerspectiveFar;
        }

        case ProjectionType::Orthographic: {
            return OrthographicFar;
        }

        default: {
            return 0.0f;
        }
    }
}

float SceneCamera::GetNearClip() {
    switch (Type) {
        case ProjectionType::Perspective: {
            return PerspectiveNear;
        }

        case ProjectionType::Orthographic: {
            return OrthographicNear;
        }

        default: {
            return 0.0f;
        }
    }
}

void SceneCamera::SetFarClip(float value) {
    switch (Type) {
        case ProjectionType::Perspective: {
            PerspectiveFar = value;
            break;
        }

        case ProjectionType::Orthographic: {
            OrthographicFar = value;
            break;
        }

        default: {
            break;
        }
    }
    RecalculateProjection();
}

void SceneCamera::SetNearClip(float value) {
    switch (Type) {
        case ProjectionType::Perspective: {
            PerspectiveNear = value;
            break;
        }

        case ProjectionType::Orthographic: {
            OrthographicNear = value;
            break;
        }

        default: {
            break;
        }
    }
    RecalculateProjection();
}


SceneCamera::ProjectionType SceneCamera::GetProjectionType() {
    return Type;
}

void SceneCamera::SetProjectionType(ProjectionType type) {
    Type = type;
}


void SceneCamera::RecalculateProjection() {
    switch (Type) {
        case ProjectionType::Perspective: {
            Projection = glm::perspective(PerspectiveFOV, AspectRatio, PerspectiveNear, PerspectiveFar);
            break;
        }

        case ProjectionType::Orthographic: {
            float left = -OrthographicSize * AspectRatio * 0.5f;
            float right = OrthographicSize * AspectRatio * 0.5f;
            float bottom = -OrthographicSize * 0.5f;
            float top = OrthographicSize * 0.5f;

            Projection = glm::ortho(left, right, bottom, top, OrthographicNear, OrthographicFar);
            break;
        }

        default: {
            break;
        }
    }
}

}
