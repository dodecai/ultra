module;

#include <glad/gl.h>

module Ultra.Platform.Renderer.GLPipelineState;

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



GLPipelineState::GLPipelineState(const PipelineProperties &properties): PipelineState(properties) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    Apply();
}

GLPipelineState::~GLPipelineState() {
    glDeleteVertexArrays(1, &mPipelineID);
}

void GLPipelineState::Apply() {
    if (mPipelineID) glDeleteVertexArrays(1, &mPipelineID);
    glCreateVertexArrays(1, &mPipelineID);
}

void GLPipelineState::Bind() {
    glBindVertexArray(mPipelineID);

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

void GLPipelineState::Unbind() {
    glBindVertexArray(0);
}

}
