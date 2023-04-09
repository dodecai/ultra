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

GLPipeline::GLPipeline(const PipelineProperties &properties):
    mProperties(properties)
{
    Invalidate();
}

GLPipeline::~GLPipeline() {
    glDeleteVertexArrays(1, &mRendererID);
}

void GLPipeline::Bind() const {
    glBindVertexArray(mRendererID);

    auto index = 0;
    const auto &layout = mProperties.Layout;
    for (const auto &attribute : layout) {
        auto baseType = ShaderDataTypeToGLBaseType(attribute.Type);
        glEnableVertexAttribArray(index);
        if (baseType == GL_INT) {
            glVertexAttribIPointer(index, attribute.GetComponentCount(), baseType, layout.GetStride(), (const void *)attribute.Offset);
        } else {
            glVertexAttribPointer(index, attribute.GetComponentCount(), baseType, attribute.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void *)attribute.Offset);
        }
        index++;
    }
}

void GLPipeline::Invalidate() {
    if (mRendererID) glDeleteVertexArrays(1, &mRendererID);
    glGenVertexArrays(1, &mRendererID);
    glBindVertexArray(mRendererID);
    glBindVertexArray(0);
}

}
