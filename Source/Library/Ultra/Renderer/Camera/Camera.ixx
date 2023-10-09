export module Ultra.Renderer.Camera;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math;

export namespace Ultra {

class Camera {
public:
    Camera() = default;
    Camera(const Matrix4 &projection): mProjection(projection) {}
    virtual ~Camera() = default;

    // Accessors
    const float GetExposure() const { return mExposure; }
    const Matrix4 &GetProjection() const { return mProjection; }

    // Mutators
    void SetProjection(const Matrix4 &projection) { mProjection = projection; }

protected:
    float mExposure = 1.0f;
    Matrix4 mProjection = Matrix4(1.0f);
};

}
