export module Ultra.Renderer.Camera;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math;

export namespace Ultra {

class Camera {
public:
    Camera() = default;
    Camera(const glm::mat4 &projection): mProjection(projection) {}
    virtual ~Camera() = default;

    // Accessors
    const float GetExposure() const { return mExposure; }
    const glm::mat4 &GetProjection() const { return mProjection; }

    // Mutators
    void SetProjection(const glm::mat4 &projection) { mProjection = projection; }

protected:
    float mExposure = 1.0f;
    glm::mat4 mProjection = glm::mat4(1.0f);
};

}
