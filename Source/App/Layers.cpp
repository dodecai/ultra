#include <Ultra.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Ultra {

// 3D Renderer
class Layer3D: public Layer {
public:
    Layer3D():
		Layer("Test"),
		ClearColor(0.1f, 0.1f, 0.1f, 1.0f),
		SceneCamera(1.33f, true),
		PlayerPosition(0.0f) {
	}
	virtual ~Layer3D() = default;

	void Create() override {
		Shaders.Load("./Assets/Shaders/Grid.vert", "./Assets/Shaders/Grid.frag");
		Shaders.Load("./Assets/Shaders/Player.vert", "./Assets/Shaders/Player.frag");
		Shaders.Load("./Assets/Shaders/Texture.glsl");

		// Grid
		BufferLayout gridLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TextCoord" },
		};
		GridVertexArray = VertexArray::Create();

		Reference<VertexBuffer> gridVB;
		float gridVertices[5 * 4] = {
			-0.5f,  -0.5f,  0.0f,	0.0f, 0.0f,
			0.5f,  -0.5f,  0.0f,	1.0f, 0.0f,
			0.5f,   0.5f,  0.0f,	1.0f, 1.0f,
			-0.5f,   0.5f,  0.0f,	0.0f, 1.0f,
		};
		gridVB = VertexBuffer::Create(gridVertices, sizeof(gridVertices));
		gridVB->SetLayout(gridLayout);
		GridVertexArray->AddVertexBuffer(gridVB);

		Reference<IndexBuffer> gridIB;
		uint32_t gridIndices[6] = { 0, 1, 2, 2, 3, 0 };
		gridIB = IndexBuffer::Create(gridIndices, sizeof(gridIndices) / sizeof(uint32_t));
		GridVertexArray->SetIndexBuffer(gridIB);

		// Player
		BufferLayout playerLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};
		PlayerVertexArray = VertexArray::Create();

		Reference<VertexBuffer> playerVB;
		float playerVertices[7 * 3] = {
			-0.5f,  -0.5f,  0.0f,	0.8f, 0.2f, 0.2f, 1.0f,
			0.5f,  -0.5f,  0.0f,	0.2f, 0.8f, 0.2f, 1.0f,
			0.0f,   0.5f,  0.0f,	0.2f, 0.2f, 0.8f, 1.0f
		};
		playerVB = VertexBuffer::Create(playerVertices, sizeof(playerVertices));
		playerVB->SetLayout(playerLayout);
		PlayerVertexArray->AddVertexBuffer(playerVB);

		Reference<IndexBuffer> playerIB;
		uint32_t playerIndices[3] = { 0, 1, 2 };
		playerIB = IndexBuffer::Create(playerIndices, sizeof(playerIndices) / sizeof(uint32_t));
		PlayerVertexArray->SetIndexBuffer(playerIB);

		// Texture Test
		TextureBase = Texture2D::Create("./Assets/Textures/Checkerboard.png");
		TextureLogo = Texture2D::Create("./Assets/Textures/AlphaChannel.png");
		Shaders.Get("Texture")->Bind();
		Shaders.Get("Texture")->SetInt("u_Texture", 0);
	}

	void GuiUpdate() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Clear Color", glm::value_ptr(ClearColor));
		ImGui::End();
	}

	void Update(Timestamp deltaTime) override {
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
		SceneCamera.Update(deltaTime);

		// Render
		RenderCommand::SetClearColor(ClearColor);
		RenderCommand::Clear();
		Renderer::BeginScene(SceneCamera.GetCamera());

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

	void MouseEvent(MouseEventData data) override {
		if (data.Action == MouseAction::Wheel) {
			SceneCamera.MouseEvent(data);
		}
	}

	void WindowEvent(WindowEventData data) override {
		if (data.Action == WindowAction::Resize) {
			Renderer::Resize(data.Width, data.Height);
			SceneCamera.Resize(data.Width, data.Height);
		}
	}

private:
    glm::vec4 ClearColor;

    CameraController SceneCamera;
    ShaderLibrary Shaders;

    Reference<VertexArray> GridVertexArray;

    Reference<Texture2D> TextureBase;
    Reference<Texture2D> TextureLogo;

    Reference<VertexArray> PlayerVertexArray;
    glm::vec3 PlayerPosition;
    float PlayerMoveSpeed = 1.0f;
};

