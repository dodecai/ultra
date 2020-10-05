#include "Scene.h"

#include "Ultra.pch"

#include <glm/glm.hpp>

#include "Entity.h"
#include "Ultra/Renderer/Renderer.h"
#include "Ultra/Renderer/Renderer2D.h"

namespace Ultra {

Scene::Scene(const string &name):
    mName { name } {
}

Scene::~Scene() {
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
    Width = width;
    Height = height;

    auto view = Registry.view<Component::Camera>();
    for (auto entity : view) {
        auto &camera = view.get<Component::Camera>(entity);
        if (!camera.FixedAspectRatio) {
            camera.mCamera.SetViewportSize(width, height);
        }
    }
}

Entity Scene::CreateEntity(const string &name) {
    Entity entity = { Registry.create(), this };
    auto &id = entity.AddComponent<Component::Identifier>();
    auto &tag = entity.AddComponent<Component::Tag>();
    entity.AddComponent<Component::Transform>();
    
    tag = name.empty() ? "Entity" : name;

    return entity;
}

Entity Scene::CreateEntity(const uint64_t id) {
    Entity entity = { Registry.create(), this };
    auto &identifier = entity.AddComponent<Component::Identifier>();
    identifier = id;

    return entity;
}

void Scene::AttachEntity(Entity *entity) {}

void Scene::DetachEntity(Entity *entity) {}

}
