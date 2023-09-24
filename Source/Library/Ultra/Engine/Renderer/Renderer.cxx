module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>

#pragma warning(push, 0)
//https://github.com/nothings/stb/issues/334
#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#pragma warning(pop)

module Ultra.Engine.Renderer;

import Ultra.GFX.Context;
import Ultra.Platform.DXRenderer;
import Ultra.Platform.GLRenderer;
import Ultra.Platform.VKRenderer;
import Ultra.Platform.Renderer.DXRenderDevice;
import Ultra.Platform.Renderer.GLRenderDevice;
import Ultra.Platform.Renderer.VKRenderDevice;

import Ultra.Engine.Renderer.Buffer;
import Ultra.Engine.Renderer.Texture;

namespace Ultra {

Scope<Renderer> Renderer::Create() {
    Scope<Renderer> renderer;
    Scope<RenderDevice> device;

    auto api = Context::GetAPI();
    switch (api) {
        case GraphicsAPI::DirectX: {
            renderer = CreateScope<DXRenderer>();
            device = CreateScope<DXRenderDevice>();
            break;
        }
        case GraphicsAPI::OpenGL: {
            renderer = CreateScope<GLRenderer>();
            device = CreateScope<GLRenderDevice>();
            break;
        }
        case GraphicsAPI::Vulkan: {
            renderer = CreateScope<VKRenderer>();
            device = CreateScope<VKRenderDevice>();
            break;
        }
        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer: RenderAPI not supported!");
        #else
            LogFatal("RenderAPI not supported!");
        #endif
        }
    }
    renderer->mRenderDevice = std::move(device);
    renderer->Load();
    return renderer;
}

void Renderer::Load() {
    mRenderDevice->Load();
    mRenderDevice->SetLineThickness(3.0f);
    mCommandBuffer = CommandBuffer::Create();
    mCommandBuffer->SetViewport(0, 0, 1280, 1024);
    Renderer2D::Load();
}

