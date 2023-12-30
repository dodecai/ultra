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
import Ultra.UI.GUIBuilder;
import Ultra.UI.GUILayer;
import Ultra.System.Input;

namespace Ultra::NewAndTasty {

template <typename T, size_t N>
struct PositionBase {
    array<T, N> Data = {};
};

template <typename T = float>
struct Position2D: PositionBase<T, 2> {
    [[msvc::no_unique_address]] T &X = this->Data[0];
};

template <typename T = float>
struct Position3D: PositionBase<T, 3> {
};

}

export namespace Ultra::Test {

enum class UniformPosition {
    RendererData    = 0,
    CameraData      = 1,
    SceneData       = 2,
    ScreenData      = 3,
    EntityData      = 4,
    AnimationData   = 5,
    LightData       = 6,
    ShadowData      = 7,
    MaterialData    = 9,
};

float DegreesToRadians(float degrees) {
    return degrees * static_cast<float>(std::numbers::pi) / 180.0f;
}

namespace Components {

// Miscellanous
struct Camera {
    glm::mat4 ViewProjection = {};
    glm::mat4 Projection = {};
    glm::mat4 View = {};
    glm::mat4 InverseViewProjection = {};
    glm::mat4 InverseProjection = {};
    glm::mat4 InverseView = {};
    glm::vec3 Position {};
    float Near = {};
    float Far = {};
};

struct EntityData {
    glm::vec4 Color = {};
    glm::mat4 Transform = {};
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

class Engine: public Ultra::Layer {
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
        mDesignerCamera = DesignerCamera(45.0f, aspectRatio, 0.1f, 10000.0f);
        mDesignerCamera.SetViewportSize(1280.0f, 1024.0f);
        mDesignerCamera.SetPosition({ 0.0f, 4.0f, -24.0f });

        // Load Shaders
        mDebugDepthShader = Shader::Create("Assets/Shaders/Debug/DepthVisualizer.glsl");
        mGridShader = Shader::Create("Assets/Shaders/Grid.glsl");
        mLightShader = Shader::Create("Assets/Shaders/Light.glsl");
        mModelShader = Shader::Create("Assets/Shaders/Materials/Material.Blinn-Phong.glsl");
        mNormalsShader = Shader::Create("Assets/Shaders/Debug/NormalsVisualizer.glsl");
        mSkyBoxShader = Shader::Create("Assets/Shaders/SkyBox.glsl");
        mStencilOutlineShader = Shader::Create("Assets/Shaders/Stencil.Outline.glsl");

