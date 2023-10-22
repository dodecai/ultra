module;

#include <glm/glm.hpp>

export module Ultra.Asset.Material;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Math;

export namespace Ultra {

struct MaterialData {
    alignas(16) glm::vec3 Ambient { 0.0f };
    alignas(16) glm::vec3 Diffuse { 0.0f };
    alignas(16) glm::vec3 Specular { 0.0f };
    alignas(16) float Shininess = 32.0f;
};

class Material {
public:
    Material() = default;
    ~Material() = default;
};

}
