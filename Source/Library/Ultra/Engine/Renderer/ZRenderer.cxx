module;

#include <glad/gl.h>

module Ultra.Engine.Renderer;

import Ultra.Engine.RenderCommand;

namespace Ultra {

Renderer::SceneData *Renderer::mSceneData = new Renderer::SceneData;

struct RendererData {
    //Reference<RenderPass> ActiveRenderPass;
};

void Renderer::Load() {
    RenderCommand::Load();
    Renderer2D::Load();
}

void Renderer::BeginScene() {
    RenderCommand::Clear();
    Renderer2D::ResetStatistics();
    //mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {
}

void Renderer::Submit(const IndexProperties &properties, PrimitiveType type, bool depthTest) {
    RenderCommand::DrawIndexed(properties, type, depthTest);
}

void Renderer::Unload() {
    Renderer2D::Unload();
}

void Renderer::SetClearColor(float red, float green, float blue, float alpha) {
    RenderCommand::SetClearColor({ red, green, blue, alpha });
}

void Renderer::Resize(const uint32_t width, const uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

template<typename T>
size_t vectorsizeof(const typename std::vector<T> &vec) {
    return sizeof(T) * vec.size();
}

void Renderer::Test() {

    static Reference<Pipeline> BasicPipeline;
    static Reference<ZShader> BasicShader;
    static Reference<IndexBuffer> BasicIndex;
    static Reference<VertexBuffer> BasicVertex;


    BasicShader = ZShader::Create("Assets/Shaders/Sample.glsl");
    BasicPipeline = Pipeline::Create({
        VertexBufferLayout {
            { ShaderDataType::Float3, "position" }
        }
    });

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    BasicVertex = VertexBuffer::Create(vertices, sizeof(vertices));

    uint32_t indices[] = { 0, 1, 2 };
    BasicIndex = IndexBuffer::Create(indices, sizeof(indices));

    // Renderer
    BasicShader->Bind();
    BasicVertex->Bind();
    BasicPipeline->Bind();
    BasicIndex->Bind();
    Renderer::Submit(BasicIndex->GetProperties());

    return;

#ifdef BASIC_OPENGL_WORKFLOW
    // Data: Vertices and Indices
    auto vertices = std::to_array<float>({
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f
    });
    auto indices = std::to_array<uint32_t>({
        0u, 1u, 2u
    });

    // IndexBuffer
    uint32_t ib;
    glCreateBuffers(1, &ib);
    glNamedBufferData(ib, sizeof(indices), indices.data(), GL_STATIC_DRAW);

    // VertexBuffer
    uint32_t vb;
    glCreateBuffers(1, &vb);
    glNamedBufferData(vb, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    // Pipeline (VertexArray)
    glBindBuffer(GL_ARRAY_BUFFER, vb);

    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

    //glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Draw the triangle using glDrawElements
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
#endif
}

}
