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
    ResetProperties();
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
    UpdateProperties();

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
    ResetProperties();
    glBindVertexArray(0);
}

void GLPipelineState::ResetProperties() {
    glBlendFunc(GL_ONE, GL_ZERO);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(true);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GLPipelineState::UpdateProperties() {
    switch (mProperties.BlendMode) {
        case BlendMode::Additive: { glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE); break; }
        case BlendMode::Alpha:    { glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break; /* glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break;*/ }
        case BlendMode::Disabled: { glBlendFunc(GL_ONE, GL_ZERO); break; }
        case BlendMode::Multiply: { glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); break; }
    }

    switch (mProperties.CullMode) {
        case CullMode::Back:    { glEnable(GL_CULL_FACE); glCullFace(GL_BACK); break; }
        case CullMode::Front:   { glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); break; }
        case CullMode::None:    { glDisable(GL_CULL_FACE);  break; }
    }
    //glFrontFace(GL_CCW);

    if (mProperties.DepthTest) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    if (mProperties.DepthWritable) {
        glDepthMask(mProperties.DepthWritable);
    } else {
        glDepthMask(mProperties.DepthWritable);
    }

    if (mProperties.Wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

}
