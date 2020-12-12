#pragma once

#include <glm/glm.hpp>

#include "Ultra.pch"
#include "Ultra/Core.h"

namespace Ultra {

enum class CameraTypes {
    Orthographic    = 0,
    Perspective     = 1
};

class Camera {
public:
    Camera() = default;
    Camera(const glm::mat4 &projection): Projection { projection } {}
    virtual ~Camera() = default;

    const float GetExposure() const { return Exposure; }
    const glm::mat4 &GetProjection() const { return Projection; }

    void SetProjection(const glm::mat4 &projection) { Projection = projection; }

protected:
    float Exposure = 1.0f;
    glm::mat4 Projection = glm::mat4(1.0f);
};

}
