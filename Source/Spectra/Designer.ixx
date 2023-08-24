export module Spectra.Designer;

import Ultra;
import Ultra.Core.Layer;

import Spectra.Component.Menu;
import Spectra.Panel.Browser;
import Spectra.Panel.CodeEditor;
import Spectra.Panel.Console;
import Spectra.Panel.Properties;
import Spectra.Panel.Viewport;

export namespace Ultra {

enum class DesignerState {
    Edit,
    Play,
    Pause,
};

class Designer: public Layer {
public:
    Designer(): Layer("Designer"), mState(DesignerState::Edit) {}
    virtual ~Designer() = default;

    void Create() override {}
    void Update(Timestamp deltaTime) override {}

    // Accessors
    DesignerState GetState() const { return mState; };

    // Mutators
    void SetState(DesignerState state) { mState = state; }

    // Commands
    void Play() {}
    void Pause() {}
    void Stop() {}

    void NewScene() {}
    void OpenScene() {}
    void SaveScene() {}
    void SaveSceneAs() {}
    void CloseScene() {}

    // Events
    void OnKeyboardEvent(KeyboardEventData& data, const EventListener::EventEmitter &emitter) override {}
    void OnMouseEvent(MouseEventData& data, const EventListener::EventEmitter &emitter) override {}
    void OnWindowEvent(WindowEventData& data, const EventListener::EventEmitter &emitter) override {}

private:
    DesignerState mState;
    string mCurrentScene;
};

}
