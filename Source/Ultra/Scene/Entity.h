#pragma once

#pragma warning(push, 0) 
#include <entt/entt.hpp>
#pragma warning(pop)

#include "Scene.h"

namespace Ultra {

class Entity {
public:
    Entity() = default;
    Entity(entt::entity handle, Scene *scene): EntityHandle(handle), pScene(scene) {};
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

private:
    entt::entity EntityHandle = entt::null;
    Scene *pScene = nullptr;
};

class ScriptableEntity {
public:
    ScriptableEntity() = default;
    virtual ~ScriptableEntity() = default;

    template<typename T>
    T &GetComponent() {
        return mEntity.GetComponent<T>();
    }

protected:
    virtual void Create() {}
    virtual void Destroy() {}
    virtual void Update(Timestamp deltaTime) {}

private:
    Entity mEntity;
    friend class Scene;
};

}