        // Load Buffers
        mCameraUniformBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::Camera));
        mEntityUniformBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::EntityData));
        mLightBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::Lights));
        mMaterialBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::Material));

        // Load Textures
        mCheckerBoard = Texture::Create({}, "./Assets/Textures/CheckerBoard.png");
        mConcreteTexture = Texture::Create({ .SamplerWrap = TextureWrap::Repeat }, "./Assets/Textures/Concrete.png");
        mGrassTexture = Texture::Create({ .SamplerWrap = TextureWrap::MirrorClamp }, "./Assets/Textures/Grass.png");
        mMarbleTexture = Texture::Create({ .SamplerWrap = TextureWrap::Clamp, .GenerateMips = true, }, "./Assets/Textures/Marble.jpg");
        mMatrixTexture = Texture::Create({}, "./Assets/Textures/Matrix.jpg");
        mMetalTexture = Texture::Create({}, "./Assets/Textures/Metal.png");
        mSmileyTexture = Texture::Create({ .SamplerWrap = TextureWrap::Clamp }, "./Assets/Textures/Smiley.png");
        mWindowTexture = Texture::Create({ .SamplerWrap = TextureWrap::Clamp }, "./Assets/Textures/Window.png");
        mWoodTexture = Texture::Create({}, "./Assets/Textures/Wood.png");
    }
    ~Engine() = default;

    void Test(Timestamp deltaTime) {
        mRenderer->RenderFrame();

        // Update Camera
        if (!mUIActive) mDesignerCamera.Update(deltaTime);

        mCamera.ViewProjection = mDesignerCamera.GetViewProjection();
        mCamera.Projection = mDesignerCamera.GetProjectionMatrix();
        mCamera.View = mDesignerCamera.GetViewMatrix();
        mCamera.InverseViewProjection = mDesignerCamera.GetInverseViewProjection();
        mCamera.InverseProjection = mDesignerCamera.GetInverseProjection();
        mCamera.InverseView = mDesignerCamera.GetInverseView();
        mCamera.Position = mDesignerCamera.GetPosition();
        mCamera.Near = mDesignerCamera.GetNearPoint();
        mCamera.Far = mDesignerCamera.GetFarPoint();
        mCameraUniformBuffer->UpdateData(&mCamera, sizeof(Components::Camera));
        mCameraUniformBuffer->Bind(0);
        mCameraUniformBuffer->Bind((uint32_t)UniformPosition::CameraData);

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

    void GuiUpdate() override {
        static bool active = true;
        static float value = 1.0f;
        static auto color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        ImGui::Begin("Renderer");

        mUIActive = ImGui::IsWindowFocused();
        UI::Property("Color", glm::value_ptr(color));
        UI::Property("Distance", value);
        UI::Property("Lights", active);
        UI::Label("Lights Count: %d", 3);
        UI::LabelX("Lights CountX: %d", 3);
        UI::Property("State", "Count: % d", 3);
        ImGui::End();
    }

    #pragma region Mesh Renderer

    void TestMeshRenderer(Timestamp deltaTime) {
        // Draw Scene
        //DrawGrid();
        DrawLevel(deltaTime);
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
        mCommandBuffer->DrawIndexed(sizeof(grid.Indices), PrimitiveType::Triangle, true);
    }

    void DrawLevel(Timestamp deltaTime) {
        // Load Model
        //static Model backpack("Assets/Models/Backpack/Backpack.obj");
        static Model level("Assets/Models/Test/Test.obj");
        static Model sphere("Assets/Models/Sphere/SphereUV.obj");
        static Model nanosuit("Assets/Models/Nanosuit/Nanosuit.obj");
        //static Model sponza("Assets/Models/Sponza/Sponza.obj");

        // Draw Lights
        DrawLights(deltaTime);

        // Draw Models
        //DrawModel(backpack, { 7.0f, 2.0f, 9.0f }, { 0.2f, 0.2f, 0.2f });
        DrawModel(nanosuit, { 7.0f, 0.0f, -8.0f }, { 0.2f, 0.2f, 0.2f });
        mMetalTexture->Bind(0);
        mMetalTexture->Bind(2);
        DrawModel(level, { 0.0f, 0.0f, 0.0f });
        mConcreteTexture->Bind(0);
        mConcreteTexture->Bind(2);
        DrawModel(sphere, { 10.0f, 10.0f, 0.0f });
        //DrawModel(sponza, { 0.0f, 0.0f, 0.0f }, { 0.1f, 0.1f ,0.1f });

        // Draw Objects
        static float CubeAX = -4.0f;
        static float CubeBX = 0.0f;
        static float CubeCX = 4.0f;
        static bool left = true;
        float direction = 1.0f;
        if (CubeAX > -8.0f && left) {
            CubeAX -= direction * deltaTime;
            CubeBX -= direction * deltaTime;
            CubeCX -= direction * deltaTime;
        } else if (CubeAX < -8.0f && left) {
            left = false;
        } else if (CubeCX < 8.0f && !left) {
            CubeAX += direction * deltaTime;
            CubeBX += direction * deltaTime;
            CubeCX += direction * deltaTime;
        } else if (CubeCX > 8.0f && !left) {
            left = true;
        }

        mWoodTexture->Bind(0);
        mWoodTexture->Bind(2);
        DrawCube({ { 0.0f, -0.001f, 0.0f }, { 12, 0.1f, -12.0f } });
        mMarbleTexture->Bind(0);
        mMarbleTexture->Bind(2);
        DrawCube({ { CubeAX, 10.0f, 0.0f } });
        DrawCube({ { CubeBX, 10.0f, 0.0f }, { 1.0f, 1.0f, 0.2f } });
        DrawCube({ { CubeCX, 10.0f, 0.0f } });

        // Note: Alpha Blending requires the objects to be drawn in order from farthest to nearest
        mSmileyTexture->Bind(0);
        mSmileyTexture->Bind(2);
        DrawCube({ { -5.0f, 1.0f, -8.0f }, { 1.0f, 1.0f, 0.0f } });

        mWindowTexture->Bind(0);
        mWindowTexture->Bind(2);
        DrawCube({ { -1.0f, 1.0f, -8.0f }, { 1.0f, 1.0f, 0.2f } });

        mCommandBuffer->UpdateStencilBuffer();
        DrawCube({ { 0.0f, 1.0f, -9.0f }, { 1.0f, 1.0f, 0.2f } });
        mCommandBuffer->EnableStencilTest();
        DrawCube({ { 0.0f, 1.0f, 9.0f }, { 1.1f, 1.1f, 0.3f } }, true);
        mCommandBuffer->ResetStencilTest();
    }

    void DrawCube(const Components::Transform &transform, bool stencil = false) {
        // Define Cube Model
        static Model cube("Assets/Models/Cube/Cube.obj");
        
        // Bind Shader
        if (stencil) { mStencilOutlineShader->Bind(); } else { mModelShader->Bind(); }

        // Material Tests
        MaterialData materialData {
            .Ambient { 0.1f, 0.1f, 0.1f },
            .Diffuse { 0.5f, 0.5f, 0.5f },
            .Specular { 1.0f, 1.0f, 1.0f },
            .Shininess { 32.0f }
        };
        mMaterialBuffer->UpdateData(&materialData, sizeof(Components::Material));
        mMaterialBuffer->Bind(9);

        // Update Entity Data
        Components::EntityData entityData;
        entityData.Transform = transform;
        mEntityUniformBuffer->UpdateData(&entityData, sizeof(Components::EntityData));
        mEntityUniformBuffer->Bind((size_t)UniformPosition::EntityData);

        cube.Draw(mCommandBuffer);

        // Visualize Normals
        //mNormalsShader->Bind();
        //cube.Draw(mCommandBuffer);
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

        // Update Entity Data
        mLightPosition = glm::vec3(-3.0f, 3.0f, 5.0f);
        static float timeValue = 0.1f;
        timeValue += (float)deltaTime;
        float angle = timeValue * 2.0f;
        float radius = 5.0f;
        mLightPosition.x = 1.0f + radius * std::cos(angle);
        mLightPosition.z = 3.0f + radius * std::sin(angle);

        float value = (sin(timeValue) / 2.0f) + 0.5f;
        mLightData.Color = glm::vec4(1.0f, 1.0f - value, value, 1.0f);
        auto model = glm::translate(glm::mat4(1.0f), mLightPosition) *
            glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
        mLightData.Transform = model;

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
            .LightColor = mLightData.Color,
            .LightPosition = mLightPosition,

            .Constant = 1.0f,
            .Linear = 0.14f,
            .Quadratic = 0.07f,
        };
        mLights.Light[2] = {
            .Type = Components::LightType::Point,
            .LightColor = { 1.0f, 1.0f, 1.0f },
            .LightPosition = { 7.0f, 5.0f, -9.0f },

            .Constant = 1.0f,
            .Linear = 0.14f,
            .Quadratic = 0.07f,
        };
        // - Spot Light
        mLights.Light[3] = {
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
        mLights.Count = 4;

        // Draw

        vertexBuffer->Bind();
        pipeline->Bind();
        indexBuffer->Bind();

        mLightShader->Bind();
        mLightBuffer->Bind(6);
        mLightBuffer->UpdateData(&mLights, sizeof(Components::Lights));
        mEntityUniformBuffer->UpdateData(&mLightData, sizeof(Components::EntityData));
        mEntityUniformBuffer->Bind((size_t)UniformPosition::EntityData);

        mCommandBuffer->DrawIndexed(cube.Components, PrimitiveType::Triangle, true);
    }

    void DrawModel(Model &model, const glm::vec3 &position, const glm::vec3 &size = { 1.0f, 1.0f, 1.0f }) {
        // Bind Shader
        //mDebugDepthShader->Bind();
        mModelShader->Bind();
        
        // Update Entity Data
        Components::EntityData entityData;
        entityData.Transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
        mEntityUniformBuffer->UpdateData(&entityData, sizeof(Components::EntityData));
        mEntityUniformBuffer->Bind((size_t)UniformPosition::EntityData);

        model.Draw(mCommandBuffer);

        // Visualize Normals
        //mNormalsShader->Bind();
        //model.Draw(mCommandBuffer);
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
            .SamplerWrap = TextureWrap::Clamp,
        };
        static auto skyboxTexture = Texture::Create(skyboxProperties, "Assets/Textures/Skybox/Sinister");
        static auto vertexBuffer = Buffer::Create(BufferType::Vertex, &skybox.Vertices, sizeof(Components::Skybox::Vertices));
        static auto indexBuffer = Buffer::Create(BufferType::Index, &skybox.Indices, sizeof(Components::Skybox::Indices));

        // Draw
        vertexBuffer->Bind();
        pipeline->Bind();
        indexBuffer->Bind();
        mSkyBoxShader->Bind();
        skyboxTexture->Bind(0);
        mCommandBuffer->DrawIndexed(skybox.Components, PrimitiveType::Triangle, false);
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
    // Properties
    bool mUIActive = false;

    // Objects
    Scope<Renderer> mRenderer;
    CommandBuffer *mCommandBuffer;
    DesignerCamera mDesignerCamera;

    // Point Light Position
    glm::vec3 mLightPosition {};

    // Buffers
    Reference<Buffer> mCameraUniformBuffer;
    Reference<Buffer> mEntityUniformBuffer;
    Reference<Buffer> mLightBuffer;
    Reference<Buffer> mMaterialBuffer;

    // Components
    Components::Camera mCamera;
    Components::EntityData mLightData;
    Components::Lights mLights;
    Components::Transform mTransform;

    // Shaders
    Reference<Shader> mDebugDepthShader;
    Reference<Shader> mGridShader;
    Reference<Shader> mLightShader;
    Reference<Shader> mModelShader;
    Reference<Shader> mNormalsShader;
    Reference<Shader> mSkyBoxShader;
    Reference<Shader> mStencilOutlineShader;
    
    // Textures
    Reference<Texture> mCheckerBoard;
    Reference<Texture> mConcreteTexture;
    Reference<Texture> mGrassTexture;
    Reference<Texture> mMarbleTexture;
    Reference<Texture> mMatrixTexture;
    Reference<Texture> mMetalTexture;
    Reference<Texture> mSmileyTexture;
    Reference<Texture> mWindowTexture;
    Reference<Texture> mWoodTexture;
};

}
