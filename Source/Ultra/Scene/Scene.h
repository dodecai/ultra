#pragma once

#include <entt/entt.hpp>

#include "Ultra.pch"
#include "Core.h"

#include "Components.h"
#include "Engine/Renderer/Camera.h"

namespace Ultra {

class Entity;

class Scene {
    friend class Entity;
    entt::registry Registry;


protected:
    //bool ActiveScene = false;
    //Scope<Camera> mCamera;


public:
    Scene();
    ~Scene();
    //explicit Scene(Scope<Camera> &&camera): pCamera(std::move(camera)) {}

    //bool Active() const {return pScene ? pScene->Active() : false;};
    void Update(Timestamp deltaTime);

    Entity CreateEntitiy(const string &name = "");

    // Accessors
    //Camera *GetCamera() const { return Camera.get(); }

    // Mutators
    //void SetCamera(Camera *camera) { pCamera.reset(camera); }
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


