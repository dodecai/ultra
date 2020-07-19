#pragma once

#include <entt/entt.hpp>

namespace Ultra {

class Scene {
    entt::registry Registry;

public:
    Scene();
    ~Scene();
};

}
