#include <Ultra.h>

#include <glm/gtc/matrix_transform.hpp>

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

	glm::vec3 TrianglePosition;
	glm::vec3 SquarePosition;
	float TriangleMoveSpeed = 1.0f;

public:
	DemoLayer():
		Layer("Main"),
		Camera(-1.0f, 1.0f, -1.0f, 1.0f),
		CameraPosition(0.0f),
		TrianglePosition(0.0f),
		SquarePosition(0.0f) {
		// Properties
		gladLoadGL();
		Gfx::SetVSync(false);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);  // enable depth-testing
		glDepthFunc(GL_LESS);	  // depth-testing interprets a smaller value as "closer"
		glEnable(GL_MULTISAMPLE);
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
		// Camera Input
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

		// Object Input
		if (Input::GetKeyState(KeyCode::Left)) {
			TrianglePosition.x -= TriangleMoveSpeed * deltaTime;
		} else if (Input::GetKeyState(KeyCode::Right)) {
			TrianglePosition.x += TriangleMoveSpeed * deltaTime;
		}
		if (Input::GetKeyState(KeyCode::Up)) {
			TrianglePosition.y += TriangleMoveSpeed * deltaTime;
		} else if (Input::GetKeyState(KeyCode::Down)) {
			TrianglePosition.y -= TriangleMoveSpeed * deltaTime;
		}

		// Render
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
		
		Camera.SetPosition(CameraPosition);
		Camera.SetRotation(CameraRotation);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), TrianglePosition);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Renderer::BeginScene(Camera);
		for (int y = 0; y < 25; y++) {
			for (int x = 0; x < 100; x++) {
				glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transformS = glm::translate(glm::mat4(1.0f), position) * scale;

				Renderer::Submit(BasicShader, SquareVertexArray, transformS);
			}
		}
		Renderer::Submit(BasicShader, TriangleVertexArray, transform);
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
