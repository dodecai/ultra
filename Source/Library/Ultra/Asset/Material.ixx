module;

export module Ultra.Asset.Material;

import <glm/glm.hpp>;
import <glm/gtc/matrix_transform.hpp>;
import <glm/gtc/type_ptr.hpp>;

import Ultra.Core;
import Ultra.Logger;

export namespace Ultra {

struct MaterialData {
    alignas(16) glm::vec3 Ambient { 0.0f };
    alignas(16) glm::vec3 Diffuse { 0.0f };
    alignas(16) glm::vec3 Specular { 0.0f };
    alignas(16) float Shininess = 0.0f;
};

class Material {
public:
    Material() = default;
    ~Material() = default;
};

}
