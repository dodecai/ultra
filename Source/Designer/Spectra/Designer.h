#pragma once

#include <glm/gtx/matrix_decompose.hpp>

#include <Ultra.h>
#include <Ultra/Audio/Audio.h>
#include <Ultra/Script/CameraController.h>

#include "Panel/Panels.h"

//#define RAW_RENDERER_TEST

namespace Ultra {

enum class DesignerState {
    Edit,
    Play,
    Pause
};

class Designer: public Layer {
public:
    Designer();

    void Create() override;
    void Debug();
    void GuiUpdate() override;
    void Update(Timestamp deltaTime) override;
    
    void KeyboardEvent(KeyboardEventData data) override;
    void WindowEvent(WindowEventData data) override;
    
    // Accessors
    static Reference<Scene> GetCurrentScene();

    // Commands
    void CloseScene();
    void NewScene();
    void OpenScene();
    void SaveScene();
    void SaveSceneAs();

private:
    // Properties
    string CurrentSceneFile;

    // Panels
    SceneBrowser Browser;
    ViewportPanel mViewport;
    PropertiesPanel SceneProperties;
    TextEditor ScriptEditor;

    // Scene
    Reference<Scene> CurrentScene;
    Entity CameraEntity;
    Entity BackgroundMusic;

    Ultra::Audio::Source BackgroundMusicPlayer = {};

    glm::vec4 ClearColor;
    glm::vec4 GridColor;
    Reference<Texture2D> GridTexture;
    Reference<Shader> GridShader;

    float QuadSize = 0.5f;
    float GridSteps = 0.27f;
    float GridX = 5.0f;
    float GridY = 5.0f;
    float RoatationSpeed = 5.0f;
};

}
