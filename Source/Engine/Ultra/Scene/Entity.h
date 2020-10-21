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
        if (HasComponent<T>()) {
            AppLogWarning("Entity already has component!");
            return GetComponent<T>();
        }
        T &component = pScene->Registry.emplace<T>(EntityHandle, arguments...);
        return component;
    }

    template<typename T>
    T &GetComponent() {
        if (!HasComponent<T>()) {
            AppLogWarning("The component doesn't exist under entity!");
        }
        return pScene->Registry.get<T>(EntityHandle);
    }
    
    template<typename T>
    void RemoveComponent() {
        if (HasComponent<T>()) pScene->Registry.remove<T>(EntityHandle);
    }

    template<typename T>
    void RemoveAllComponent() {
        pScene->Registry.remove_all(T)(EntityHandle);
    }

    operator bool() const { return EntityHandle != entt::null; }
    operator uint32_t() const { return (uint32_t)EntityHandle; }

    bool operator==(const Entity &other) const {
        return EntityHandle == other.EntityHandle && pScene == other.pScene;
    }
    bool operator!=(const Entity &other) const {
        return !(*this==other);
    }

    operator entt::entity() const { return EntityHandle; }

    Scene *GetScene() const { return pScene; }

private:
    Scene *pScene = nullptr;
    entt::entity EntityHandle = entt::null;
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
