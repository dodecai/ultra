export module Ultra.Asset;

import Ultra.Core;
import Ultra.Math;

export namespace Ultra {

enum class AssetType {
    Font,
    Image,
    Model,
    Script,
    Shader,
    Sound,
    Texture,
    Unknown,
};

string to_string(const AssetType &type) {
    switch (type) {
        case AssetType::Font:       return "Font";
        case AssetType::Image:      return "Image";
        case AssetType::Model:      return "Model";
        case AssetType::Script:     return "Script";
        case AssetType::Shader:     return "Shader";
        case AssetType::Sound:      return "Sound";
        case AssetType::Texture:    return "Texture";
        case AssetType::Unknown:    return "Unknown";
        default:                    return "Invalid";
    }
}

struct Asset {
    AssetType Type { AssetType::Unknown };
    string Path {};
    string ID {};
};

namespace Colors {
    // ToDo: Switch to Vector class
    //Vector3Df Black { 0.0f, 0.0f, 0.0f };
    //Vector3Df Blue  { 0.0f, 0.0f, 1.0f };
    //Vector3Df Coral { 1.0f, 0.5f, 0.31f };
    //Vector3Df Green { 0.0f, 1.0f, 0.0f };
    //Vector3Df Red   { 1.0f, 0.0f, 0.0f };
    //Vector3Df White { 1.0f, 1.0f, 1.0f };

    glm::vec3 Black { 0.0f, 0.0f, 0.0f };
    glm::vec3 Blue  { 0.0f, 0.0f, 1.0f };
    glm::vec3 Coral { 1.0f, 0.5f, 0.31f };
    glm::vec3 Green { 0.0f, 1.0f, 0.0f };
    glm::vec3 Red   { 1.0f, 0.0f, 0.0f };
    glm::vec3 White { 1.0f, 1.0f, 1.0f };
}

struct Light {
    glm::vec3 Color { 1.0f };
    glm::vec3 AmbientIntensity { 0.1f };
    glm::vec3 DiffuseIntensity { 0.5f };
    glm::vec3 SpecularIntensity { 1.0f };

    float Intensity = 1.0f;
};

struct DirectionalLight: public Light {
    glm::vec3 Direction;
};

struct PointLight: public Light {
    glm::vec3 Position;

    // Attenuation
    float Constant = 1.0f;
    float Linear = 0.09f;
    float Quadratic = 0.032f;
};

struct SpotLight: public Light {
    glm::vec3 Position;
    glm::vec3 Direction;

    // Attenuation
    float Constant = 1.0f;
    float Linear = 0.09f;
    float Quadratic = 0.032f;

    // Cutoff
    float CutOffAngle = glm::cos(glm::radians(12.5f));
};

}
