module Ultra.Platform.Renderer.GLCommandBuffer;

import <glad/gl.h>;

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Ultra {

GLCommandBuffer::GLCommandBuffer() {}

GLCommandBuffer::~GLCommandBuffer() {}


void GLCommandBuffer::Begin() {
    // ToDo: Not needed, but it can be done with lambdas
}

void GLCommandBuffer::End() {
    // ToDo: Not needed, but it can be done with lambdas
}


void GLCommandBuffer::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLCommandBuffer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    // ToDo: Prevent sizes above imagination
    if (!(width == 0) || !(height == 0)) return;
    glViewport(x, y, width, height);
}


void GLCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {}

void GLCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
    //if (!depthTest) glDisable(GL_DEPTH_TEST);

    GLenum mode = GL_TRIANGLES;
    GLenum type = GL_UNSIGNED_INT;
    //switch (properties.Type) {
    //    case IndexType::UINT8:  { type = GL_UNSIGNED_BYTE; break; }
    //    case IndexType::UINT16: { type = GL_UNSIGNED_INT; break; }
    //    case IndexType::UINT32: { type = GL_UNSIGNED_INT; break; }
    //}

    //switch (primitive) {
    //    case PrimitiveType::Line: { mode = GL_LINES; break; }
    //    case PrimitiveType::Triangle: { mode = GL_TRIANGLES; break; }
    //}

    glDrawElements(mode, indexCount, type, nullptr);

    //if (!depthTest) glEnable(GL_DEPTH_TEST);
}

void GLCommandBuffer::DrawIndexed(size_t count, PrimitiveType primitive, bool depthTest) {
    if (!depthTest) { glDepthMask(GL_FALSE); } else { glDepthMask(GL_TRUE); };

    GLenum type = GL_UNSIGNED_INT;
    //switch (properties.Type) {
    //    case IndexType::UINT8:  { type = GL_UNSIGNED_BYTE; break; }
    //    case IndexType::UINT16: { type = GL_UNSIGNED_INT; break; }
    //    case IndexType::UINT32: { type = GL_UNSIGNED_INT; break; }
    //}

    GLenum mode = GL_TRIANGLES;
    switch (primitive) {
        case PrimitiveType::Circle:   { mode = GL_TRIANGLES; break; }
        case PrimitiveType::Line:     { mode = GL_LINES; break; }
        case PrimitiveType::Triangle: { mode = GL_TRIANGLES; break; }
    }

    // ToDo: C4267 possible loss of data
    glDrawElements(mode, static_cast<GLsizei>(count), type, nullptr);

    if (!depthTest) { glDepthMask(GL_TRUE); } else { glDepthMask(GL_FALSE); };
}

void GLCommandBuffer::UpdateStencilBuffer() {
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
}

void GLCommandBuffer::EnableStencilTest() {
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
}

void GLCommandBuffer::ResetStencilTest() {
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
}


void GLCommandBuffer::Execute() {}

}

#pragma warning(pop)
