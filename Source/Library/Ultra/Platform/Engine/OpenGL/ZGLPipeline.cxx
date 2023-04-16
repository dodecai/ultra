module;

#include <glad/gl.h>

module Ultra.Platform.Engine.GLPipeline;

namespace Ultra {

static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type) {
	switch (type) {
        case ShaderDataType::Bool:      return GL_BOOL;
        case ShaderDataType::Bool2:     return GL_BOOL;
        case ShaderDataType::Bool3:     return GL_BOOL;
        case ShaderDataType::Bool4:     return GL_BOOL;

		case ShaderDataType::Float:     return GL_FLOAT;
		case ShaderDataType::Float2:    return GL_FLOAT;
		case ShaderDataType::Float3:    return GL_FLOAT;
		case ShaderDataType::Float4:    return GL_FLOAT;

        case ShaderDataType::Int:       return GL_INT;
        case ShaderDataType::Int2:      return GL_INT;
        case ShaderDataType::Int3:      return GL_INT;
        case ShaderDataType::Int4:      return GL_INT;

        case ShaderDataType::Mat2:      return GL_FLOAT;
		case ShaderDataType::Mat3:      return GL_FLOAT;
		case ShaderDataType::Mat4:      return GL_FLOAT;
	}
	return 0;
}

GLPipeline::GLPipeline(const PipelineProperties &properties): mProperties(properties) {
    Invalidate();
}

GLPipeline::~GLPipeline() {
    glDeleteVertexArrays(1, &mRendererID);
}

void GLPipeline::Bind() const {
    glBindVertexArray(mRendererID);

    auto attributeIndex = 0;
    const auto &layout = mProperties.Layout;
    for (const auto &attribute : layout) {
        auto baseType = ShaderDataTypeToGLBaseType(attribute.Type);
        glEnableVertexAttribArray(attributeIndex);
        if (baseType == GL_INT) {
            glVertexAttribIPointer(
                attributeIndex,
                attribute.GetComponentCount(),
                baseType,
                layout.GetStride(),
                (const void *)(intptr_t)attribute.Offset
            );
        } else {
            glVertexAttribPointer(
                attributeIndex,
                attribute.GetComponentCount(),
                baseType,
                attribute.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void *)(intptr_t)attribute.Offset
            );
        }
        attributeIndex++;
    }
}

void GLPipeline::Invalidate() {
    if (mRendererID) glDeleteVertexArrays(1, &mRendererID);
    glCreateVertexArrays(1, &mRendererID);
}

void GLPipeline::Unbind() const {
    glBindVertexArray(0);
}

}
