module;

//#define ENTT_ASSERT(condition)
#include <entt/entt.hpp>

export module Ultra.Engine.Scene;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Engine.DesignerCamera;
import Ultra.Engine.Components;
import Ultra.Engine.SceneCamera;
import Ultra.Engine.Renderer2D;
import Ultra.Engine.Renderer.Framebuffer;
import Ultra.Utility.Timer;
import Ultra.Utility.UUID;

export namespace Ultra {

class Entity;
class Serializer;

class Scene {
    // Friends
    friend class Entity;
    friend class Serializer;

public:
    Scene(const string &caption = "Scene"): mCaption(caption) {
        //Registry.on_construct<Component::Camera>().connect<&Scene::UpdateCamera>(*this);
    };
    //explicit Scene(Scope<Camera> &&camera): pCamera(std::move(camera)) {}
    ~Scene() { Clear(); };

    //Entity CreateEntity(const string &name = "Entity") {
    //    //Entity entity = { Registry.create(), this };
    //    //auto &id = entity.AddComponent<Component::Identifier>();
    //    //auto &tag = entity.AddComponent<Component::Tag>(name);
    //    //entity.AddComponent<Component::Transform>();
    //    //return entity;
    //    return {};
    //}
    //Entity CreateEntity(const UUID<string> &id) {
    //    //Entity entity = { Registry.create(), this };
    //    //auto &identifier = entity.AddComponent<Component::Identifier>(id);
    //    //return entity;
    //    return {};
    //}
    //void DestroyEntity(Entity entity) {
    //    //Registry.destroy(entity);
    //}

    void Clear() {
        Registry.clear();
    }
    void UpdateDesigner(Timestamp deltaTime, DesignerCamera &camera) {
        Renderer2D::StartScene(camera);
        {
            try {
                auto group = Registry.group<Component::Transform>(entt::get<Component::Sprite>);
                for (auto entity : group) {
                    auto [transform, sprite] = group.get<Component::Transform, Component::Sprite>(entity);
                    Renderer2D::DrawQuad(transform, sprite.Color, "");
                }
            } catch (...) {
                LogError("An unknown entt error occurred!");
            }
        }

        Renderer2D::FinishScene();
    }
    void UpdateRuntime(Timestamp deltaTime) {
        // Update Scripts
        {
            //Registry.view<Component::NativeScript>().each([=](auto entity, auto &nsc) {
            //    if (!nsc.Instance) {
            //        nsc.Instance = nsc.Load2();
            //        nsc.Instance->mEntity = Entity { entity, this };
            //        nsc.Instance->Create();
            //    }

            //    nsc.Instance->Update(deltaTime);
            //});
        }

        // 2D 
        Camera *sceneCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = Registry.view<Component::Camera, Component::Transform>();
            for (auto entity : view) {
                auto [camera, transform] = view.get<Component::Camera, Component::Transform>(entity);

                if (camera.Primary) {
                    sceneCamera = &camera.mCamera;
                    cameraTransform = transform;
                }
            }
        }
        if (sceneCamera) {
            Renderer2D::StartScene(*sceneCamera, cameraTransform);
            {
                try {
                    auto group = Registry.group<Component::Transform>(entt::get<Component::Sprite>);
                    for (auto entity : group) {
                        auto [transform, sprite] = group.get<Component::Transform, Component::Sprite>(entity);

                        Renderer2D::DrawQuad(transform, sprite.Color);
                    }
                } catch (...) {
                    LogError("An unknown entt error occurred!");
                }
            }

            Renderer2D::FinishScene();
        }

    }
    void Resize(uint32_t width, uint32_t height) {
        if (width == 0 && height == 0) return;
        mWidth = width;
        mHeight = height;

        auto view = Registry.view<Component::Camera>();
        for (auto &entity : view) {
            auto &camera = view.get<Component::Camera>(entity);
            if (camera.ProjectionType == SceneCamera::ProjectionType::Orthographic && camera.FixedAspectRatio) return;
            camera.mCamera.SetViewportSize(width, height);
        }
    }

    // Accessors
    const Camera &GetCamera() {
        auto view = Registry.view<Component::Camera>();
        for (auto &entity : view) {
            const auto &camera = view.get<Component::Camera>(entity);
            if (camera.Primary) { return camera.mCamera; }
        }
        return {};
    }
    //Entity GetCameraEntity() {
    //    auto view = Registry.view<Component::Camera>();
    //    for (auto &entity : view) {
    //        const auto &camera = view.get<Component::Camera>(entity);
    //        if (camera.Primary) { return { entity, this }; }
    //    }
    //    return {};
    //}
    const string &GetCaption() const { return mCaption; }
    //const Entity GetEntity(const string &name) {
    //    auto view = Registry.view<Component::Tag>();
    //    for (auto &entity : view) {
    //        auto &component = view.get<Component::Tag>(entity);
    //        if (component == name) {
    //            return { entity, this };
    //        }
    //    }
    //    return {};
    //}
    //const Entity GetEntity(const UUID<string> &id) {
    //    auto view = Registry.view<Component::Identifier>();
    //    for (auto &entity : view) {
    //        auto &component = view.get<Component::Identifier>(entity);
    //        if (component == id) {
    //            return { entity, this };
    //        }
    //    }
    //    static Entity *ptr = nullptr;
    //    return *ptr;
    //}
    const uint32_t GetHeight() const { return mHeight; }
    const uint32_t GetWdith() const { return mWidth; }
    bool IsActive() const { return mActive; };

private:
    // Mutators
    void SetCaption(const string &caption) { mCaption = caption; }
    void UpdateCamera(Scene &scene) {
        scene.Resize(scene.GetWdith(), scene.GetHeight());
    }

private:
    //SceneCamera *pCamera = nullptr;
    entt::registry Registry;
    vector<Entity *> mEntities;

    std::string mCaption;
    bool mActive = false;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
};

class SceneLibrary {
public:
    SceneLibrary() = default;
    void Update(Timestamp deltaTime) {
        GetActiveScene()->UpdateRuntime(deltaTime);
    }

    void Push(Reference<Scene> &scene) {
        mScenes.push_back(scene);
    }
    void Pop(Reference<Scene> &scene) {
    }

private:
    Reference<Scene> &GetActiveScene() {
        for (auto &scene : mScenes) {
            if (scene->IsActive()) {
                return scene;
            }
        }
    }

private:
    vector<Reference<Scene>> mScenes;
};

}


