module;

#define TEST_MESH_RENDERER 1
#define TEST_SPRITE_RENDERER 0
#define TEST_UI_RENDERER 0

//#pragma warning(push, 0)
////https://github.com/nothings/stb/issues/334
//#ifndef STB_IMAGE_IMPLEMENTATION
//    #define STB_IMAGE_IMPLEMENTATION
//#endif
//#define STB_IMAGE_STATIC
//#include <stb/stb_image.h>
////#define STB_IMAGE_WRITE_IMPLEMENTATION
//#pragma warning(pop)

export module Ultra.Test.Engine;

import Ultra;
import Ultra.Asset;
import Ultra.Asset.Model;
import Ultra.Math;
import Ultra.Renderer.Buffer;
import Ultra.Renderer.DesignerCamera;
import Ultra.Renderer.PipelineState;
import Ultra.Renderer.Texture;
import Ultra.System.Input;

export namespace Ultra::Test {

float DegreesToRadians(float degrees) {
    return degrees * static_cast<float>(std::numbers::pi) / 180.0f;
}

namespace Components {

// Miscellanous
struct Camera {
    glm::mat4 ViewProjectionMatrix = {};
    glm::mat4 Projection = {};
    glm::mat4 View = {};
    float NearClip = {};
    float FarClip = {};
};

struct CameraLight {
    glm::mat4 Model = {};
    glm::mat4 View = {};
    glm::mat4 Projection = {};
};

struct Grid {
    unsigned int Indices[6] = { 0, 1, 3, 1, 2, 3 };
    float Vertices[12] = {
         1.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
    };
};

enum class LightType: uint32_t {
    Directional = 0,
    Point = 1,
    Spot = 2,
    Flash = 3,
};

struct Light {
    alignas(16) LightType Type = LightType::Directional;
    alignas(16) glm::vec3 LightColor {};
    alignas(16) glm::vec3 LightPosition {};
    alignas(16) glm::vec3 LightDirection {};
    alignas(16) glm::vec3 Ambient { 0.1f, 0.1f, 0.1f };
    alignas(16) glm::vec3 Diffuse { 0.5f, 0.5f, 0.5f };
    alignas(16) glm::vec3 Specular { 1.0f, 1.0f, 1.0f };

    // Attenuation
    float Constant;
    float Linear;
    float Quadratic;

    // Spot Light
    float CutOffAngle;
};

struct Lights {
    Light Light[5] {};
    alignas(16) uint32_t Count = 1;
};

struct Material {
    glm::vec3 uDiffuseColor {};
    glm::vec3 uSpecularColor {};
    glm::vec3 uAmbientColor {};
    float Shininess;
};

struct Properties {
    glm::vec4 Color = glm::vec4(1.0f);
    glm::mat4 Transform = glm::mat4(1.0f);
};

struct Transform {
    glm::vec3 Position = {};
    glm::vec3 Size = { 1.0f, 1.0f, 1.0f };
    glm::vec3 Rotation = {};

