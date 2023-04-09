module;

#include <glm/glm.hpp>

export module Ultra.Engine.Camera;

import Ultra.Core;
import Ultra.Logger;

export namespace Ultra {

enum class CameraTypes {
    Orthographic    = 0,
    Perspective     = 1
};

class Camera {
public:
    Camera() = default;
    Camera(const glm::mat4 &projection): mProjection(projection) {}
    virtual ~Camera() = default;

    const float GetExposure() const { return mExposure; }
    const glm::mat4 &GetProjection() const { return mProjection; }

    void SetProjection(const glm::mat4 &projection) { mProjection = projection; }

protected:
    float mExposure = 1.0f;
    glm::mat4 mProjection = glm::mat4(1.0f);
};

}
