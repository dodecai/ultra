#pragma once

#include <entt/entt.hpp>

#include "Ultra.pch"
#include "Ultra/Core.h"
#include "Ultra/Scene/Entity.h"

#include <glm/glm.hpp>

namespace Ultra {

class CameraSystem: public ScriptableEntity {
public:
    CameraSystem() = default;
    virtual ~CameraSystem() = default;

    void Create() override {
        //auto &transform = GetComponent<Component::Transform>();
        //transform.Position.x = rand() % 10 - 5.0f;
    }
    void Destroy() override {}

    void Update(Timestamp deltaTime) override {
        auto &transform = GetComponent<Component::Transform>();
        float speed = 12.0f;

        if (Input::GetKeyState(KeyCode::KeyA)) { transform.Position.x -= speed * deltaTime; }
        if (Input::GetKeyState(KeyCode::KeyD)) { transform.Position.x += speed * deltaTime; }
        if (Input::GetKeyState(KeyCode::KeyW)) { transform.Position.y += speed * deltaTime; }
        if (Input::GetKeyState(KeyCode::KeyS)) { transform.Position.y -= speed * deltaTime; }
    }
};

}
