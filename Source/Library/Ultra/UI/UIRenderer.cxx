module Ultra.UI.Renderer;

import <glad/gl.h>;
import <glm/gtc/matrix_transform.hpp>;

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
        glScissor(static_cast<GLint>(x), static_cast<GLint>(properties.Height - (y + height)), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
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



void UIRenderer::DrawPanel(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float innerAlpha, float bevel) {
    if (SRenderData.PanelVertexBufferData.size() * 6 >= SRenderData.PanelMaxIndices) Reset();

    auto zValue = 0.0f;
    glm::vec4 positions[4] = {
        { position.x,           position.y + size.y,    zValue, 1.0f }, // top-left
        { position.x,           position.y,             zValue, 1.0f }, // bottom-left
        { position.x + size.x,  position.y,             zValue, 1.0f }, // bottom-right
        { position.x + size.x,  position.y + size.y,    zValue, 1.0f }, // top-right
    };
    glm::vec2 texCoords[4] = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
    };
    for (size_t i = 0; i < SRenderData.PanelVertexPositions.size(); i++) {
        //glm::mat4 transform = glm::translate(glm::mat4(1.0f), { positions[i].x, positions[i].y, 0.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        //auto newPos = transform * SRenderData.PanelVertexPositions[i];
        SRenderData.PanelVertexBufferData.emplace_back(positions[i], color, size, innerAlpha, bevel, texCoords[i]);
    }

    // ToDo: Implement Statistics
    Reset();
}

void UIRenderer::DrawLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color) {
    if (SRenderData.ComponentVertexBufferData.size() >= SRenderData.ComponentMaxIndices) Reset();

    SRenderData.ComponentVertexBufferData.emplace_back(start, color);
    SRenderData.ComponentVertexBufferData.emplace_back(end, color);
    SRenderData.ComponentVertexBufferData.emplace_back(glm::vec3(), glm::vec4());
    SRenderData.ComponentVertexBufferData.emplace_back(glm::vec3(), glm::vec4());
}

void UIRenderer::DrawRectangle(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
    if (SRenderData.ComponentVertexBufferData.size() * 6 >= SRenderData.ComponentMaxIndices) Reset();

    const float textureIndex = 0.0f;
    const glm::vec2 textureCoords[] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
    };
    const float tiling = 1.0f;

    glm::vec2 center = position + glm::vec3(size.x, size.y, 0.0f) *0.5f;
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    for (size_t i = 0; i < SRenderData.ComponentVertexPositions.size(); i++) {
        SRenderData.ComponentVertexBufferData.emplace_back((transform * SRenderData.ComponentVertexPositions[i]), color, textureCoords[i], textureIndex, tiling);
    }

    // ToDo: Implement Statistics
    Reset();
}

void UIRenderer::DrawRectangle(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture> &texture, const glm::vec4 &color, float tiling) {
    if (SRenderData.ComponentVertexBufferData.size() * 6 >= SRenderData.ComponentMaxIndices) Reset();

    float textureIndex = 0.0f;
    const glm::vec2 textureCoords[] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
    };
    for (uint32_t i = 1; i < SRenderData.TextureSlotIndex; i++) {
        if (*SRenderData.TextureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }
    if (textureIndex == 0.0f) {
        if (SRenderData.TextureSlotIndex >= SRenderData.MaxTextureSlots) Reset();

        textureIndex = (float)SRenderData.TextureSlotIndex;
        SRenderData.TextureSlots[SRenderData.TextureSlotIndex] = texture;
        SRenderData.TextureSlotIndex++;
    }

    glm::vec2 center = position + glm::vec3(size.x, size.y, 0.0f) * 0.5f;
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    //glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    for (size_t i = 0; i < SRenderData.ComponentVertexPositions.size(); i++) {
        SRenderData.ComponentVertexBufferData.emplace_back((transform * SRenderData.ComponentVertexPositions[i]), color, textureCoords[i], textureIndex, tiling);
    }

    // ToDo: Implement Statistics
    Reset();
}

