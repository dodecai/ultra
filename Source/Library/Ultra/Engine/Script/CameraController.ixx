export module Ultra.Engine.Script.CameraController;

import Ultra.Core;
import Ultra.System.Input;
import Ultra.Utility.Timer;

import Ultra.Engine.Entity;

export namespace Ultra {

class CameraSystem: public ScriptableEntity {
public:
    CameraSystem() = default;
    virtual ~CameraSystem() = default;

    void Create() override {
        //auto &transform = GetComponent<Component::Transform>();
        //transform.Position.x = rand() % 10 - 5.0f;
    }
    void Destroy() override {}

    void Update([[maybe_unused]] Timestamp deltaTime) override {
        //auto &transform = GetComponent<Component::Transform>();
        //float speed = 12.0f;

        //if (Input::GetKeyState(KeyCode::KeyA)) { transform.Position.x -= speed * deltaTime; }
        //if (Input::GetKeyState(KeyCode::KeyD)) { transform.Position.x += speed * deltaTime; }
        //if (Input::GetKeyState(KeyCode::KeyW)) { transform.Position.y += speed * deltaTime; }
        //if (Input::GetKeyState(KeyCode::KeyS)) { transform.Position.y -= speed * deltaTime; }
    }
};

}

#ifdef THIS_WAS_ANOTHER_ATTEMPT

class CameraController {
public:
    CameraController(float apsectRatio, bool rotation = false):
        AspectRatio { apsectRatio },
        Bounds { -AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel },
        Camera { Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top } {
        ZoomLevel -= 1.0f * 0.25f;
        ZoomLevel = std::max(ZoomLevel, 0.25f);
        Camera.SetProjection(Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top);
    }
    CameraController(CameraController &right) = default;

    void Update(Timestamp deltaTime) {
    // Left or Right
        if (Input::GetKeyState(KeyCode::KeyA)) {
            CameraPosition.x -= CameraMoveSpeed * deltaTime;
        } else if (Input::GetKeyState(KeyCode::KeyD)) {
            CameraPosition.x += CameraMoveSpeed * deltaTime;
        }

        // Up or Down
        if (Input::GetKeyState(KeyCode::KeyW)) {
            CameraPosition.y += CameraMoveSpeed * deltaTime;
        } else if (Input::GetKeyState(KeyCode::KeyS)) {
            CameraPosition.y -= CameraMoveSpeed * deltaTime;
        }

        //// Backward or Forward
        //if (Input::GetKeyState(KeyCode::KeyY)) {
        //	CameraPosition.z += CameraMoveSpeed * deltaTime;
        //} else if (Input::GetKeyState(KeyCode::KeyX)) {
        //	CameraPosition.z -= CameraMoveSpeed * deltaTime;
        //}

        // Rotation
        if (Input::GetKeyState(KeyCode::KeyQ)) {
            CameraRotation -= CameraRotationSpeed * deltaTime;
        } else if (Input::GetKeyState(KeyCode::KeyE)) {
            CameraRotation += CameraRotationSpeed * deltaTime;
        }
        Camera.SetRoll(CameraRotation);

        Camera.SetPosition(CameraPosition);
        CameraMoveSpeed = ZoomLevel;
    }
    void Resize(float width, float height) {
        AspectRatio = width / height;
        CalculateView();
    }

    Ultra::PerspectiveCamera &GetCamera() { return Camera; }
    const Ultra::PerspectiveCamera &GetCamera() const { return Camera; }

    const CameraBounds &GetBounds() const { return Bounds; }
    float GetZoomLevel() const { return ZoomLevel; };

    void SetZoomLevel(float level) { ZoomLevel = level; CalculateView(); }

    void Event(void *event) {}
    void KeyboardEvent(KeyboardEventData &data) {}
    void MouseEvent(MouseEventData &data) {
        ZoomLevel -= data.DeltaWheelY * 0.32f;
        ZoomLevel = std::max(ZoomLevel, 0.12f);
        CalculateView();
    }

private:
    void CalculateView() {
        Bounds = { -AspectRatio * ZoomLevel, AspectRatio * ZoomLevel, -ZoomLevel, ZoomLevel };
        Camera.SetProjection(Bounds.Left, Bounds.Right, Bounds.Bottom, Bounds.Top);
    }

    // Camera Movement: Processes input received from any input system.
    bool Move(CameraMovement direction, float delta) {
        float velocity = MovementSpeed * delta;
        float currentSpeed = Acceleration;
        if (currentSpeed < MovementSpeed) currentSpeed += Acceleration;

        switch (direction) {
            case CameraMovement::Backward:	Camera.Position -= Camera.Forward * velocity;	break;
            case CameraMovement::Down:		Camera.Position += Camera.Up * velocity;		break;
            case CameraMovement::Forward:	Camera.Position += Camera.Forward * velocity;	break;
            case CameraMovement::Left:		Camera.Position -= Camera.Right * velocity;	break;
            case CameraMovement::Right:	Camera.Position += Camera.Right * velocity;	break;
            case CameraMovement::Up:		Camera.Position -= Camera.Up * velocity;		break;
            default:					return false;					break;
        }

        // Stick at Ground Level - make sure the user stays at the ground level
        //Position.y = 0.0f; // <-- this one-liner keeps the user at the ground level (xz plane)

        return true;
    }

    // Camera Movement: Processes input received from an mouse input system. Expects the offset value in both x and y directions.
    bool Move(CameraMovement direction, float xoffset, float yoffset, bool constrainPitch = true) {
        switch (direction) {
            case CameraMovement::Sphere:
                xoffset *= MouseSensitivity;
                yoffset *= MouseSensitivity;

                Camera.Yaw += xoffset;
                Camera.Pitch += yoffset;

                // Make sure that when pitch is out of bounds, screen doesn't get flipped
                if (constrainPitch) {
                    if (Camera.Pitch > 89.0f)
                        Camera.Pitch = 89.0f;
                    if (Camera.Pitch < -89.0f)
                        Camera.Pitch = -89.0f;
                }

                // Update Front, Right and Up Vectors using the updated Euler angles
                Camera.CalculateViewMatrix();
                break;

            case CameraMovement::Zoom:
                // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis

                if (ZoomLevel >= 1.0f && ZoomLevel <= 45.0f)
                    ZoomLevel -= yoffset;
                if (ZoomLevel <= 1.0f)
                    ZoomLevel = 1.0f;
                if (ZoomLevel >= 45.0f)
                    ZoomLevel = 45.0f;
                break;

            default:
                return false;
                break;
        }

        return true;
    }

private:
    float AspectRatio;
    glm::vec3 CameraPosition = { 0.0f, 0.0f, 0.0f };
    float CameraRotation = 0.0f;
    float ZoomLevel = 1.0f;

    CameraBounds Bounds;
    PerspectiveCamera Camera;

    float CameraAcceleration = 0.1f;
    float CameraSensitivity = 0.1f;
    float CameraSpeed = 2.5f;
    float CameraZoomSpeed = 2.5f;

    float CameraRotationSpeed = 180.0f;
    float CameraMoveSpeed = 12.0f;

    // States
    float Acceleration;
    float MovementSpeed;
    float MouseSensitivity;

};

#endif