    operator glm::mat4() const {
        return
            glm::translate(glm::mat4(1.0f), Position) *
            glm::toMat4(glm::quat(Rotation)) *
            glm::scale(glm::mat4(1.0f), Size);
    }
};

struct View {
    glm::vec3 Position;
};


// Shapes
struct Cube {
    int Components = 36;
    unsigned int Indices[36] = {
         0,  1,  2,  2,  3,  0, // Front
         4,  5,  6,  6,  7,  4, // Back
         3,  2,  6,  6,  7,  3, // Top
         0,  1,  5,  5,  4,  0, // Bottom
         0,  3,  7,  7,  4,  0, // Left
         1,  2,  6,  6,  5,  1, // Right
    };
    float Vertices[24] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };
};

struct Rectangle {
    int Components = 6;
    unsigned int Indices[6] = {
        0, 1, 3,    // First Triangle
        1, 2, 3,    // Second Triangle
    };
    float Vertices[36] = {
        // DrawArrays (specify every rectangle)
        //// First Triangle
        // 0.5f,  0.5f, 0.0f, // Top-Right
        // 0.5f, -0.5f, 0.0f, // Bottom-ight
        //-0.5f,  0.5f, 0.0f, // Top-Left

        //// Second Triangle
        // 0.5f, -0.5f, 0.0f, // Bottom-Right
        //-0.5f, -0.5f, 0.0f, // Bottom-Left
        //-0.5f,  0.5f, 0.0f, // Top-Left

        // DrawIndex (specify only the vertices once)
        // Positions                // Colors           // Texture Coords
         0.5f,  0.5f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top-Right
         0.5f, -0.5f, 0.0f, 0.0f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f, // Bottom-Right
        -0.5f, -0.5f, 0.0f, 0.0f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom-Left
        -0.5f,  0.5f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // Top-Left 
    };
};

struct Skybox {
    int Components = 36;
    unsigned int Indices[36] = {
        0, 1, 2, 2, 3, 0,   // Front
        4, 5, 6, 6, 7, 4,   // Back
        0, 3, 7, 7, 4, 0,   // Left
        1, 2, 6, 6, 5, 1,   // Right
        3, 2, 6, 6, 7, 3,   // Top
        0, 1, 5, 5, 4, 0,   // Bottom
        //0,  1,  2,  2,  3,  0, // Front
        //4,  5,  6,  6,  7,  4, // Back
        //3,  2,  6,  6,  7,  3, // Top
        //0,  1,  5,  5,  4,  0, // Bottom
        //0,  3,  7,  7,  4,  0, // Left
        //1,  2,  6,  6,  5,  1, // Right
    };
    float Vertices[24] = {
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
    };
};

struct Triangle {
    int Components = 3;
    unsigned int Indices[3] = {
        0, 1, 2,
    };
    float Vertices[32] = {
        // Positions            // Colors                   // Texture Coords
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 0.0f,         // Left
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,     1.0f, 0.0f,         // Right
         0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,     0.5f, 1.0f,         // Top
    };
    float TexCoords[6] = {
        // Triangle
        0.0f, 0.0f,  // Lower-Left corner  
        1.0f, 0.0f,  // Lower-Right corner
        0.5f, 1.0f   // Top-Center corner
    };
};

}