void Renderer::RenderFrame() {
    mCommandBuffer->Clear(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer2D::ResetStatistics();
}

void Renderer::Dispose() {
    Renderer2D::Dispose();
    mRenderDevice->Dispose();
}

struct CameraData {
    glm::mat4 ViewProjection = {};
    glm::mat4 Projection = {};
    glm::mat4 View = {};
    float NearClip = {};
    float FarClip = {};
};

void Renderer::DrawGrid(const DesignerCamera &camera) {
    static unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
    static float vertices[] = {
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
    };
    static CameraData cameraUniformData;

    static Reference<Shader> BasicShader = Shader::Create("Assets/Shaders/Grid.glsl");
    static Reference<PipelineState> BasicPipeline = PipelineState::Create({
        .Layout = {
            { ShaderDataType::Float3, "aPosition" },
        }
    });
    static Reference<Buffer> BasicVertex = Buffer::Create(BufferType::Vertex, vertices, sizeof(vertices));
    static Reference<Buffer> BasicIndex = Buffer::Create(BufferType::Index, indices, sizeof(indices));
    static Reference<Buffer> BasicUniform = Buffer::Create(BufferType::Uniform, nullptr, sizeof(CameraData));
    BasicUniform->Bind(0);

    // Renderer
    BasicShader->Bind();
    cameraUniformData.Projection = camera.GetProjection();
    cameraUniformData.View = camera.GetViewMatrix();
    cameraUniformData.ViewProjection = camera.GetViewProjection();
    cameraUniformData.NearClip = camera.GetNearPoint();
    cameraUniformData.FarClip = camera.GetFarPoint();
    BasicUniform->UpdateData(&cameraUniformData, sizeof(CameraData));
    BasicVertex->Bind();
    BasicPipeline->Bind();
    BasicIndex->Bind();
    mCommandBuffer->DrawIndexed(6, PrimitiveType::Triangle, true);
}

#pragma region Test

#define TEST_ABSTRACT_RENDER_CALLS 0

// Components
struct Components {
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
} static sComponents;


static void TestGL();

///
/// @brief Test function for OpenGL raw usage.
/// This function is a quick start guide for OpenGL usage without any abstraction.
///
static void TestGLRaw();

void Renderer::Test() {
#if TEST_ABSTRACT_RENDER_CALLS
    TestGL();
#else
    TestGLRaw();
#endif
}

void TestGL() {
    // Load shaders, buffers, textures
    static auto linkedShaders = Shader::Create("Assets/Shaders/Test.glsl");
    //static auto vertexBuffer = Buffer::Create(BufferType::VertexBuffer, vertices, vertexCount);
    //static auto indexBuffer = Buffer::Create(BufferType::IndexBuffer, indices, indexCount);
    //auto linkedShaders = Shader::Create("Assets/Shaders/Sample.glsl");
    //auto texture = Texture::Create(TextureType::Texture2D, "path/to/texture.png");

    //// Create render states
    //auto renderState = RenderState::Create();
    

    // Begin recording commands
    //commandBuffer->Begin();
    //commandBuffer->Clear(0.2f, 0.3f, 0.3f, 1.0f);     // Clear the framebuffer
    //commandBuffer->BindRenderState(renderState);      // Set up the render state

    // Bind shaders, buffers, textures
    //    commandBuffer->BindShader(vertexShader);
    //    commandBuffer->BindShader(fragmentShader);
    //    commandBuffer->BindVertexBuffer(vertexBuffer);
    //    commandBuffer->BindIndexBuffer(indexBuffer);
    //    commandBuffer->BindTexture(0, texture);
    //    commandBuffer->DrawIndexed(indexCount);           // Draw the mesh

        //Renderer::EndScene();

    //    commandBuffer->End();                             // End recording commands
    //    commandBuffer->Execute();                         // Execute the command buffer
    //    swapchain->Present();                             // Present the rendered image to the screen

}

void TestGLRaw() {
    // Shader Source Code
    #pragma region Shader Source Code
    static auto vertexShaderSource = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec4 aColor;
        layout (location = 2) in vec2 aTexCoord;

        out vec4 oColor;
        out vec2 oTexCoord;

        uniform mat4 transform;

        void main() {
            oColor = aColor;
            oTexCoord = aTexCoord;

            gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";

    static auto fragmentShaderSource = R"(
        #version 330 core

        in vec4 oColor;
        in vec2 oTexCoord;
        
        uniform vec4 ourColor;
        uniform sampler2D ourTexture;

        out vec4 FragColor;

        void main() {
            // mix(a, b, percent from a)
            FragColor = texture(ourTexture, oTexCoord) * ourColor;// * oColor;
        } 
    )";
    #pragma endregion

    // Build and Compile Shaders
    auto checkProgramState = [](unsigned int id) -> bool {
        vector<char> message(1024);
        int result {};
        glGetProgramiv(id, GL_LINK_STATUS, &result);
        if (!result) {
            glGetProgramInfoLog(id, 1024, nullptr, message.data());
            Log("Error: Shader link failed: {0}", message.data());
            return false;
        };
        return true;
    };
    auto checkShaderState = [](unsigned int id) -> bool {
        vector<char> message(1024);
        int result {};
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (!result) {
            glGetShaderInfoLog(id, 1024, nullptr, message.data());
            Log("Error: Shader compilation failed: {0}", message.data());
            return false;
        };
        return true;
    };
    static unsigned int vertexShader {};
    if (!vertexShader) {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        if (!checkShaderState(vertexShader)) return;
    }
    static unsigned int fragmentShader {};
    if (!fragmentShader) {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        if (!checkShaderState(fragmentShader)) return;
    }
    static unsigned int shaderProgram {};
    if (!shaderProgram) {
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if (!checkProgramState(shaderProgram)) return;
    };

    // Create and Bind Vertex Array Object
    // Note: Must be done before binding VBO, 'cause VAO captures and stores the vertex attribute configuration.
    static unsigned int vertexArrayObject {};
    if (!vertexArrayObject) {
        glGenVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);
    }

    // Create and Bind Vertex Buffer Object
    static unsigned int vertexBufferObject {};
    if (!vertexBufferObject) {
        glGenBuffers(1, &vertexBufferObject);

        // Copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(sComponents.TriangleVertices), (void *)sComponents.TriangleVertices, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sComponents.RectangleVertices), (void *)sComponents.RectangleVertices, GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Color
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture Coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    // Create and Bind Element Buffer Object
    static unsigned int elementBufferObject {};
    if (!elementBufferObject) {
        glGenBuffers(1, &elementBufferObject);

        // Copy our indices array in a element buffer for OpenGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sComponents.TriangleIndices), (void *)sComponents.TriangleIndices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sComponents.RectangleIndices), (void *)sComponents.RectangleIndices, GL_STATIC_DRAW);
    }

    // Create and Bind Texture
    static unsigned int texture {};
    if (!texture) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // load and generate the texture
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load("Assets/Textures/Wallpaper2.png", &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            LogError("Failed to load texture!");
        }
        stbi_image_free(data);
    }

    // Update Unfiorms;
    glUseProgram(shaderProgram);

    static int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    static int vertexTextureLocation = glGetUniformLocation(shaderProgram, "ourTexture");

    static auto timeValue = 0.1f;
    timeValue += 0.0001f;
    float value = (sin(timeValue) / 2.0f) + 0.5f;
    glUniform4f(vertexColorLocation, 0.1f, 1.0f - value, value, 1.0f);

    // Scale, Rotation and Transformation
    static float speed = 0.0f;
    speed += 0.01f;
    if (speed > 360.0f) speed = 0.0f;
    static unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, glm::radians(speed), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.52f, 0.52f, 0.52f));

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


    // Draw the triangle
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vertexArrayObject);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe: On
    //glDrawElements(GL_TRIANGLES, sComponents.TriangleComponents, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, sComponents.RectangleComponents, GL_UNSIGNED_INT, 0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Wireframe: Off
    glBindVertexArray(0);

    // Optional: De-allocate all resources once they've outlived their purpose...
    //glDeleteVertexArrays(1, &vertexArrayObject);
    //glDeleteBuffers(1, &vertexBufferObject);
    //glDeleteProgram(shaderProgram);
}

#pragma endregion

}
