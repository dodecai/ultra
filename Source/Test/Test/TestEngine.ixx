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

struct CameraData {
    //glm::mat4 ViewProjection = {};
    glm::mat4 View = {};
    glm::mat4 Projection = {};
    float NearClip = {};
    float FarClip = {};
};

enum class LightType: uint32_t {
    Directional = 0,
    Point       = 1,
    Spot        = 2,
    Flash       = 3,
};

float DegreesToRadians(float degrees) {
    return degrees * static_cast<float>(std::numbers::pi) / 180.0f;
}

namespace ComponentsTest {

struct Transform {
    Vector3 Rotation = {};
    Vector3 Scale = {};
    Vector3 Translation = {};

    operator Matrix4() const {
        Matrix4 data = Matrix4::Identity();
        data.Translate(Translation);
        data.Rotate(Quaternion(Rotation));
        data.Scale(Scale);
        return data;
    }
};

}

struct Components {
    float CubeVertices[24] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };
    unsigned int CubeIndices[36] = {
         0,  1,  2,  2,  3,  0, // Front
         4,  5,  6,  6,  7,  4, // Back
         3,  2,  6,  6,  7,  3, // Top
         0,  1,  5,  5,  4,  0, // Bottom
         0,  3,  7,  7,  4,  0, // Left
         1,  2,  6,  6,  5,  1, // Right
    };
    int CubeComponents = 36;

    float SkyboxVertices[24] = {
        // Vorderseite
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,

        // Rückseite
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
    };
    unsigned int SkyboxIndices[36] = {
        // Vorderseite
        0, 1, 2,
        2, 3, 0,

        // Rückseite
        4, 5, 6,
        6, 7, 4,

        // Links
        0, 3, 7,
        7, 4, 0,

        // Rechts
        1, 2, 6,
        6, 5, 1,

        // Oben
        3, 2, 6,
        6, 7, 3,

        // Unten
        0, 1, 5,
        5, 4, 0,
    };
    int SkyboxComponents = 36;


    float TriangleVertices[27] = {
        // Positions            // Colors                   // Texture Coords
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 0.0f,         // Left
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,     1.0f, 0.0f,         // Right
         0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,     0.5f, 1.0f,         // Top
    };
    unsigned int TriangleIndices[3] = {
        0, 1, 2,
    };
    int TriangleComponents = 3;

    float RectangleVertices[36] = {
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
    unsigned int RectangleIndices[6] = {
        0, 1, 3,    // First Triangle
        1, 2, 3,    // Second Triangle
    };
    int RectangleComponents = 6;

    float TriangleTexCoords[6] = {
        // Triangle
        0.0f, 0.0f,  // Lower-Left corner  
        1.0f, 0.0f,  // Lower-Right corner
        0.5f, 1.0f   // Top-Center corner
    };

    struct CameraData {
        glm::mat4 ViewProjectionMatrix = {};
        glm::mat4 Projection = {};
        glm::mat4 View = {};
        float NearClip = {};
        float FarClip = {};
    } Camera;
    Reference<Buffer> CameraUniformBuffer;

    struct CameraData2 {
        glm::mat4 Model = {};
        glm::mat4 View = {};
        glm::mat4 Projection = {};
    } Camera2;
    Reference<Buffer> CameraUniformBuffer2;

    struct UProperties {
        glm::vec4 Color = glm::vec4(1.0f);
    } Properties;

    struct UMaterial {
        glm::vec3 uDiffuseColor {};
        glm::vec3 uSpecularColor {};
        glm::vec3 uAmbientColor {};
        float Shininess;
    } Material;
    Reference<Buffer> MaterialBuffer;

    struct ULight {
        alignas(16) LightType Type = LightType::Directional;
        alignas(16) glm::vec3 LightColor {};
        alignas(16) glm::vec3 LightPosition {};
        alignas(16) glm::vec3 LightDirection {};
        alignas(16) glm::vec3 Ambient {};
        alignas(16) glm::vec3 Diffuse {};
        alignas(16) glm::vec3 Specular {};

        // Attenuation
        float Constant;
        float Linear;
        float Quadratic;

        // Spot Light
        float CutOffAngle;
    };
    struct ULights {
        ULight Light[5] {};
        alignas(16) uint32_t Count = 1;
    } Lights;
    Reference<Buffer> LightBuffer;

    struct UView {
        glm::vec3 Position;
    } View;
    Reference<Buffer> ViewBuffer;
};

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
        mLightShader = Shader::Create("Assets/Shaders/Light.glsl");
        mModelShader = Shader::Create("Assets/Shaders/Material.Blinn-Phong.glsl");
        mSkyBoxShader = Shader::Create("Assets/Shaders/SkyBox.glsl");
        mStencilOutlineShader = Shader::Create("Assets/Shaders/Stencil.Outline.glsl");

        // Load Buffers
        sComponents.CameraUniformBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::CameraData));
        sComponents.CameraUniformBuffer2 = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::CameraData2));
        sComponents.MaterialBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::UMaterial));
        sComponents.LightBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::ULights));
        sComponents.ViewBuffer = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Components::UView));

    }
    ~Engine() = default;

    void Test(Timestamp deltaTime) {
        mRenderer->RenderFrame();
        mDesignerCamera.Update(deltaTime);

        sComponents.Camera2.Projection = mDesignerCamera.GetProjectionMatrix();
        sComponents.Camera2.View = mDesignerCamera.GetViewMatrix();
        sComponents.Camera2.Model = glm::mat4(1.0f);

        // Mesh Renderer (3D)
        #if TEST_MESH_RENDERER == 1
            TestMeshRenderer(deltaTime, mDesignerCamera);
        #endif
    
        // Sprite Renderer (2D)
        #if TEST_SPRITE_RENDERER == 1
            TestSpriteRenderer(deltaTime, mDesignerCamera);
        #endif
    
        // UI Renderer (2D)
        #if TEST_UI_RENDERER == 1
            TestUIRenderer(deltaTime, mDesignerCamera);
        #endif
    }

    #pragma region Mesh Renderer

    void TestMeshRenderer(Timestamp deltaTime, const DesignerCamera &camera) {
        // Draw Scene
        //DrawGrid(mDesignerCamera);
        DrawSkybox(deltaTime, camera);
        DrawLevel(deltaTime, camera);
        DrawLights(deltaTime, camera);
    }

    void DrawGrid(const DesignerCamera &camera) {
        //static unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };
        //static float vertices[] = {
        //     1.0f,  1.0f,  0.0f,
        //     1.0f, -1.0f,  0.0f,
        //    -1.0f, -1.0f,  0.0f,
        //    -1.0f,  1.0f,  0.0f,
        //};

        //static Reference<Shader> BasicShader = Shader::Create("Assets/Shaders/Grid.glsl");
        //static PipelineProperties properties;
        //properties.BlendMode = BlendMode::Alpha;
        //properties.DepthTest = true;
        //properties.Wireframe = false;
        //properties.Layout = {
        //    { ShaderDataType::Float3, "aPosition" }
        //};
        //static Reference<PipelineState> BasicPipeline = PipelineState::Create(properties);
        //static Reference<Buffer> BasicVertex = Buffer::Create(BufferType::Vertex, vertices, sizeof(vertices));
        //static Reference<Buffer> BasicIndex = Buffer::Create(BufferType::Index, indices, sizeof(indices));
        //static Reference<Buffer> BasicUniform = Buffer::Create(BufferType::Uniform, nullptr, sizeof(CameraData));

        //BasicVertex->UpdateData(vertices, sizeof(vertices));
        //BasicIndex->UpdateData(indices, sizeof(indices));

        //static CameraData cameraUniformData;
        ////cameraUniformData.ViewProjection = camera.GetViewProjection();
        //cameraUniformData.View = camera.GetViewMatrix();
        //cameraUniformData.Projection = camera.GetProjection();
        //cameraUniformData.NearClip = camera.GetNearPoint();
        //cameraUniformData.FarClip = camera.GetFarPoint();
        //BasicUniform->UpdateData(&cameraUniformData, sizeof(CameraData));

        //// Renderer
        //BasicVertex->Bind();
        //BasicPipeline->Bind();
        //BasicIndex->Bind();
        //BasicShader->Bind();
        //BasicUniform->Bind(0);
        //mCommandBuffer->DrawIndexed(6, PrimitiveType::Triangle, true);
    }

    void DrawSkybox(Timestamp deltaTime, const DesignerCamera &camera) {
        // Prepare
        auto &projection = camera.GetProjectionMatrix();
        auto &view = camera.GetViewMatrix();
        auto model = glm::mat4(1.0f);

        // Skybox
        TextureProperties skyboxProperties;
        skyboxProperties.Dimension = TextureDimension::TextureCube;
        static auto skyboxTexture = Texture::Create(skyboxProperties, "Assets/Textures/Skybox/Light Blue");

        // Specify Pipeline and Shader
        static PipelineProperties properties;
        properties.DepthTest = true;
        properties.Wireframe = false;
        properties.Layout = {
            { ShaderDataType::Float3, "aPosition" }
        };
        static auto pipeline = PipelineState::Create(properties);

        // Specify Buffers and Textures
        static auto vertexBuffer = Buffer::Create(BufferType::Vertex, &sComponents.SkyboxVertices, sizeof(sComponents.SkyboxVertices));
        static auto indexBuffer = Buffer::Create(BufferType::Index, &sComponents.SkyboxIndices, sizeof(sComponents.SkyboxIndices));
        
        // Update Vertices and Indices
        vertexBuffer->UpdateData(&sComponents.SkyboxVertices, sizeof(sComponents.SkyboxVertices));
        indexBuffer->UpdateData(&sComponents.SkyboxIndices, sizeof(sComponents.SkyboxIndices));

        // Update Transforms
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
        sComponents.Camera2.View = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        //sComponents.Camera2.Model = model;

        // Draw
        vertexBuffer->Bind();
        pipeline->Bind();
        indexBuffer->Bind();

        mSkyBoxShader->Bind();
        skyboxTexture->Bind(0);
        sComponents.CameraUniformBuffer2->Bind(0);
        sComponents.CameraUniformBuffer2->UpdateData(&sComponents.Camera2, sizeof(Components::CameraData2));
        mCommandBuffer->DrawIndexed(sComponents.SkyboxComponents, PrimitiveType::Triangle, false);
    }

    void DrawLevel(Timestamp deltaTime, const DesignerCamera &camera) {
        // Prepare
        auto &projection = camera.GetProjectionMatrix();
        auto &view = camera.GetViewMatrix();
        auto model = glm::mat4(1.0f);

        // Load Models
        static Model cone("Assets/Models/Cone/Cone.obj");
        static Model cube("Assets/Models/Cube/Cube.obj");
        static Model cylinder("Assets/Models/Cylinder/Cylinder.obj");
        static Model level("Assets/Models/Level/Level.obj");
        static Model monkey("Assets/Models/Monkey/Monkey.obj");
        static Model plane("Assets/Models/Plane/Plane.obj");
        static Model sphere("Assets/Models/Sphere/SphereUV.obj");
        static Model torus("Assets/Models/Torus/Torus.obj");
        static Model test("Assets/Models/Test/Test.obj");

        // Translate Cube Model
        auto cubeModel = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        cubeModel = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        // Update Camera, Light and View
        mModelShader->Bind();
        //mDebugDepthShader->Bind();
        sComponents.Camera2.Projection = mDesignerCamera.GetProjectionMatrix();
        sComponents.Camera2.View = mDesignerCamera.GetViewMatrix();
        sComponents.Camera2.Model = cubeModel;
        sComponents.View.Position = camera.GetPosition();

        sComponents.Material.Shininess = 16.0f;

        // Directional Light
        sComponents.Lights.Light[1].Type = LightType::Directional;
        sComponents.Lights.Light[0].LightColor = { 1.0f, 1.0f, 1.0f };
        sComponents.Lights.Light[0].LightDirection = { 0.0f, -128.0f, -64.0f };

        sComponents.Lights.Light[0].Ambient = { 0.1f, 0.1f, 0.1f };
        sComponents.Lights.Light[0].Diffuse = { 0.5f, 0.5f, 0.5f };
        sComponents.Lights.Light[0].Specular = { 1.0f, 1.0f, 1.0f };

        // Point Light
        sComponents.Lights.Light[1].Type = LightType::Point;
        sComponents.Lights.Light[1].LightColor = sComponents.Properties.Color;
        sComponents.Lights.Light[1].LightPosition = mLightPosition;

        sComponents.Lights.Light[1].Ambient = { 0.1f, 0.1f, 0.1f };
        sComponents.Lights.Light[1].Diffuse = { 0.5f, 0.5f, 0.5f };
        sComponents.Lights.Light[1].Specular = { 1.0f, 1.0f, 1.0f };

        sComponents.Lights.Light[1].Constant = 1.0f;
        sComponents.Lights.Light[1].Linear = 0.14f;
        sComponents.Lights.Light[1].Quadratic = 0.07f;

        // Spot Light
        sComponents.Lights.Light[2].Type = LightType::Spot;
        sComponents.Lights.Light[2].LightColor = { 1.0f, 1.0f, 1.0f };
        sComponents.Lights.Light[2].LightPosition = camera.GetPosition();
        sComponents.Lights.Light[2].LightDirection = camera.GetForwardDirection();

        sComponents.Lights.Light[2].Ambient = { 0.1f, 0.1f, 0.1f };
        sComponents.Lights.Light[2].Diffuse = { 0.5f, 0.5f, 0.5f };
        sComponents.Lights.Light[2].Specular = { 1.0f, 1.0f, 1.0f };

        sComponents.Lights.Light[2].Constant = 1.0f;
        sComponents.Lights.Light[2].Linear = 0.14f;
        sComponents.Lights.Light[2].Quadratic = 0.07f;

        sComponents.Lights.Light[2].CutOffAngle = glm::cos(glm::radians(12.5f));

        // Finish Light
        sComponents.Lights.Count = 3;

        // ToDo: Compiler error...
        // Texture Test
        //static MaterialData materialData {
        //    .Shininess = 32.0f * 256.0f
        //};
        //static auto materialBuffer = Buffer::Create(BufferType::Uniform, &materialData, sizeof(MaterialData));
        static auto diffuseMapTexture = Texture::Create({}, "Assets/Models/Test/diffuse-map.png");
        static auto specularMapTexture = Texture::Create({}, "Assets/Models/Test/specular-map.png");

        //materialBuffer->UpdateData(&materialData, sizeof(MaterialData));

        //materialBuffer->Bind(0);
        diffuseMapTexture->Bind(0);
        specularMapTexture->Bind(2);

        sComponents.CameraUniformBuffer2->Bind(0);
        sComponents.ViewBuffer->Bind(4);
        //sComponents.MaterialBuffer->Bind(5);
        sComponents.LightBuffer->Bind(6);
        sComponents.CameraUniformBuffer2->UpdateData(&sComponents.Camera2, sizeof(Components::CameraData2));
        //sComponents.MaterialBuffer->UpdateData(&sComponents.Material, sizeof(Components::UMaterial));
        sComponents.LightBuffer->UpdateData(&sComponents.Lights, sizeof(Components::ULights));
        sComponents.ViewBuffer->UpdateData(&sComponents.View, sizeof(Components::UView));

        test.Draw(mCommandBuffer);
    }

    void DrawLights(Timestamp deltaTime, const DesignerCamera &camera) {
        // Prepare
        auto &projection = camera.GetProjectionMatrix();
        auto &view = camera.GetViewMatrix();
        auto model = glm::mat4(1.0f);

        static float timeValue = 0.1f;
        timeValue += (float)deltaTime /2;
        mLightPosition = glm::vec3(-3.0f, 3.0f, 5.0f);
        float radius = 5.0f;
        float angle = timeValue * 2.0f;
        mLightPosition.x = 1.0f + radius * std::cos(angle);
        mLightPosition.z = 3.0f + radius * std::sin(angle);

        // Specify Pipeline and Shader
        static PipelineProperties properties;
        properties.DepthTest = true;
        properties.Wireframe = false;
        properties.Layout = {
            { ShaderDataType::Float3, "aPosition" }
        };
        static auto pipeline = PipelineState::Create(properties);

        // Specify Buffers and Textures
        static auto vertexBuffer = Buffer::Create(BufferType::Vertex, &sComponents.CubeVertices, sizeof(sComponents.CubeVertices));
        static auto indexBuffer = Buffer::Create(BufferType::Index, &sComponents.CubeIndices, sizeof(sComponents.CubeIndices));
        static auto propertiesUniform = Buffer::Create(BufferType::Uniform, &sComponents.Properties, sizeof(sComponents.Properties));
        static auto translationUnfiorm = Buffer::Create(BufferType::Uniform, nullptr, sizeof(Matrix4));

        // Update Vertices and Indices
        vertexBuffer->UpdateData(&sComponents.CubeVertices, sizeof(sComponents.CubeVertices));
        indexBuffer->UpdateData(&sComponents.CubeIndices, sizeof(sComponents.CubeIndices));

        // Update Properties
        float value = (sin(timeValue) / 2.0f) + 0.5f;
        sComponents.Properties.Color = glm::vec4(1.0f, 1.0f - value, value, 1.0f);
        propertiesUniform->UpdateData(&sComponents.Properties, sizeof(sComponents.Properties));

        // Update Transforms
        auto lightModel = glm::translate(glm::mat4(1.0f), mLightPosition);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f, 0.2f, 0.2f));
        auto lightModelStencil = glm::translate(glm::mat4(1.0f), mLightPosition);
        lightModelStencil = glm::scale(lightModelStencil, glm::vec3(0.3f, 0.3f, 0.3f));

        auto lightTransform = projection * view * lightModel;
        auto lightTransformStencil = projection * view * lightModelStencil;

        // Draw
        mCommandBuffer->UpdateStencilBuffer();

        vertexBuffer->Bind();
        pipeline->Bind();
        indexBuffer->Bind();

        mLightShader->Bind();
        propertiesUniform->Bind(0);
        translationUnfiorm->Bind(1);
        translationUnfiorm->UpdateData(&lightTransform, sizeof(Matrix4));
        mCommandBuffer->DrawIndexed(sComponents.CubeComponents, PrimitiveType::Triangle, true);

        mCommandBuffer->EnableStencilTest();

        vertexBuffer->Bind();
        pipeline->Bind();
        indexBuffer->Bind();

        mStencilOutlineShader->Bind();
        propertiesUniform->Bind(0);
        translationUnfiorm->Bind(1);
        translationUnfiorm->UpdateData(&lightTransformStencil, sizeof(Matrix4));
        mCommandBuffer->DrawIndexed(sComponents.CubeComponents, PrimitiveType::Triangle, false);

        mCommandBuffer->ResetStencilTest();
    }

    #pragma endregion

    #pragma region Sprite Renderer
    void TestSpriteRenderer(Timestamp deltaTime, const DesignerCamera &camera) {
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

    void TestUIRenderer(Timestamp deltaTime, const DesignerCamera &camera) {
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

    // Shaders
    Reference<Shader> mDebugDepthShader;
    Reference<Shader> mModelShader;
    Reference<Shader> mLightShader;
    Reference<Shader> mSkyBoxShader;
    Reference<Shader> mStencilOutlineShader;

    static inline Components sComponents {};
};

}