class Engine {
public:
    Engine() {
        // Prepare
        AssetManager::Instance().Load();
        mRenderer = Renderer::Create();
        auto swapchain = Swapchain::Create(nullptr, 1280, 1024);
        auto commandBuffer = CommandBuffer::Create();
        mCommandBuffer = mRenderer->GetCommandBuffer();

        // Setup Camera
        auto aspectRatio = 1280.0f / 1024.0f;
        mDesignerCamera = DesignerCamera(45.0f, aspectRatio, 0.1f, 1000.0f);

        // Load Textures
        mCheckerBoard = Texture::Create(TextureProperties(), "./Assets/Textures/CheckerBoard.png");

        // Load Shaders
        mDebugDepthShader = Shader::Create("Assets/Shaders/Debug.Depth.glsl");
        mGridShader = Shader::Create("Assets/Shaders/Grid.glsl");
        mLightShader = Shader::Create("Assets/Shaders/Light.glsl");
        mModelShader = Shader::Create("Assets/Shaders/Material.Blinn-Phong.glsl");
        mSkyBoxShader = Shader::Create("Assets/Shaders/SkyBox.glsl");
        mStencilOutlineShader = Shader::Create("Assets/Shaders/Stencil.Outline.glsl");

        // Load Buffers
        mCameraUniformBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::Camera));
        mCameraUniformBuffer2 = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::CameraLight));
        mMaterialBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::Material));
        mLightBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::Lights));
        mPropertiesBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::Properties));
        mViewBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::View));

    }
    ~Engine() = default;

    void Test(Timestamp deltaTime) {
        mRenderer->RenderFrame();
        mDesignerCamera.Update(deltaTime);

        mCamera.ViewProjectionMatrix = mDesignerCamera.GetViewProjection();
        mCamera.Projection = mDesignerCamera.GetProjectionMatrix();
        mCamera.View = mDesignerCamera.GetViewMatrix();
        mCamera.NearClip = mDesignerCamera.GetNearPoint();
        mCamera.FarClip = mDesignerCamera.GetFarPoint();
        mCameraUniformBuffer->UpdateData(&mCamera, sizeof(Components::Camera));

        mCameraLight.Projection = mDesignerCamera.GetProjectionMatrix();
        mCameraLight.View = mDesignerCamera.GetViewMatrix();
        mCameraLight.Model = glm::mat4(1.0f);

        // Mesh Renderer (3D)
        #if TEST_MESH_RENDERER == 1
            TestMeshRenderer(deltaTime);
        #endif
    
        // Sprite Renderer (2D)
        #if TEST_SPRITE_RENDERER == 1
            TestSpriteRenderer(deltaTime);
        #endif
    
        // UI Renderer (2D)
        #if TEST_UI_RENDERER == 1
            TestUIRenderer(deltaTime);
        #endif
    }

    #pragma region Mesh Renderer

    void TestMeshRenderer(Timestamp deltaTime) {
        // Draw Scene
        //DrawGrid();
        DrawLevel(deltaTime);

        //static Model cone("Assets/Models/Cone/Cone.obj");
        //static Model cube("Assets/Models/Cube/Cube.obj");
        //static Model cylinder("Assets/Models/Cylinder/Cylinder.obj");
        //static Model level("Assets/Models/Level/Level.obj");
        //static Model monkey("Assets/Models/Monkey/Monkey.obj");
        //static Model plane("Assets/Models/Plane/Plane.obj");
        //static Model sphere("Assets/Models/Sphere/SphereUV.obj");
        //static Model torus("Assets/Models/Torus/Torus.obj");
        DrawCube({ { 0.0f, -0.1f, 0.0f }, { 100.0f, 0.1f, -100.0f } });
        DrawCube({ { -4.0f, 10.0f, 0.0f } });
        DrawCube({ { 0.0f, 10.0f, 0.0f }, { 1.0f, 1.0f, 0.2f } });
        DrawCube({ { 4.0f, 10.0f, 0.0f } });
        DrawCube({ { 0.0f, 1.0f, 9.0f }, { 1.0f, 1.0f, 0.2f } });

        DrawLights(deltaTime);
        DrawSkybox(deltaTime);
    }

    void DrawGrid() {
        // Specify Buffers, Pipeline and Texture
        static Components::Grid grid;
        static PipelineProperties properties {
            .BlendMode = BlendMode::Alpha,
            .DepthTest = true,
            .Wireframe = false,
            .Layout = {
                { ShaderDataType::Float3, "aPosition" }
            },
        };
        static Reference<PipelineState> pipeline = PipelineState::Create(properties);
        static Reference<Buffer> vertexBuffer = Buffer::Create(BufferType::Vertex, &grid.Vertices, sizeof(Components::Grid::Vertices));
        static Reference<Buffer> indexBuffer = Buffer::Create(BufferType::Index, &grid.Indices, sizeof(Components::Grid::Indices));

        // Draw
        vertexBuffer->Bind();
        pipeline->Bind();
        indexBuffer->Bind();
        mGridShader->Bind();
        mCameraUniformBuffer->Bind(0);
        mCommandBuffer->DrawIndexed(sizeof(grid.Indices), PrimitiveType::Triangle, true);
    }

    void DrawSkybox(Timestamp deltaTime) {
        // Specify Buffers, Pipeline and Texture
        static Components::Skybox skybox;
        static PipelineProperties properties {
            .DepthTest = true,
            .Wireframe = false,
            .Layout = {
                { ShaderDataType::Float3, "aPosition" }
            }
        };
        static auto pipeline = PipelineState::Create(properties);
        static TextureProperties skyboxProperties = {
            .Dimension = TextureDimension::TextureCube,
        };
        static auto skyboxTexture = Texture::Create(skyboxProperties, "Assets/Textures/Skybox/Sinister");
        static auto vertexBuffer = Buffer::Create(BufferType::Vertex, &skybox.Vertices, sizeof(Components::Skybox::Vertices));
        static auto indexBuffer = Buffer::Create(BufferType::Index, &skybox.Indices, sizeof(Components::Skybox::Indices));

        // Update Transform
        mCameraLight.View = glm::mat4(glm::mat3(mDesignerCamera.GetViewMatrix()));

        // Draw
        vertexBuffer->Bind();
        pipeline->Bind();
        indexBuffer->Bind();
        mSkyBoxShader->Bind();
        skyboxTexture->Bind(0);
        mCameraUniformBuffer2->Bind(0);
        mCameraUniformBuffer2->UpdateData(&mCameraLight, sizeof(Components::CameraLight));
        mCommandBuffer->DrawIndexed(skybox.Components, PrimitiveType::Triangle, false);
    }

    void DrawLevel(Timestamp deltaTime) {
        // Load Model
        static Model test("Assets/Models/Test/Test.obj");

        // Update Transform
        auto model =
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        // Update Camera, Light and View
        mModelShader->Bind();
        mCameraLight.Projection = mDesignerCamera.GetProjectionMatrix();
        mCameraLight.View = mDesignerCamera.GetViewMatrix();
        mCameraLight.Model = model;
        mView.Position = mDesignerCamera.GetPosition();


        // Lighs
        // - Directional Light
        mLights.Light[0] = {
            .Type = Components::LightType::Directional,
            .LightColor = { 1.0f, 1.0f, 1.0f },
            .LightDirection = { 0.0f, -128.0f, -64.0f },
        };
        // - Point Light
        mLights.Light[1] = {
            .Type = Components::LightType::Point,
            .LightColor = mProperties.Color,
            .LightPosition = mLightPosition,
            
            .Constant = 1.0f,
            .Linear = 0.14f,
            .Quadratic = 0.07f,
        };
        // - Spot Light
        mLights.Light[2] = {
            .Type = Components::LightType::Spot,
            .LightColor = { 1.0f, 1.0f, 1.0f },
            .LightPosition = mDesignerCamera.GetPosition(),
            .LightDirection = mDesignerCamera.GetForwardDirection(),

            .Constant = 1.0f,
            .Linear = 0.14f,
            .Quadratic = 0.07f,

            .CutOffAngle = glm::cos(glm::radians(12.5f)),
        };
        // - Finish Light
        mLights.Count = 3;

        // ToDo: Compiler error...
        // Texture Test
        //static MaterialData materialData {
        //    .Shininess = 32.0f * 256.0f
        //};
        mMaterial.Shininess = 16.0f;
        //static auto materialBuffer = Buffer::Create(BufferType::Uniform, &materialData, sizeof(MaterialData));
        static auto diffuseMapTexture = Texture::Create({}, "Assets/Models/Test/diffuse-map.png");
        static auto specularMapTexture = Texture::Create({}, "Assets/Models/Test/specular-map.png");

        //materialBuffer->UpdateData(&materialData, sizeof(MaterialData));

        //materialBuffer->Bind(0);
        diffuseMapTexture->Bind(0);
        specularMapTexture->Bind(2);

        mCameraUniformBuffer2->Bind(0);
        mViewBuffer->Bind(4);
        //mMaterialBuffer->Bind(5);
        mLightBuffer->Bind(6);
        mCameraUniformBuffer2->UpdateData(&mCameraLight, sizeof(Components::CameraLight));
        //mMaterialBuffer->UpdateData(&sComponents.Material, sizeof(Components::UMaterial));
        mLightBuffer->UpdateData(&mLights, sizeof(Components::Lights));
        mViewBuffer->UpdateData(&mView, sizeof(Components::View));

        test.Draw(mCommandBuffer);
    }

    void DrawLights(Timestamp deltaTime) {
        // Specify Buffers, Pipeline and Texture
        static Components::Cube cube;
        static PipelineProperties properties {
            .DepthTest = true,
            .Wireframe = false,
            .Layout = {
                { ShaderDataType::Float3, "aPosition" }
            },
        };
        static auto pipeline = PipelineState::Create(properties);

        // Specify Buffers and Textures
        static auto vertexBuffer = Buffer::Create(BufferType::Vertex, &cube.Vertices, sizeof(Components::Cube::Vertices));
        static auto indexBuffer = Buffer::Create(BufferType::Index, &cube.Indices, sizeof(Components::Cube::Indices));

        // Update Properties
        mLightPosition = glm::vec3(-3.0f, 3.0f, 5.0f);
        static float timeValue = 0.1f;
        timeValue += (float)deltaTime;
        float angle = timeValue * 2.0f;
        float radius = 5.0f;
        mLightPosition.x = 1.0f + radius * std::cos(angle);
        mLightPosition.z = 3.0f + radius * std::sin(angle);

        float value = (sin(timeValue) / 2.0f) + 0.5f;
        mProperties.Color = glm::vec4(1.0f, 1.0f - value, value, 1.0f);
        auto model = glm::translate(glm::mat4(1.0f), mLightPosition) *
                     glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
        mProperties.Transform = mDesignerCamera.GetProjectionMatrix() * mDesignerCamera.GetViewMatrix() * model;

        // Draw
        mCommandBuffer->UpdateStencilBuffer();

        vertexBuffer->Bind();
        pipeline->Bind();
        indexBuffer->Bind();

        mLightShader->Bind();
        mPropertiesBuffer->Bind(0);
        mPropertiesBuffer->UpdateData(&mProperties, sizeof(Components::Properties));
        mCommandBuffer->DrawIndexed(cube.Components, PrimitiveType::Triangle, true);

        mCommandBuffer->EnableStencilTest();

        model = glm::translate(glm::mat4(1.0f), mLightPosition) *
                glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
        mProperties.Transform = mDesignerCamera.GetProjectionMatrix() * mDesignerCamera.GetViewMatrix() * model;

        vertexBuffer->Bind();
        pipeline->Bind();
        indexBuffer->Bind();

        mStencilOutlineShader->Bind();
        mPropertiesBuffer->Bind(0);
        mPropertiesBuffer->UpdateData(&mProperties, sizeof(Components::Properties));
        mCommandBuffer->DrawIndexed(cube.Components, PrimitiveType::Triangle, false);

        mCommandBuffer->ResetStencilTest();
    }



    void DrawCube(const Components::Transform &transform) {
        static Model cube("Assets/Models/Cube/Cube.obj");
        auto model = transform;

        mModelShader->Bind();
        mCameraLight.Model = model;
        mView.Position = mDesignerCamera.GetPosition();

        mCameraUniformBuffer2->Bind(0);
        mViewBuffer->Bind(4);
        //mMaterialBuffer->Bind(5);
        mCameraUniformBuffer2->UpdateData(&mCameraLight, sizeof(Components::CameraLight));
        //mMaterialBuffer->UpdateData(&mMaterial, sizeof(Components::Material));
        mViewBuffer->UpdateData(&mView, sizeof(Components::View));
        cube.Draw(mCommandBuffer);
    }

    #pragma endregion

    #pragma region Sprite Renderer
    void TestSpriteRenderer(Timestamp deltaTime) {
        // Prepare
        Renderer2D::StartScene(mDesignerCamera);

        // 2D Renderer: Primitives
        Renderer2D::DrawLine({ -0.9f, -0.9f }, { 0.9f,  -0.9f }, { 0.9f, 0.9f, 0.9f, 1.0f });
        Renderer2D::DrawLine({ -0.9f, -0.9f }, { -0.9f,   0.9f }, { 1.0f, 0.0f, 1.0f, 1.0f });
        Renderer2D::DrawLine({ 0.2f,  0.2f }, { 0.7f,   0.7f }, { 1.0f, 1.0f, 1.0f, 1.0f });
        Renderer2D::DrawQuad({ -0.6f, -0.6f }, { 0.5f,   0.5f }, mCheckerBoard, 1.0f, { 1.0f, 0.0f, 0.0f, 1.0f });
        Renderer2D::DrawQuad({ 0.2f,  0.2f }, { 0.7f,   0.7f }, mCheckerBoard, 1.0f, { 0.0f, 0.0f, 1.0f, 1.0f });
        Renderer2D::DrawRect({ -0.9f,  0.9f }, { 0.5f,   0.5f }, { 0.2f, 0.2f, 0.2f, 1.0f });
        Renderer2D::DrawCircle({ 1.0f, 1.0f }, { 0.5f,   0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, 0.1f, 0.1f);

        static float rotation = 0.0f;
        const float speed = 180.0f;
        rotation += speed * deltaTime;
        if (rotation >= 360.0f) rotation = 0.0f;
        Renderer2D::DrawRotatedQuad({ 0.7f,   0.7f }, { 0.2f,  0.2f }, rotation * (3.14f / 180.0f), mCheckerBoard, 1.0f, { 0.0f, 1.0f, 0.0f, 1.0f });
        Renderer2D::DrawRotatedQuad({ 0.7f,  -0.6f }, { 0.4f,  0.4f }, rotation * (3.14f / 180.0f) * -1.0f, mCheckerBoard, 1.0f, { 0.7f, 0.7f, 0.7f, 1.0f });

        // Finish
        Renderer2D::FinishScene();
    }

    #pragma endregion

    #pragma region UI Renderer

    void TestUIRenderer(Timestamp deltaTime) {
    }

    #pragma endregion

    #pragma region Miscellanous

    void ToDo() {
        //// Model, View, Projection
        ////auto orthographic = glm::ortho(0.0f, 1280.0f, 0.0f, 1024.0f, 0.1f, 100.0f);
        //auto model = glm::mat4(1.0f);
        //auto view = glm::mat4(1.0f);
        //auto projection = glm::perspective(glm::radians(flyCamera.Zoom), 1280.0f / 1024.0f, 0.1f, 100.0f);
        //view = flyCamera.GetViewMatrix();
        ////view = glm::translate(view, glm::vec3(-1.0f, 1.0f, -5.0f));
        ////view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ////view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ////view = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetForwardDirection(), camera.GetUpDirection());

        //// Scale, Rotation and Transformation
        //auto translation = projection * view * model;
        ////glm::mat4 translation = glm::mat4(1.0f);
        ////translation = glm::translate(translation, glm::vec3(0.5f, -0.5f, 0.0f));
        ////translation = glm::rotate(translation, glm::radians(speed), glm::vec3(0.0f, 0.0f, 1.0f));
        ////translation = glm::scale(translation, glm::vec3(0.52f, 0.52f, 0.52f));
        ////view = glm::scale(model, glm::vec3(1.0f, 1.0f, -1.0f)); // Flip Z-Axis (OpenGL is a right-handed system)
    }

    #pragma endregion

private:
    CommandBuffer *mCommandBuffer;
    DesignerCamera mDesignerCamera;
    Reference<Texture> mCheckerBoard;
    Scope<Renderer> mRenderer;

    // Point Light Position
    glm::vec3 mLightPosition {};

    // Buffers
    Reference<Buffer> mCameraUniformBuffer;
    Reference<Buffer> mCameraUniformBuffer2;
    Reference<Buffer> mMaterialBuffer;
    Reference<Buffer> mLightBuffer;
    Reference<Buffer> mPropertiesBuffer;
    Reference<Buffer> mViewBuffer;

    // Components
    Components::Camera mCamera;
    Components::CameraLight mCameraLight;
    Components::Lights mLights;
    Components::Material mMaterial;
    Components::Properties mProperties;
    Components::Transform mTransform;
    Components::View mView;

    // Shaders
    Reference<Shader> mDebugDepthShader;
    Reference<Shader> mGridShader;
    Reference<Shader> mModelShader;
    Reference<Shader> mLightShader;
    Reference<Shader> mSkyBoxShader;
    Reference<Shader> mStencilOutlineShader;
};

}
