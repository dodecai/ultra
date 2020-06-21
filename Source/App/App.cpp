#include <Ultra.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Application
namespace Ultra {

class DemoLayer: public Layer {
	glm::vec4 ClearColor;
	OrthographicCamera Camera;
	glm::vec3 CameraPosition;
	float CameraMoveSpeed = 1.0f;
	float CameraRotation = 0.1f;
	float CameraRotationSpeed = 90.0f;

	ShaderLibrary Shaders;

	Reference<VertexArray> GridVertexArray;

	Reference<Texture2D> TextureBase;
	Reference<Texture2D> TextureLogo;

	Reference<VertexArray> PlayerVertexArray;
	glm::vec3 PlayerPosition;
	float PlayerMoveSpeed = 1.0f;

public:
	DemoLayer():
		Layer("Main"),
		ClearColor(0.1f, 0.1f, 0.1f, 1.0f),
		Camera(-1.0f, 1.0f, -1.0f, 1.0f),
		CameraPosition(0.0f),
		PlayerPosition(0.0f) {
		// Properties
		gladLoadGL();
		Gfx::SetVSync(false);
		Renderer::Load();
	}

	void Create() override {
		Shaders.Load("./Assets/Shaders/Grid.vert", "./Assets/Shaders/Grid.frag");
		Shaders.Load("./Assets/Shaders/Player.vert", "./Assets/Shaders/Player.frag");
		Shaders.Load("./Assets/Shaders/Texture.glsl");

		// Grid
		BufferLayout gridLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TextCoord" },
		};
		GridVertexArray.reset(VertexArray::Create());

		Reference<VertexBuffer> gridVB;
		float gridVertices[5 * 4] = {
			-0.5f,  -0.5f,  0.0f,	0.0f, 0.0f,
			0.5f,  -0.5f,  0.0f,	1.0f, 0.0f,
			0.5f,   0.5f,  0.0f,	1.0f, 1.0f,
			-0.5f,   0.5f,  0.0f,	0.0f, 1.0f,
		};
		gridVB.reset(VertexBuffer::Create(gridVertices, sizeof(gridVertices)));
		gridVB->SetLayout(gridLayout);
		GridVertexArray->AddVertexBuffer(gridVB);

		Reference<IndexBuffer> gridIB;
		uint32_t gridIndices[6] = { 0, 1, 2, 2, 3, 0 };
		gridIB.reset(IndexBuffer::Create(gridIndices, sizeof(gridIndices) / sizeof(uint32_t)));
		GridVertexArray->SetIndexBuffer(gridIB);

		// Player
		BufferLayout playerLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};
		PlayerVertexArray.reset(VertexArray::Create());

		Reference<VertexBuffer> playerVB;
		float playerVertices[7 * 3] = {
			-0.5f,  -0.5f,  0.0f,	0.8f, 0.2f, 0.2f, 1.0f,
			0.5f,  -0.5f,  0.0f,	0.2f, 0.8f, 0.2f, 1.0f,
			0.0f,   0.5f,  0.0f,	0.2f, 0.2f, 0.8f, 1.0f
		};
		playerVB.reset(VertexBuffer::Create(playerVertices, sizeof(playerVertices)));
		playerVB->SetLayout(playerLayout);
		PlayerVertexArray->AddVertexBuffer(playerVB);

		Reference<IndexBuffer> playerIB;
		uint32_t playerIndices[3] = { 0, 1, 2 };
		playerIB.reset(IndexBuffer::Create(playerIndices, sizeof(playerIndices) / sizeof(uint32_t)));
		PlayerVertexArray->SetIndexBuffer(playerIB);

		// Texture Test
		TextureBase.reset(Texture2D::Create("./Assets/Textures/Checkerboard.png"));
		TextureLogo.reset(Texture2D::Create("./Assets/Textures/AlphaChannel.png"));
		Shaders.Get("Texture")->Bind();
		Shaders.Get("Texture")->UploadaUniformInt("u_Texture", 0);
	}

	void Update(Timestamp deltaTime) override {
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

		// Player Input
		if (Input::GetKeyState(KeyCode::Left)) {
			PlayerPosition.x -= PlayerMoveSpeed * deltaTime;
		} else if (Input::GetKeyState(KeyCode::Right)) {
			PlayerPosition.x += PlayerMoveSpeed * deltaTime;
		}
		if (Input::GetKeyState(KeyCode::Up)) {
			PlayerPosition.y += PlayerMoveSpeed * deltaTime;
		} else if (Input::GetKeyState(KeyCode::Down)) {
			PlayerPosition.y -= PlayerMoveSpeed * deltaTime;
		}

		// Camera
		Camera.SetPosition(CameraPosition);
		Camera.SetRotation(CameraRotation);

		// Render
		RenderCommand::SetClearColor(ClearColor);
		RenderCommand::Clear();
		Renderer::BeginScene(Camera);

		// Grid
		glm::mat4 scaleGrid = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int y = 0; y < 25; y++) {
			for (int x = 0; x < 100; x++) {
				glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transformS = glm::translate(glm::mat4(1.0f), position) * scaleGrid;
				Renderer::Submit(Shaders.Get("Grid"), GridVertexArray, transformS);
			}
		}
		TextureBase->Bind();
		Renderer::Submit(Shaders.Get("Texture"), GridVertexArray, glm::mat4(1.0f));
		TextureLogo->Bind();
		Renderer::Submit(Shaders.Get("Texture"), GridVertexArray, glm::mat4(1.0f));

		// Player
		glm::mat4 scalePlayer = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), PlayerPosition) * scalePlayer;
		Renderer::Submit(Shaders.Get("Player"), PlayerVertexArray, transform);

		//~Renderer
		Renderer::EndScene();
	}

	void GuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Clear Color", glm::value_ptr(ClearColor));
		ImGui::End();
	}
};

class App: public Omnia::Application {
public:
	void Create() override {
		PushLayer(new DemoLayer());
	}

	void Destroy() override {
	}

	void Update(Timestamp deltaTime) override {
	}
};

}

// Application Entry-Point
Omnia::Application *CreateApplication() {
	return new Ultra::App();
}
