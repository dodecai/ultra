#pragma once

#include <memory>

class Camera;
class World;

namespace core {

class Scene {
	friend Scenes;

protected:
	bool Running = false;
	std::unique_ptr<Camera> pCamera;
	std::unique_ptr<World> pWorld;

public:
	Scene() = default;
	explicit Scene(std::unique_ptr<Camera> &&camera):
		pCamera(std::move(camera)),
		pWorld(std::make_unique<World>()) {}
	virtual ~Scene() = default;

	// Accessors
	Camera *GetCamera() const { return Camera.get(); }

	// Mutators
	void SetCamera(Camera *camera) { pCamera.reset(camera); }

	// Methods
	virtual bool Active() = 0;
	virtual void Run() = 0;
	virtual void Update() = 0;
};

class Scenes {
	std::unique_ptr<Scene> pScene;

public:
	Scenes() {}

	// Accessors
	Scene *GetScene() const { return pScene->get(); }
	bool Active() const { return pScene ? pScene->Active() : false; }
	Camera *GetCamera() const { return pScene ? pScene->GetCamera() : nullptr; }

	// Mutators
	void SetScene(std::unique_ptr<Scene> &&scene) { pScene = std::move(scene); }

	// Methods
	void Update() override {
		if (!pScene) return;

		if(!pScene->Running) {
			pScene->Run();
			pScene->Running = true;
		}

		pScene->Update();
	}
	
};

}
