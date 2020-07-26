#include "Scene.h"

#include "Ultra.pch"

#include <glm/glm.hpp>

#include "Entity.h"
#include "Engine/Renderer/Renderer2D.h"

namespace Ultra {

Scene::Scene() {
    #ifdef Scene_Example
    TransformComponent transform;

    entt::entity entity = Registry.create();
    Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

    Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

    if (Registry.has<TransformComponent>(entity)) transform = Registry.get<TransformComponent>(entity);

    auto &view = Registry.view<TransformComponent>();
    for (auto entity : view) {
        TransformComponent &transform = view.get<TransformComponent>(entity);
    }


    auto group = Registry.group<TransformComponent>(entt::get<MeshComponent>);
    for (auto entity : group) {
        auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
    }
    #endif
}

Scene::~Scene() {
    Registry.clear();
}

void Scene::Update(Timestamp deltaTime) {

    //auto group = Registry.group<TransformComponent>(entt::get<CSpriteRenderer>);
    //for (auto entity : group) {
    //    auto &[transform, sprite] = group.get<TransformComponent, CSpriteRenderer>(entity);
    //}
    auto group = Registry.group<TransformComponent>(entt::get<CSpriteRenderer>);
    for (auto entity : group) {
        auto [transform, sprite] = group.get<TransformComponent, CSpriteRenderer>(entity);
        Renderer2D::DrawQuad(transform, sprite.Color);
    }
}

Entity Scene::CreateEntitiy(const string &name) {
    Entity entity = { Registry.create(), this };
    auto &tag = entity.AddComponent<TagComponent>();
    entity.AddComponent<TransformComponent>();
    
    tag.Tag = name.empty() ? "Entity" : name;

    return entity;
}


}
