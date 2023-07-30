module;

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

module Ultra.Engine.UIRenderer;

import Ultra.Engine.Renderer.Buffer;
import Ultra.Engine.Renderer.CommandBuffer;
import Ultra.Engine.Renderer.PipelineState;
import Ultra.Engine.Renderer.Shader;
import Ultra.Engine.Renderer.Texture;

namespace Ultra {

void ClipRect::Activate() {
    if (mCurrentIndex < 0) return;
    auto &current = mRectangle[mCurrentIndex];

    if (current.Enabled) {
        auto properties = mViewport->GetProperties();

        auto x = current.Position.X;
        auto y = current.Position.Y;
        auto width = current.Size.Width;
        auto height = current.Size.Height;
        TransformRectangle(x, y, width, height);

        glEnable(GL_SCISSOR_TEST);
        glScissor(x, properties.Height - (y + height), width, height);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}

void ClipRect::Push(const Position &position, const Size &size) {
    if (mCurrentIndex + 1 >= mStackDepth) return;

    mCurrentIndex++;
    auto &current = mRectangle[mCurrentIndex];
    current = { true, position, size };
    
    Activate();
}

void ClipRect::PushCombined(const Position &position, const Size &size, Viewport *viewport) {
    mViewport = viewport;
    if (mCurrentIndex < 0) {
        Push(position, size);
        return;
    }

    auto &current = mRectangle[mCurrentIndex];
    if (!current.Enabled) return;

    float maxX = position.X + size.Width;
    float maxY = position.Y + size.Height;
    auto x = std::max(position.X, current.Position.X);
    auto y = std::max(position.Y, current.Position.Y);

    Push(position, { std::min(maxX, current.Position.X + current.Size.Width) - x, std::min(maxY, current.Position.Y + current.Size.Height) - y });
}

void ClipRect::PushDisabled() {
    if (!Validate()) return;

    mCurrentIndex++;
    auto &current = mRectangle[mCurrentIndex];
    current.Enabled = false;

    Activate();
}

void ClipRect::PushTransform(const Position &position, const Size &size) {
    if (!Validate()) return;
    mCurrentTransformIndex++;

    auto &transform = mTransform[mCurrentTransformIndex];
    transform.tx = position.X;
    transform.ty = position.Y;
    transform.sx = size.Width;
    transform.sy = size.Height;
    if (mCurrentIndex >= 0) Activate();
}

void ClipRect::Pop() {
    if (!Validate()) return;
    mCurrentIndex--;

    Activate();
}

void ClipRect::PopTransform() {
    if (!Validate()) return;
    mCurrentTransformIndex--;

    if (mCurrentIndex >= 0) Activate();
}

void ClipRect::TransformRectangle(float &x, float &y, float &sx, float &sy) {
    if (mCurrentTransformIndex >= 0) {
        auto &transform = mTransform[mCurrentTransformIndex];
        x = transform.sx * x + transform.tx;
        y = transform.sy * y + transform.ty;
        sx = transform.sx * sx;
        sy = transform.sy * sy;
    }
}

bool ClipRect::Validate() {
    if (mCurrentIndex < 0) {
        LogFatal("Attempting to pop an empty stack");
        return false;
    } else if (mCurrentIndex + 1 >= mStackDepth) {
        LogFatal("Maximum stack depth exceeded");
        return false;
    }
    return true;
}



template<typename T>
size_t sizeof_vector(const typename std::vector<T> &vec) {
    return sizeof(T) * vec.size();
}

struct PanelComponent {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 Size;
    float InnerAlpha;
    float Bevel;
    glm::vec2 TextureCoordinates;
};

struct RectangleComponent {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TextureCoordinates;
};

struct UIRenderData {
    // Panel
    static constexpr uint32_t MaxPanels = 4096;
    static constexpr uint32_t MaxPanelVertices = MaxPanels * 4;
    static constexpr uint32_t MaxPanelIndices = MaxPanels * 6;

    vector<PanelComponent> PanelVertexBufferData;
    array<glm::vec4, 4> PanelVertexPositions;

    Reference<Shader> PanelShader;
    Reference<PipelineState> PanelPipeline;
    Reference<Buffer> PanelVertexBuffer;
    Reference<Buffer> PanelIndexBuffer;

    // Rectangles
    static constexpr uint32_t MaxRectangles = 4096;
    static constexpr uint32_t MaxRectangleVertices = MaxRectangles * 4;
    static constexpr uint32_t MaxRectangleIndices = MaxRectangles * 6;

    vector<RectangleComponent> RectangleVertexBufferData;
    array<glm::vec4, 4> RectangleVertexPositions;

    Reference<Shader> RectangleShader;
    Reference<PipelineState> RectanglePipeline;
    Reference<Buffer> RectangleVertexBuffer;
    Reference<Buffer> RectangleIndexBuffer;
};

static UIRenderData sUIRenderData;

Scope<CommandBuffer> sCommandBuffer = nullptr;

void UIDraw::Load() {
    // Panels
    {
        sUIRenderData.PanelShader = Shader::Create("Assets/Shaders/UI.glsl");
        PipelineProperties pipelineProperties;
        pipelineProperties.BlendMode = BlendMode::Alpha;
        pipelineProperties.DepthTest = true;
        pipelineProperties.Layout = {
            { ShaderDataType::Float3, "aPosition" },
            { ShaderDataType::Float4, "aColor" },
            { ShaderDataType::Float2, "aSize" },
            { ShaderDataType::Float, "aInnerAlpha" },
            { ShaderDataType::Float, "aBevel" },
            { ShaderDataType::Float2, "aTextureCoordinates" },
        };
        sUIRenderData.PanelPipeline = PipelineState::Create(pipelineProperties);

        sUIRenderData.PanelVertexBufferData.reserve(sUIRenderData.MaxPanelVertices);

        sUIRenderData.PanelVertexBuffer = Buffer::Create(BufferType::Vertex, nullptr, sUIRenderData.MaxPanelVertices * sizeof(PanelComponent));
        sUIRenderData.PanelVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        sUIRenderData.PanelVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        sUIRenderData.PanelVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        sUIRenderData.PanelVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        uint32_t offset = 0;
        uint32_t *indices = new uint32_t[sUIRenderData.MaxPanelIndices];
        for (uint32_t i = 0; i < sUIRenderData.MaxPanelIndices; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }

        sUIRenderData.PanelIndexBuffer = Buffer::Create(BufferType::Index, indices, sUIRenderData.MaxPanelIndices * sizeof(uint32_t));
        delete[] indices;

    }

    // Rectangles
    {
        sUIRenderData.RectangleShader = Shader::Create("Assets/Shaders/Rectangle.glsl");
        PipelineProperties pipelineProperties;
        pipelineProperties.BlendMode = BlendMode::Alpha;
        pipelineProperties.DepthTest = true;
        pipelineProperties.Layout = {
            { ShaderDataType::Float3, "aPosition" },
            { ShaderDataType::Float4, "aColor" },
            { ShaderDataType::Float2, "aTextureCoordinates" },
        };
        sUIRenderData.RectanglePipeline = PipelineState::Create(pipelineProperties);

        sUIRenderData.RectangleVertexBufferData.reserve(sUIRenderData.MaxRectangleVertices);

        sUIRenderData.RectangleVertexBuffer = Buffer::Create(BufferType::Vertex, nullptr, sUIRenderData.MaxRectangleVertices * sizeof(RectangleComponent));
        sUIRenderData.RectangleVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        sUIRenderData.RectangleVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        sUIRenderData.RectangleVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        sUIRenderData.RectangleVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        uint32_t offset = 0;
        uint32_t *indices = new uint32_t[sUIRenderData.MaxRectangleIndices];
        for (uint32_t i = 0; i < sUIRenderData.MaxRectangleIndices; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }

        sUIRenderData.RectangleIndexBuffer = Buffer::Create(BufferType::Index, indices, sUIRenderData.MaxRectangleIndices * sizeof(uint32_t));
        delete[] indices;
    }


    sCommandBuffer = CommandBuffer::Create();

    float left = 0.0f;
    float top = 0.0f;
    float right = 1280.0f;
    float bottom = 1024.0f;
    float nearPlane = -1.0f;
    float farPlane = 1.0f;

    ProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
}

void UIDraw::StartFrame() {
    sUIRenderData.PanelVertexBufferData.clear();
    sUIRenderData.RectangleVertexBufferData.clear();
}

void UIDraw::FinishFrame() {
    Flush();
}

void UIDraw::Flush() {
    if (sUIRenderData.PanelVertexBufferData.size()) {
        auto dataSize = sizeof_vector(sUIRenderData.PanelVertexBufferData);
        sUIRenderData.PanelVertexBuffer->UpdateData(sUIRenderData.PanelVertexBufferData.data(), dataSize);

        sUIRenderData.PanelShader->Bind();

        const float pad = 64.0f;
        sUIRenderData.PanelShader->UpdateUniform("uPadding", pad);
        sUIRenderData.PanelShader->UpdateUniform("uProjection", ProjectionMatrix);
        sUIRenderData.PanelShader->UpdateUniform("uModelView", ModelViewMatrix);

        sUIRenderData.PanelVertexBuffer->Bind();
        sUIRenderData.PanelPipeline->Bind();
        sUIRenderData.PanelIndexBuffer->Bind();
        sCommandBuffer->DrawIndexed({ sUIRenderData.PanelVertexBufferData.size() * 6 }, PrimitiveType::Triangle, true);
        sUIRenderData.PanelPipeline->Unbind();
        sUIRenderData.PanelIndexBuffer->Unbind();
        sUIRenderData.PanelVertexBuffer->Unbind();
        sUIRenderData.PanelShader->Unbind();
        //sUIRenderData.Stats.DrawCalls++;
    }

    if (sUIRenderData.RectangleVertexBufferData.size()) {
        auto dataSize = sizeof_vector(sUIRenderData.RectangleVertexBufferData);
        sUIRenderData.RectangleVertexBuffer->UpdateData(sUIRenderData.RectangleVertexBufferData.data(), dataSize);

        sUIRenderData.RectangleShader->Bind();
        sUIRenderData.PanelShader->UpdateUniform("uProjection", ProjectionMatrix);
        sUIRenderData.PanelShader->UpdateUniform("uModelView", ModelViewMatrix);
        //sUIRenderData.PanelShader->UpdateUniform("uViewProjection", sUIRenderData.ViewProjectionMatrix);

        //for (uint32_t i = 0; i < sUIRenderData.TextureSlotIndex; i++) {
        //    sUIRenderData.TextureSlots[i]->Bind(i);
        //}

        sUIRenderData.RectangleVertexBuffer->Bind();
        sUIRenderData.RectanglePipeline->Bind();
        sUIRenderData.RectangleIndexBuffer->Bind();
        sCommandBuffer->DrawIndexed({ sUIRenderData.RectangleVertexBufferData.size() * 6 }, PrimitiveType::Triangle, true);
        sUIRenderData.RectanglePipeline->Unbind();
        sUIRenderData.RectangleVertexBuffer->Unbind();
        sUIRenderData.RectangleIndexBuffer->Unbind();
        sUIRenderData.RectangleShader->Unbind();
        //sUIRenderData.Stats.DrawCalls++;
    }
}

void UIDraw::FlushAndReset() {
    Flush();
    sUIRenderData.PanelVertexBufferData.clear();
    sUIRenderData.RectangleVertexBufferData.clear();
}

void UIDraw::DrawPanel(glm::vec2 position, glm::vec2 size, glm::vec4 color, float innerAlpha, float bevel) {
    if (sUIRenderData.PanelVertexBufferData.size() * 6 >= UIRenderData::MaxPanelIndices) FlushAndReset();

    auto zValue = -0.2f;

    glm::vec4 positions[4] = {
        { position.x, position.y, zValue, 1.0f },                     // bottom-left
        { position.x, position.y + size.y, zValue, 1.0f },            // top-left
        { position.x + size.x, position.y + size.y, zValue, 1.0f },   // top-right
        { position.x + size.x, position.y, zValue, 1.0f }             // bottom-right
    };


    glm::vec2 texCoords[4] = {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f}
    };

    //glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    for (size_t i = 0; i < sUIRenderData.PanelVertexPositions.size(); i++) {
        //auto newPos = transform * sUIRenderData.PanelVertexPositions[i];
        sUIRenderData.PanelVertexBufferData.emplace_back(positions[i], color, size, innerAlpha, bevel, texCoords[i]);
    }

