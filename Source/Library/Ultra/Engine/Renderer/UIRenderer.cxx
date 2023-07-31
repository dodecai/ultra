﻿module;

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

module Ultra.Engine.UIRenderer;

import Ultra.Logger;
import Ultra.Engine.Renderer.Buffer;
import Ultra.Engine.Renderer.CommandBuffer;
import Ultra.Engine.Renderer.PipelineState;
import Ultra.Engine.Renderer.Shader;
import Ultra.Engine.Renderer.Texture;

namespace Ultra {

template<typename T>
size_t sizeof_vector(const typename std::vector<T> &vec) {
    return sizeof(T) * vec.size();
}



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



#define TEST_NEW_RENDERER

void UIDraw::DrawBorder(float s, float x, float y, float w, float h) {
    #ifdef TEST_NEW_RENDERER
    UIRenderer::Instance().DrawRectangle({ x, y, 0 }, { w, s });
    UIRenderer::Instance().DrawRectangle({ x, y + h - s, 0 }, { w, s });
    UIRenderer::Instance().DrawRectangle({ x, y + s, 0 }, { s, h - 2 * s });
    UIRenderer::Instance().DrawRectangle({ x + w - s , y + s, 0 }, { s, h - 2 * s });
    #else
    DrawRect(x, y, w, s);
    DrawRect(x, y + h - s, w, s);
    DrawRect(x, y + s, s, h - 2 * s);
    DrawRect(x + w - s, y + s, s, h - 2 * s);
    #endif
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

void UIDraw::DrawText(Reference<Texture> texture, int index, float x0, float y0, float x1, float y1, const Color &color) {
    #ifdef TEST_NEW_RENDERER
    float alpha = 1.0f;
    UIRenderer::Instance().DrawRectangle({ x0, y0, 0 }, { x1, y1 }, texture, { color.Red, color.Green, color.Blue, color.Alpha * alpha });
    UIDraw::DrawColor(1, 1, 1, 1);
    #else
    UIDraw::DrawColor(color.Red, color.Green, color.Blue, color.Alpha);
    glEnable(GL_TEXTURE_2D);
    texture->Bind(index);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x0, y0);
    glTexCoord2f(0, 1); glVertex2f(x0, y1);
    glTexCoord2f(1, 1); glVertex2f(x1, y1);
    glTexCoord2f(1, 0); glVertex2f(x1, y0);
    glEnd();

    texture->Unbind(index);
    UIDraw::DrawColor(1, 1, 1, 1);
    glDisable(GL_TEXTURE_2D);
    UIDraw::DrawColor(1, 1, 1, 1);
    #endif
}



void Panel::Draw() const {
    const float pad = 64.0f;
    float x = mPosition.X - pad;
    float y = mPosition.Y - pad;
    float width = mSize.Width + 2.0f * pad;
    float height = mSize.Height + 2.0f * pad;

    UIRenderer::Instance().DrawPanel({ x, y, 0.0f }, { width, height }, { mColor.Red, mColor.Green, mColor.Blue, mColor.Alpha }, mInnerAlpha, mBevel);
}

void Image::Draw() const {
    #ifdef TEST_NEW_RENDERER
    #else
    //mImage->Draw(0, mPosition.X, mPosition.Y, mSize.Width, mSize.Height);
    #endif
}

void Rectangle::Draw() const {
    #ifdef TEST_NEW_RENDERER
    UIRenderer::Instance().DrawRectangle({ mPosition.X, mPosition.Y, 0.0f }, { mSize.Width, mSize.Height }, { mColor.Red, mColor.Green, mColor.Blue, mColor.Alpha });
    #else
    UIDraw::DrawColor(mColor.Red, mColor.Green, mColor.Blue, mColor.Alpha);
    if (mOutline) {
        UIDraw::DrawBorder(1.0f, mPosition.X, mPosition.Y, mSize.Width, mSize.Height);
    } else {
        UIDraw::DrawRect(mPosition.X, mPosition.Y, mSize.Width, mSize.Height);
    }
    #endif
}

void Text::Draw() const {
    //Font::Draw(mFont, mText.c_str(), mPosition.X, mPosition.Y, mColor.Red, mColor.Green, mColor.Blue, mColor.Alpha);
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

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
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
        { 0.0f, 1.0f }
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

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    for (size_t i = 0; i < SRenderData.ComponentVertexPositions.size(); i++) {
        SRenderData.ComponentVertexBufferData.emplace_back((transform * SRenderData.ComponentVertexPositions[i]), color, textureCoords[i], textureIndex, tiling);
    }

    // ToDo: Implement Statistics
    Reset();
}

void UIRenderer::EndScene() {
    Reset();
}

void UIRenderer::Reset() {
    Flush();
    SRenderData.PanelVertexBufferData.clear();
    SRenderData.ComponentVertexBufferData.clear();
    SRenderData.TextureSlotIndex = 1;
}

void UIRenderer::Flush() {
    if (SRenderData.PanelVertexBufferData.size()) {
        auto dataSize = sizeof_vector(SRenderData.PanelVertexBufferData);
        SRenderData.PanelVertexBuffer->UpdateData(SRenderData.PanelVertexBufferData.data(), dataSize);

        SRenderData.PanelShader->Bind();

        const float pad = 64.0f;
        SRenderData.PanelShader->UpdateUniform("uPadding", pad);
        SRenderData.PanelShader->UpdateUniform("uProjection", SRenderData.ProjectionMatrix);
        SRenderData.PanelShader->UpdateUniform("uModelView", SRenderData.ViewMatrix);

        SRenderData.PanelVertexBuffer->Bind();
        SRenderData.PanelPipeline->Bind();
        SRenderData.PanelIndexBuffer->Bind();
        SCommandBuffer->DrawIndexed({ SRenderData.PanelVertexBufferData.size() * 6 }, PrimitiveType::Triangle, true);
        SRenderData.PanelPipeline->Unbind();
        SRenderData.PanelIndexBuffer->Unbind();
        SRenderData.PanelVertexBuffer->Unbind();
        SRenderData.PanelShader->Unbind();
        //SRenderData.Stats.DrawCalls++;
    }

    if (SRenderData.ComponentVertexBufferData.size()) {
        auto dataSize = sizeof_vector(SRenderData.ComponentVertexBufferData);
        SRenderData.ComponentVertexBuffer->UpdateData(SRenderData.ComponentVertexBufferData.data(), dataSize);

        SRenderData.ComponentShader->Bind();
        SRenderData.ComponentShader->UpdateUniform("uProjection", SRenderData.ProjectionMatrix);
        SRenderData.ComponentShader->UpdateUniform("uView", SRenderData.ViewMatrix);
        //SRenderData.ComponentShader->UpdateUniform("uViewProjection", SRenderData.ViewProjectionMatrix);

        for (uint32_t i = 0; i < SRenderData.TextureSlotIndex; i++) {
            SRenderData.TextureSlots[i]->Bind(i);
        }

        SRenderData.ComponentVertexBuffer->Bind();
        SRenderData.ComponentPipeline->Bind();
        SRenderData.ComponentIndexBuffer->Bind();
        //SCommandBuffer->DrawIndexed({ SRenderData.PanelVertexBufferData.size() * 6 }, PrimitiveType::Triangle, true);
        glDrawElements(GL_TRIANGLES, (SRenderData.ComponentVertexBufferData.size() * 6), GL_UNSIGNED_INT, nullptr);

        // ToDo: Implement Statistics
    }
}

}