void UIRenderer::DrawText(const glm::vec3 &position, const glm::vec2 &size, const Reference<Texture> &texture, const glm::vec4 &color, float tiling) {
    if (SRenderData.TextVertexBufferData.size() * 6 >= SRenderData.TextMaxIndices) Reset();

    float textureIndex = 0.0f;
    const glm::vec2 textureCoords[] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
    };
    for (uint32_t i = 1; i < SRenderData.TextTextureSlotIndex; i++) {
        if (*SRenderData.TextTextureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }
    if (textureIndex == 0.0f) {
        if (SRenderData.TextTextureSlotIndex >= SRenderData.MaxTextureSlots) Reset();

        textureIndex = (float)SRenderData.TextTextureSlotIndex;
        SRenderData.TextTextureSlots[SRenderData.TextTextureSlotIndex] = texture;
        SRenderData.TextTextureSlotIndex++;
    }

    glm::vec2 center = position + glm::vec3(size.x, size.y, 0.0f) * 0.5f;
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    //glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    for (size_t i = 0; i < SRenderData.TextVertexPositions.size(); i++) {
        SRenderData.TextVertexBufferData.emplace_back((transform * SRenderData.TextVertexPositions[i]), color, textureCoords[i]);
    }

    // ToDo: Implement Statistics
    Reset();
}


void UIRenderer::Reset() {
    Flush();
    SRenderData.PanelVertexBufferData.clear();
    SRenderData.ComponentVertexBufferData.clear();
    SRenderData.TextureSlotIndex = 1;
    SRenderData.TextVertexBufferData.clear();
    SRenderData.TextTextureSlotIndex = 0;
}

void UIRenderer::Flush() {
    // Panels
    if (SRenderData.PanelVertexBufferData.size()) {
        auto dataSize = sizeof_vector(SRenderData.PanelVertexBufferData);
        SRenderData.PanelVertexBuffer->UpdateData(SRenderData.PanelVertexBufferData.data(), dataSize);

        SRenderData.PanelShader->Bind();

        SRenderData.PanelVertexBuffer->Bind();
        SRenderData.PanelPipeline->Bind();
        SRenderData.PanelIndexBuffer->Bind();
        SCommandBuffer->DrawIndexed(SRenderData.PanelVertexBufferData.size() * 6, PrimitiveType::Triangle, true);
        SRenderData.PanelPipeline->Unbind();
        SRenderData.PanelIndexBuffer->Unbind();
        SRenderData.PanelVertexBuffer->Unbind();
        SRenderData.PanelShader->Unbind();
        //SRenderData.Stats.DrawCalls++;
    }

    // Components
    if (SRenderData.ComponentVertexBufferData.size()) {
        auto dataSize = sizeof_vector(SRenderData.ComponentVertexBufferData);
        SRenderData.ComponentVertexBuffer->UpdateData(SRenderData.ComponentVertexBufferData.data(), dataSize);

        SRenderData.ComponentShader->Bind();
        //SRenderData.ComponentShader->UpdateUniform("uViewProjection", SRenderData.ViewProjectionMatrix);

        for (uint32_t i = 0; i < SRenderData.TextureSlotIndex; i++) {
            SRenderData.TextureSlots[i]->Bind(i);
        }

        SRenderData.ComponentVertexBuffer->Bind();
        SRenderData.ComponentPipeline->Bind();
        SRenderData.ComponentIndexBuffer->Bind();

        SCommandBuffer->DrawIndexed(static_cast<GLsizei>(SRenderData.ComponentVertexBufferData.size() * 6), PrimitiveType::Triangle, true);

        // ToDo: Implement Statistics
    }

    // Text
    if (SRenderData.TextVertexBufferData.size()) {
        auto dataSize = sizeof_vector(SRenderData.TextVertexBufferData);
        SRenderData.TextVertexBuffer->UpdateData(SRenderData.TextVertexBufferData.data(), dataSize);

        SRenderData.TextShader->Bind();
        //SRenderData.TextShader->UpdateUniform("uViewProjection", SRenderData.ViewProjectionMatrix);

        for (uint32_t i = 0; i < SRenderData.TextTextureSlotIndex; i++) {
            SRenderData.TextTextureSlots[i]->Bind(i);
        }

        SRenderData.TextVertexBuffer->Bind();
        SRenderData.TextPipeline->Bind();
        SRenderData.TextIndexBuffer->Bind();

        SCommandBuffer->DrawIndexed(static_cast<GLsizei>(SRenderData.TextVertexBufferData.size() * 6), PrimitiveType::Triangle, true);

        // ToDo: Implement Statistics
    }
}

}
