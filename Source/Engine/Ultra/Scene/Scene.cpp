#include "Scene.h"

#include <glm/glm.hpp>

#include "Ultra.pch"
#include "Ultra/Renderer/Renderer.h"
#include "Ultra/Renderer/Renderer2D.h"

#include "Entity.h"

namespace Ultra {

Scene::Scene(const string &caption): mCaption(caption) {}

Scene::~Scene() { Clear(); }

Entity Scene::CreateEntity(const string &name) {
    Entity entity = { Registry.create(), this };
    auto &id = entity.AddComponent<Component::Identifier>();
    auto &tag = entity.AddComponent<Component::Tag>(name);
    entity.AddComponent<Component::Transform>();
    return entity;
}

Entity Scene::CreateEntity(const UUID<string> &id) {
    Entity entity = { Registry.create(), this };
    auto &identifier = entity.AddComponent<Component::Identifier>(id);
    return entity;
}


void Scene::Clear() {
    Registry.clear();
}

void Scene::Update(Timestamp deltaTime) {
    // Update Scripts
    {
        Registry.view<Component::NativeScript>().each([=](auto entity, auto &nsc) {
            if (!nsc.Instance) {
                nsc.Instance = nsc.Load2();
                nsc.Instance->mEntity = Entity{ entity, this };
                nsc.Instance->Create();
            }

            nsc.Instance->Update(deltaTime);
        });
    }


    // 2D 
    Camera *sceneCamera = nullptr;
    glm::mat4 *cameraTransform = nullptr;
    {
        auto view = Registry.view<Component::Camera, Component::Transform>();
        for (auto entity : view) {
            auto [camera, transform] = view.get<Component::Camera, Component::Transform>(entity);

            if (camera.Primary) {
                sceneCamera = &camera.mCamera;
                cameraTransform = &transform.mTransform;
            }
        }
    }
    if (sceneCamera) {
        Renderer2D::StartScene(*sceneCamera, *cameraTransform);
        {
            try {
                auto group = Registry.group<Component::Transform>(entt::get<Component::Sprite>);
                for (auto entity : group) {
                    auto [transform, sprite] = group.get<Component::Transform, Component::Sprite>(entity);

                    Renderer2D::DrawQuad(transform, sprite.Color);
                }
            } catch (...) {
                AppLogError("An unknwon entt error occured!");
            }
        }

        Renderer2D::FinishScene();
    }

}

void Scene::Resize(uint32_t width, uint32_t height) {
    mWidth = width;
    mHeight = height;

    auto view = Registry.view<Component::Camera>();
    for (auto entity : view) {
        auto &camera = view.get<Component::Camera>(entity);
        if (camera.ProjectionType == SceneCamera::ProjectionType::Orthographic && camera.FixedAspectRatio) return;
        camera.mCamera.SetViewportSize(width, height);
    }
}


const Camera &Scene::GetCamera() {
    auto view = Registry.view<Component::Camera>();
    for (auto entity : view) {
        auto camera = view.get<Component::Camera>(entity);
        if (camera.Primary) { return camera.mCamera; }
    }
}

const string &Scene::GetCaption() const {
    return mCaption;
}

const Entity &Scene::GetEntity(const string &name) {
    auto view = Registry.view<Component::Tag>();
    for (auto entity : view) {
        auto &component = view.get<Component::Tag>(entity);
        if (component == name) {
            return { entity, this };
        }
    }
    return Entity();
}

const Entity &Scene::GetEntity(const UUID<string> &id) {
    auto view = Registry.view<Component::Identifier>();
    for (auto entity : view) {
        auto &component = view.get<Component::Identifier>(entity);
        if (component == id) {
            return { entity, this };
        }
    }
    return Entity();
}


void Scene::SetCaption(const string &caption) {
    mCaption = caption;
}

}
