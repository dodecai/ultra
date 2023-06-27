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
