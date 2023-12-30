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

        Direction2D aDirection2D { 1.0f, 1.0f };
        Direction3D aDirection3D { 1.0f, 1.0f, 1.0f };
        Normal2D aNormal2D { 1.0f, 1.0f };
        Normal3D aNormal3D { 1.0f, 1.0f, 1.0f };
        Position2D aPosition2D { 1.0f, 1.0f };
        Position3D aPosition3D { 1.0f, 1.0f, 1.0f };
        Position4D aPosition4D { 1.0f, 1.0f, 1.0f, 1.0f };
        //Rotation2D rotation2D { 1.0f };
        Rotation3D aRotation3D { 1.0f, 1.0f, 1.0f };
        TextureCoord2D aTextureCoordinate2D { 1.0f, 1.0f };
        TextureCoord3D aTextureCoordinate3D { 1.0f, 1.0f, 1.0f };

        Matrix2 bMatrix2x2 { 0.0f, 1.0f, 2.0f, 3.0f };
        Matrix3 bMatrix3x3 { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
        Matrix4 bMatrix4x4 { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f };

        Quaternion q0 { 1, 0, 0, 0 };

        auto size02 = sizeof(Vector2);
        auto size03 = sizeof(Vector3);
        auto size04 = sizeof(Vector4);

        Vector2 vector2 { 1.0f, 2.0f }; //= Vector2::AsZero();
        Vector3 vector3 { 1.0f, 2.0f, 3.0f };
        Vector4 vector4 { 1.0f, 2.0f, 3.0f };

        vector4.X = 4.0f;

        array<TextureCoord2D, 4> testTextureCoord2D { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };

        auto test = true;

        //auto sizeGlmMat2 = sizeof(glm::mat2);   // 16
        //auto sizeGlmMat3 = sizeof(glm::mat3);   // 36
        //auto sizeGlmMat4 = sizeof(glm::mat4);   // 64

        //auto color1 = VectorBase<float, 3, VectorAliases::Color>();
        //auto color2 = VectorBase<float, 4, VectorAliases::Color>();

        //auto sizeA = sizeof(color1);
        //auto sizeB = sizeof(color2);

        Ultra::Test::VectorTests();
        Ultra::Test::MatrixTests();
        Ultra::Test::QuaternionTests();

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