// 2D Renderer
class Layer2D: public Layer {
public:
    Layer2D(): Layer("Test"),
		ClearColor(0.1f, 0.1f, 0.1f, 1.0f),
		GridColor(0.8f, 0.8f, 0.2f, 0.72f),
		SceneCamera(1.33f, true) {
	}
	virtual ~Layer2D() = default;

	void Create() override {
		GridTexture = Texture2D::Create("./Assets/Textures/Checkerboard.png");

		Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		Particle.SizeBegin = 0.5f;
		Particle.SizeVariation = 0.3f;
		Particle.SizeEnd = 0.0f;
		Particle.LifeTime = 10.0f;
		Particle.Velocity = { 0.0f, 0.0f };
		Particle.VelocityVariation = { 3.0f, 1.0f };
		Particle.Position = { 0.0f, 0.0f };
	}

	void GuiUpdate() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Clear Color", glm::value_ptr(ClearColor));
		ImGui::ColorEdit4("Grid Color", glm::value_ptr(GridColor));
		ImGui::End();

		ImGui::Begin("Statistics");
		ImGui::Text("2D Renderer");
		ImGui::Text("DrawCalls: %d", Renderer2D::GetStatistics().DrawCalls);
		ImGui::Text("Triangles: %d", Renderer2D::GetStatistics().QuadCount * 2);
		ImGui::Text("Vertices: %d", Renderer2D::GetStatistics().GetTotalVertexCount());
		ImGui::Text("Indices: %d", Renderer2D::GetStatistics().GetTotalIndexCount());
		ImGui::End();
	}

	void Update(Timestamp deltaTime) override {
		Renderer2D::ResetStatistics();

		// Preparation
		// Camera
		SceneCamera.Update(deltaTime);
		// Renderer
		RenderCommand::SetClearColor(ClearColor);
		RenderCommand::Clear();
		
		static float rotation = 0.0f;
		rotation += 1.0f;

		// Draw
		Renderer2D::StartScene(SceneCamera.GetCamera());

		Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 32.0f, 32.0f }, GridTexture);

		// Low performance Grid
		float steps = 0.27f;
		for (float y = -5; y < +5; y += steps) {
			for (float x = -5; x < +5; x += steps) {
				glm::vec4 color = { (x + 5.0f) / 10.f, 0.2f, (y +5.0f) / 10.0f, 0.72f };
				Renderer2D::DrawQuad({ x, y, -0.05f }, { steps * 0.72f, steps * 0.72f }, color);
			}
		}

		Renderer2D::DrawQuad({ -0.2f, -0.2f }, { 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f });
		Renderer2D::DrawQuad({ 0.2f, 0.2f }, { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f });
		Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 1.0f }, { 0.2f, 0.2f }, glm::radians(rotation), { 0.0f, 1.0f, 0.0f, 0.72f });
		Renderer2D::FinishScene();

		// Particle Tests
		if (Input::GetMouseButtonState(MouseButton::Left)) {
			auto [x, y] = Input::GetMousePosition();
			auto [width, height] = Ultra::Application::Get().GetWindow().GetDisplaySize();
			auto bounds = SceneCamera.GetBounds();
			auto pos = SceneCamera.GetCamera().GetPosition();
			
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 10; i++) Particles.Emit(Particle);

		}
		Particles.Update(deltaTime);
		Particles.Render(SceneCamera.GetCamera());
	}

	void Destroy() override {	}

	void KeyboardEvent(KeyboardEventData data) override {
		SceneCamera.KeyboardEvent(data);
	}

	void MouseEvent(MouseEventData data) override {
		if (data.Action == MouseAction::Wheel) {
			SceneCamera.MouseEvent(data);
		}
	}

	void WindowEvent(WindowEventData data) override {
		if (data.Action == WindowAction::Resize) {
			Renderer::Resize(data.Width, data.Height);
			SceneCamera.Resize(data.Width, data.Height);
		}
	}

private:
    CameraController SceneCamera;
    glm::vec4 ClearColor;
    glm::vec4 GridColor;

    Reference<Texture2D> GridTexture;

    ParticleProperties Particle;
    ParticleSystem Particles;
};

}
