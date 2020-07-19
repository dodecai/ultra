#include "Scene.h"

#include "Ultra.pch"

#include <glm/glm.hpp>

namespace Ultra {

struct MeshComponent {};

struct TransformComponent {
    glm::mat4 Transform;

    TransformComponent() = default;
    TransformComponent(const TransformComponent &) = default;
    TransformComponent(const glm::mat4 &transform): Transform(transform) {}

    operator glm::mat4 &() { return Transform; }
    operator const glm::mat4 &() const { return Transform; }
};

static void OnTransformConstruct(entt::registry &registry, entt::entity entity) {

}

Scene::Scene() {
    TransformComponent transform;

    entt::entity entity = Registry.create();
    auto &transform = Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

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
}

Scene::~Scene() {
    Registry.clear();
}


}
