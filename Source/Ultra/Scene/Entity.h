#pragma once

#include <entt/entt.hpp>
#include "Scene.h"

namespace Ultra {

class Entity {
    entt::entity EntityHandle = entt::null;
    Scene *pScene = nullptr;

public:
    Entity() = default;
    Entity(entt::entity handle, Scene *scene):
        EntityHandle(handle),
        pScene(scene) {};
    ~Entity() = default;

    Entity(const Entity &other) = default;

    template<typename T>
    bool HasComponent() {
       return pScene->Registry.has<T>(EntityHandle);
    }

    template<typename T, typename... Args>
    T &AddComponent(Args &&...arguments) {
        APP_ASSERT(!HasComponent<T>(), "Entity already has component!");

        return pScene->Registry.emplace<T>(EntityHandle, arguments...);
    }

    template<typename T>
    T &GetComponent() {
        APP_ASSERT(HasComponent<T>(), "The component doesn't exist under entity!");

        return pScene->Registry.get<T>(EntityHandle);
    }

    template<typename T>
    T &RemoveComponent() {
        APP_ASSERT(!HasComponent<T>(), "The component doesn't exist under entity!");

        return pScene->Registry.remove<T>(EntityHandle);
    }

    operator bool() const { return EntityHandle != entt::null; }
};

}
