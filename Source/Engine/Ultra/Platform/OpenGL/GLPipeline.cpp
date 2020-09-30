#include "GLPipeline.h"

#include <glad/gl.h>

#include "Ultra/Renderer/Renderer.h"

namespace Ultra {


static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Bool:			return GL_BOOL;
        case ShaderDataType::Float:			return GL_FLOAT;
        case ShaderDataType::Float2:		return GL_FLOAT;
        case ShaderDataType::Float3:		return GL_FLOAT;
        case ShaderDataType::Float4:		return GL_FLOAT;
        case ShaderDataType::Int:			return GL_INT;
        case ShaderDataType::Int2:			return GL_INT;
        case ShaderDataType::Int3:			return GL_INT;
        case ShaderDataType::Int4:			return GL_INT;
        case ShaderDataType::Mat3:			return GL_FLOAT;
        case ShaderDataType::Mat4:			return GL_FLOAT;
        default:							return 0;
    }
    AppLogCritical("Unknown ShaderDataType!");
    return 0;
}


GLPipeline::GLPipeline(const PipelineProperties &properties):
    Properties { properties } {
    Invalidate();
}

GLPipeline::~GLPipeline() {
    glDeleteVertexArrays(1, &VertexArrayRendererID);
}

void GLPipeline::Bind() {
    glBindVertexArray(VertexArrayRendererID);

    const auto &layout = Properties.Layout;
    uint32_t attributeIndex = 0;
    for (const auto &element : layout) {
        auto baseType = ShaderDataTypeToGLBaseType(element.Type);
        glEnableVertexAttribArray(attributeIndex);
        if (baseType == GL_INT) {
            glVertexAttribIPointer(
                attributeIndex,
                element.GetComponentCount(),
                baseType,
                layout.GetStride(),
                (const void *)(intptr_t)element.Offset
            );
        } else {
            glVertexAttribPointer(
                attributeIndex,
                element.GetComponentCount(),
                baseType,
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void *)(intptr_t)element.Offset);
        }
        attributeIndex++;
    }
}

void GLPipeline::Invalidate() {
    auto &vertexArrayRendererID = VertexArrayRendererID;
    if (vertexArrayRendererID) glDeleteVertexArrays(1, &vertexArrayRendererID);

    glCreateVertexArrays(1, &vertexArrayRendererID);
}

PipelineProperties &GLPipeline::GetProperties() {
    return Properties;
}

const PipelineProperties &GLPipeline::GetProperties() const {
    return Properties;
}
}
