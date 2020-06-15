#include <Ultra.h>

// Engine
#include "Engine.h"

// Applicaton
namespace Ultra {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
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
}


class MainLayer: public Layer {
public:
	MainLayer():
		Layer{ "Main" } {
	}

	void Event(void *event) {}
	void Update() override {}
};

class App: public Application {
	// Test
	uint32_t VertexArray = 0;

	unique_ptr<Ultra::Shader> BasicShader;
	unique_ptr<Ultra::VertexBuffer> VBO;
	unique_ptr<Ultra::IndexBuffer> IBO;

public:
	void Create() {
		// Preparation
		applog <<  "Welcome to " << AppCaption << "!" << std::endl;
		PushLayer(new MainLayer());

		// Test
		gladLoadGL();

		BasicShader.reset(Ultra::Shader::Create("./Assets/Shaders/Basic.vert", "./Assets/Shaders/Basic.frag"));

		glGenVertexArrays(1, &VertexArray);
		glBindVertexArray(VertexArray);

		float vertices[3 * 7] = {
			-0.5f,  -0.5f,  0.0f,	0.8f, 0.2f, 0.2f, 1.0f,
			 0.5f,  -0.5f,  0.0f,	0.2f, 0.8f, 0.2f, 1.0f,
			 0.0f,   0.5f,  0.0f,	0.2f, 0.2f, 0.8f, 1.0f
		};
		VBO.reset(Ultra::VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		VBO->SetLayout(layout);

		uint32_t index = 0;
		for (const auto &element : VBO->GetLayout()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
								  element.GetComponentCount(),
								  ShaderDataTypeToOpenGLBaseType(element.Type),
								  element.Normalized ? GL_TRUE : GL_FALSE,
								  VBO->GetLayout().GetStride(),
								  (const void *)(uint64_t)element.Offset
			);
			index++;
		}

		uint32_t indices[3] = { 0, 1, 2 };
		IBO.reset(Ultra::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	}

	void Destroy() {
	}

	void Update() {
		BasicShader->Bind();
		glBindVertexArray(VertexArray);
		glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	//void KeyboardEvent(KeyboardEventData data) {}
	//void MouseEvent(MouseEventData data) {}
	//void WindowEvent(WindowEventData data) {}
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
	return new Ultra::App();
}
