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
    Scene(const string &name = "Scene");
    //explicit Scene(Scope<Camera> &&camera): pCamera(std::move(camera)) {}
    ~Scene();

    Entity CreateEntity(const string &name = "");
    Entity CreateEntity(const uint64_t id);
    
    void Update(Timestamp deltaTime);
    void Resize(uint32_t width, uint32_t height);

    // Accessors
    //Camera &GetCamera() const { return mCamera; }
    bool IsActive() const {return mActive;};

    // Mutators
    void AttachEntity(Entity *entity);
    void DetachEntity(Entity *entity);
    //void SetCamera(const Camera &camera) { mCamera = camera; }

private:
    entt::registry Registry;

    bool mActive = false;
    float mEnvironmentBlur = 1.0f;
    std::string mName;

    vector<Entity *> mEntities;

    uint32_t Width = 0;
    uint32_t Height = 0;
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


