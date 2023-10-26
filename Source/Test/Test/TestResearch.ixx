module;

#include <Ultra/Core/Core.h>

export module Ultra.Test.Research;

import Ultra;
import Ultra.Math.Vector;

export namespace Ultra::Test {

class Research {
public:
    Research() {
        Test();
    }
    ~Research() = default;

    void Test() {
        auto size02 = sizeof(Vector2);
        auto size03 = sizeof(Vector3);
        auto size04 = sizeof(Vector4);

        Vector2 vector2 { 1.0f, 2.0f }; //= Vector2::AsZero();
        Vector3 vector3 { 1.0f, 2.0f, 3.0f };
        Vector4 vector4 { 1.0f, 2.0f, 3.0f };

        auto equal2 = vector2 == Vector2();
        auto equal3 = vector3 == Vector3();
        auto equal4 = vector4 == Vector4();

        vector4.X = 4.0f;

        ColorRGB rgb { 1.0f, 2.0f, 3.0f };
        ColorRGBA rgba { 1.0f, 2.0f, 3.0f, 4.0f };

        auto equalRGB = rgb == ColorRGB();
        auto equalRGBA = rgba == ColorRGBA();

        auto test = true;

        //auto sizeGlmMat2 = sizeof(glm::mat2);   // 16
        //auto sizeGlmMat3 = sizeof(glm::mat3);   // 36
        //auto sizeGlmMat4 = sizeof(glm::mat4);   // 64

        //auto color1 = VectorBase<float, 3, VectorAliases::Color>();
        //auto color2 = VectorBase<float, 4, VectorAliases::Color>();

        //auto sizeA = sizeof(color1);
        //auto sizeB = sizeof(color2);

        Ultra::Test::VectorTests();

        //logger.Test();
        //Log("Default 水 öäü");
        //LogFatal("Fatal");
        //LogError("Error");
        //LogWarning("Warn");
        //LogInfo("Info");
        //LogDebug("Debug");
        //LogTrace("Trace");
    }
};

}
