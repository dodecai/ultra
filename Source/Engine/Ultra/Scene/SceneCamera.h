#pragma once

#include <Ultra/Renderer/Camera.h>

namespace Ultra {

class SceneCamera: public Camera {
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


// ToDo: CleanUp old stuff
struct CameraBounds {
    float Left, Right;
    float Bottom, Top;

    float GetWidth() { return Right - Left; }
    float GetHeight() { return Top - Bottom; }
};

enum class CameraDirection {
    Left,       // X-Axis--
    Right,      // X-Axis++

    Down,       // Y-Axis--
    Up,         // Y-Axis++

    Backward,   // Z-Axis++
    Forward,    // Z-Axis--
};

enum class CameraMovement {
    // 2D
    Left,
    Up,
    Right,
    Down,
    Zoom,

    // >2.5D
    Sphere,

    // 3D
    Backward,
    Forward,
};

class CameraController;

// ToDo: All-In-One camera class vs specific class
class PerspectiveCamera {
    friend CameraController;

    // Properties
    CameraTypes Type = CameraTypes::Orthographic;

    glm::mat4 ProjectionMatrix = {};
    glm::mat4 ViewMatrix = {};
    glm::mat4 ViewProjectionMatrix = {};

    glm::vec3 Position = {};
    glm::vec3 Forward = {};
    glm::vec3 Up = {};
    glm::vec3 Right = {};
    glm::vec3 World = {};

    float Roll	= 0.0f;		// Roll	 ... around z-axis
    float Pitch = 0.0f;		// Pitch ... around x-axis
    float Yaw	= 0.0f;		// Yaw	 ... around y-Axis

                            // Calculates the front vector from the Camera's (updated) Euler Angles
    void CalculateViewMatrix();

public:
    // Constructors
    PerspectiveCamera(glm::mat4 projection);
    PerspectiveCamera(float left, float right, float buttom, float top);

    // Accessors
    const glm::vec3 GetDirection(CameraDirection direction) const;
    const glm::vec3 &GetPosition() const;

    const glm::mat4 &GetProjectionMatrix() const { return ProjectionMatrix; }
    const glm::mat4 &GetViewMatrix() const { return ViewMatrix; }
    const glm::mat4 &GetViewProjectionMatrix() const { return ViewProjectionMatrix; }

    // Mutators
    void SetPosition(const glm::vec3 &position) { Position = position; CalculateViewMatrix(); }
    void SetProjection(float left, float right, float buttom, float top);

    void SetRoll(float rotation) { Roll = rotation; }
    void SetPitch(float rotation) { Pitch = rotation; }
    void SetYaw(float rotation) { Yaw = rotation; }
};

class OrthographicCamera {
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ViewProjectionMatrix;

    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
    float Rotation = 0.0f;

public:
    OrthographicCamera(float left, float right, float buttom, float top);

    const glm::mat4 &GetProjectionMatrix() const { return ProjectionMatrix; }
    const glm::mat4 &GetViewMatrix() const { return ViewMatrix; }
    const glm::mat4 &GetViewProjectionMatrix() const { return ViewProjectionMatrix; }

    const glm::vec3 &GetPosition() const { return Position; }
    const float &GetRotation() const { return Rotation; }

    void SetProjection(float left, float right, float buttom, float top);
    void SetPosition(const glm::vec3 &position) { Position = position; CalculateViewMatrix(); }
    void SetRotation(const float rotation) { Rotation = rotation; CalculateViewMatrix(); }

private:
    void CalculateViewMatrix();
};

}
