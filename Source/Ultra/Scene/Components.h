#pragma once

#include <entt/entt.hpp>

#include "Ultra.pch"
#include "Core.h"

#include <glm/glm.hpp>

namespace Ultra {

struct MeshComponent {
    bool Data;
};

struct TagComponent {
    string Tag = "";

    TagComponent() = default;
    TagComponent(const TagComponent &) = default;
    TagComponent(const string &tag): Tag(tag) {}
};

struct TransformComponent {
    glm::mat4 Transform { 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent &) = default;
    TransformComponent(const glm::mat4 &transform): Transform(transform) {}

    operator glm::mat4 &() { return Transform; }
    operator const glm::mat4 &() const { return Transform; }
};

struct CSpriteRenderer {
    glm::vec4 Color { 1.0f };

    CSpriteRenderer() = default;
    CSpriteRenderer(const CSpriteRenderer &) = default;
    CSpriteRenderer(const glm::vec4 &color): Color(color) {}
};


// Systems
static void OnTransformConstruct(entt::registry &registry, entt::entity entity) {

}

}
