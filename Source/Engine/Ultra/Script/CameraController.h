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

    void Create() override {}
    void Destroy() override {}

    void Update(Timestamp deltaTime) override {
        auto &transform = GetComponent<Component::Transform>().mTransform;
        float speed = 12.0f;

        if (Input::GetKeyState(KeyCode::KeyA)) { transform[3].x -= speed * deltaTime; }
        if (Input::GetKeyState(KeyCode::KeyD)) { transform[3].x += speed * deltaTime; }
        if (Input::GetKeyState(KeyCode::KeyW)) { transform[3].y += speed * deltaTime; }
        if (Input::GetKeyState(KeyCode::KeyS)) { transform[3].y -= speed * deltaTime; }
    }
};

}