    //sUIRenderData.Stats.Triangles += 2;
    //sUIRenderData.Stats.RectangleCount++;
}

void UIDraw::DrawRectangle(glm::vec2 position, glm::vec2 size, glm::vec4 color) {
    if (sUIRenderData.RectangleVertexBufferData.size() * 6 >= UIRenderData::MaxRectangleIndices) FlushAndReset();

    auto zValue = 0.0f;

    glm::vec4 positions[4] = {
        { position.x, position.y, zValue, 1.0f },                     // bottom-left
        { position.x, position.y + size.y, zValue, 1.0f },            // top-left
        { position.x + size.x, position.y + size.y, zValue, 1.0f },   // top-right
        { position.x + size.x, position.y, zValue, 1.0f }             // bottom-right
    };

    glm::vec2 texCoords[4] = { {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

    //glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    for (size_t i = 0; i < sUIRenderData.RectangleVertexPositions.size(); i++) {
        sUIRenderData.RectangleVertexBufferData.emplace_back((positions[i] * sUIRenderData.RectangleVertexPositions[i]), color, texCoords[i]);
    }

    //sUIRenderData.Stats.Triangles += 2;
    //sUIRenderData.Stats.RectangleCount++;
}

void UIDraw::DrawText(glm::vec2 position, glm::vec2 size, glm::vec4 color, glm::vec2 uv1, glm::vec2 uv2) {
    //if (sUIRenderData.QuadVertexBufferData.size() * 6 >= UIRenderData::MaxQuadIndices) FlushAndReset();

    //glm::vec2 texCoords[4] = { {uv1.x, uv1.y}, {uv1.x, uv2.y}, {uv2.x, uv2.y}, {uv2.x, uv1.y} };

    //glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    //for (size_t i = 0; i < sUIRenderData.QuadVertexPositions.size(); i++) {
    //    auto newPos = transform * sUIRenderData.QuadVertexPositions[i];
    //    sUIRenderData.QuadVertexBufferData.emplace_back(newPos, color, size, 1.0f, 0.0f, texCoords[i]);
    //}

    //glBindTexture(GL_TEXTURE_2D, textureID);
}



void UIDraw::DrawBorder(float s, float x, float y, float w, float h) {
    DrawRect(x, y, w, s);
    DrawRect(x, y + h - s, w, s);
    DrawRect(x, y + s, s, h - 2 * s);
    DrawRect(x + w - s, y + s, s, h - 2 * s);
    //DrawRectangle({ position.x, position.y }, { size.x, s }, color);
    //DrawRectangle({ position.x, position.y + size.y - s }, { size.x, s }, color);
    //DrawRectangle({ position.x, position.y + s }, { s, size.y - 2 * s }, color);
    //DrawRectangle({ position.x + size.x - s , position.y + s }, { s, size.y - 2 * s }, color);
}

void UIDraw::DrawColor(float r, float g, float b, float a) {
    float alpha = 1.0f;
    Vector4Df color = { 1, 1, 1, 1 };
    color = { r, g, b, a };
    glColor4f(r, g, b, a * alpha);
}

void UIDraw::DrawRect(float x1, float y1, float xs, float ys) {
    float x2 = x1 + xs;
    float y2 = y1 + ys;
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x1, y1);
    glTexCoord2f(0, 1); glVertex2f(x1, y2);
    glTexCoord2f(1, 1); glVertex2f(x2, y2);
    glTexCoord2f(1, 0); glVertex2f(x2, y1);
    glEnd();
}

}
