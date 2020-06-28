#include <Ultra.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Ultra {

static const char *sMapTiles =
	"000000000000000000000000000000"
	"000011110000000111000011100000"
	"000001221110001221111122110000"
	"000000122221112222222222210000"
	"000000011222222222222222210000"
	"000000001222222222222222210000"
	"000000111222222222222221000000"
	"000001222222222222222210000000"
	"000012222222222222222100000000"
	"000122222222222222222100000000"
	"000122222222222222222100000000"
	"000012112211222111222210000000"
	"000001001100111000111110000000"
	"000000000000000000000000000000"
;

class Game2D: public Layer {
	OrthographicCameraController SceneCamera;
	glm::vec4 ClearColor;

	std::unordered_map<char, Reference<SubTexture2D>> TextureMap;

	uint32_t sMapWidth = 0;
	uint32_t sMapHeight = 0;

	Reference<Texture2D> SpriteSheet;
	Reference<SubTexture2D> EmptyTexture;
	Reference<SubTexture2D> DirtTexture;
	Reference<SubTexture2D> GrassTexture;
	Reference<SubTexture2D> TreeTexture;
	Reference<SubTexture2D> WaterTexture;

public:
	Game2D(): Layer("Test"),
		ClearColor(0.1f, 0.1f, 0.1f, 1.0f),
		SceneCamera(1.33f, true) {
	}
	virtual ~Game2D() = default;

	void Create() override {
		SceneCamera.SetZoomLevel(8.0f);

		SpriteSheet = Texture2D::Create("./Assets/Textures/RPGpack_sheet_2X.png");
		EmptyTexture = SubTexture2D::CreateFromCoordinates(SpriteSheet, {0, 0}, { 128.0f, 128.0f });

		TextureMap['0'] = WaterTexture = SubTexture2D::CreateFromCoordinates(SpriteSheet, {11, 11}, { 128.0f, 128.0f });
		TextureMap['1'] = DirtTexture = SubTexture2D::CreateFromCoordinates(SpriteSheet, {6, 11}, { 128.0f, 128.0f });
		TextureMap['2'] = GrassTexture = SubTexture2D::CreateFromCoordinates(SpriteSheet, {1, 11}, { 128.0f, 128.0f });
		TextureMap['3'] = TreeTexture = SubTexture2D::CreateFromCoordinates(SpriteSheet, { 2, 1 }, { 128.0f, 128.0f }, { 1, 2 });

		sMapWidth = 32;
		sMapHeight = strlen(sMapTiles) / sMapWidth;
	}

	void GuiUpdate() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Clear Color", glm::value_ptr(ClearColor));
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

		// Draw
		Renderer2D::BeginScene(SceneCamera.GetCamera());

		for (uint32_t y = 0; y < sMapHeight; y++) {
			for (uint32_t x = 0; x < sMapWidth; x++) {
				char type = sMapTiles[x + y * sMapWidth];
				Reference<SubTexture2D> texture;
				if (TextureMap.find(type) != TextureMap.end())
					texture = TextureMap[type];
				else
					texture = EmptyTexture;


				Renderer2D::DrawQuad({ x - (sMapWidth /2), y - (sMapHeight /2), 0.1f }, { 1.0f, 1.0f }, texture);

			}
		}

		Renderer2D::EndScene();
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
};

}
