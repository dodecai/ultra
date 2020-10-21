#pragma once

// ToDo: Diable debugging on third party libraries by default...
#pragma warning(push, 0) 
#define ENTT_ASSERT(condition)
#include <entt/entt.hpp>
#pragma warning(pop)

#include "Ultra.pch"
#include "Ultra/Core.h"

#include "Components.h"
#include "SceneCamera.h"

namespace Ultra {

class Entity;

class Scene {
    friend class Entity;
    friend class SceneBrowser;
    friend class Serializer;
    friend class PropertiesPanel;

public:
    // Accessors
    Scene(const string &caption = "Scene");
    //explicit Scene(Scope<Camera> &&camera): pCamera(std::move(camera)) {}
    ~Scene();

    Entity CreateEntity(const string &name = "Entity");
    Entity CreateEntity(const UUID<string> &id);
    void DestroyEntity(Entity entity);
    
    void Clear();
    void Update(Timestamp deltaTime);
    void Resize(uint32_t width, uint32_t height);

    // Accessors
    const Camera &GetCamera();
    const string &GetCaption() const;
    const Entity &GetEntity(const string &name);
    const Entity &GetEntity(const UUID<string> &id);
    const uint32_t GetHeight() const { return mHeight; }
    const uint32_t GetWdith() const { return mWidth; }
    bool IsActive() const {return mActive;};

private:
    // Mutators
    void SetCaption(const string &caption);

private:
    //SceneCamera *pCamera = nullptr;
    entt::registry Registry;
    vector<Entity *> mEntities;

    std::string mCaption;
    bool mActive = false;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
};

// ToDo: From Prototype something like a scene library
//class Scenes {
//	std::unique_ptr<Scene> pScene;
//
//public:
//	Scenes() {}
//
//	// Accessors
//	Scene *GetScene() const { return pScene; }
//	Camera *GetCamera() const { return pScene ? pScene->GetCamera() : nullptr; }
//	bool IsActive() const { return pScene ? pScene->Active() : false; }
//
//	// Mutators
//	void SetScene(std::unique_ptr<Scene> &&scene) { pScene = std::move(scene); }
//
//	// Methods
//	void Update() override {
//		if (!pScene) return;
//
//		if(!pScene->Running) {
//			pScene->Run();
//			pScene->Running = true;
//		}
//
//		pScene->Update();
//	}
//
//};
}


