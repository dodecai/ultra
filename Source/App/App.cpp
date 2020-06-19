#include <Ultra.h>

// Application
namespace Ultra {

class DemoLayer: public Layer {
	shared_ptr<Shader> BasicShader;
	shared_ptr<VertexArray> SquareVertexArray;
	shared_ptr<VertexArray> TriangleVertexArray;

	OrthographicCamera Camera;
	glm::vec3 CameraPosition;
	float CameraMoveSpeed = 1.0f;
	float CameraRotation = 0.1f;
	float CameraRotationSpeed = 90.0f;

public:
	DemoLayer():
		Layer("Main"),
		Camera(-1.0f, 1.0f, -1.0f, 1.0f),
		CameraPosition(0.0f) {
		// Properties
		Gfx::SetVSync(false);
	}

	void Create() override {
		// Shader
		BasicShader.reset(Shader::Create("./Assets/Shaders/Basic.vert", "./Assets/Shaders/Basic.frag"));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		// Triangle
		TriangleVertexArray.reset(VertexArray::Create());

		float triangleVertices[7 * 3] = {
			-0.5f,  -0.5f,  0.0f,	0.8f, 0.2f, 0.2f, 1.0f,
			0.5f,  -0.5f,  0.0f,	0.2f, 0.8f, 0.2f, 1.0f,
			0.0f,   0.5f,  0.0f,	0.2f, 0.2f, 0.8f, 1.0f
		};
		std::shared_ptr<VertexBuffer> triangleVB;
		triangleVB.reset(VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));
		triangleVB->SetLayout(layout);
		TriangleVertexArray->AddVertexBuffer(triangleVB);

		uint32_t indicesTrianlge[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> triangleIB;
		triangleIB.reset(IndexBuffer::Create(indicesTrianlge, sizeof(indicesTrianlge) / sizeof(uint32_t)));
		TriangleVertexArray->SetIndexBuffer(triangleIB);

		// Square
		SquareVertexArray.reset(VertexArray::Create());

		float suareVertices[7 * 4] = {
			-0.5f,  -0.5f,  0.0f,	0.8f, 0.2f, 0.2f, 1.0f,
			0.5f,  -0.5f,  0.0f,	0.2f, 0.8f, 0.2f, 1.0f,
			0.5f,   0.5f,  0.0f,	0.2f, 0.2f, 0.8f, 1.0f,
			-0.5f,   0.5f,  0.0f,	0.5f, 0.5f, 0.5f, 1.0f
		};
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(suareVertices, sizeof(suareVertices)));
		squareVB->SetLayout(layout);
		SquareVertexArray->AddVertexBuffer(squareVB);

		uint32_t squreIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
		SquareVertexArray->SetIndexBuffer(squareIB);
	}

	void Update(Timestep deltaTime) override {
		// Inputs
		if (Input::GetKeyState(KeyCode::KeyA)) {
			CameraPosition.x -= CameraMoveSpeed * deltaTime;
		} else if (Input::GetKeyState(KeyCode::KeyD)) {
			CameraPosition.x += CameraMoveSpeed * deltaTime;
		}
		if (Input::GetKeyState(KeyCode::KeyW)) {
			CameraPosition.y += CameraMoveSpeed * deltaTime;
		} else if (Input::GetKeyState(KeyCode::KeyS)) {
			CameraPosition.y -= CameraMoveSpeed * deltaTime;
		}

		if (Input::GetKeyState(KeyCode::KeyQ)) {
			CameraRotation -= CameraRotationSpeed * deltaTime;
		} else if (Input::GetKeyState(KeyCode::KeyE)) {
			CameraRotation += CameraRotationSpeed * deltaTime;
		}

		// Render
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
		
		Camera.SetPosition(CameraPosition);
		Camera.SetRotation(CameraRotation);

		Renderer::BeginScene(Camera);
		Renderer::Submit(BasicShader, SquareVertexArray);
		Renderer::Submit(BasicShader, TriangleVertexArray);
		Renderer::EndScene();
	}
};

class App: public Omnia::Application {
public:
	void Create() override {
		PushLayer(new DemoLayer());
	}

	void Destroy() override {
	}

	void Update(Timestep deltaTime) override {
	}
};

}

// Application Entry-Point
Omnia::Application *CreateApplication() {
	return new Ultra::App();
}
