export module Ultra.Core.Layer;

import Ultra.Core;
import Ultra.Core.Event.Data;
import Ultra.System.Event;

export namespace Ultra {

///
/// @brief Simple Application Layer Implementation
///
class Layer {
public:
    Layer(const string &name = "Layer"): mName(name) {}
    virtual ~Layer() = default;

    virtual void Attach() {}
    virtual void Detach() {}

    virtual void Create() {}
    virtual void Destroy() {}
    virtual void GuiUpdate() {}
    virtual void Update([[maybe_unused]] Timestamp deltaTime) {}

    inline const string &GetName() const { return mName; }

    // EventListener
    [[maybe_unused]] virtual void OnControllerEvent([[maybe_unused]] ControllerEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}
    [[maybe_unused]] virtual void OnKeyboardEvent([[maybe_unused]] KeyboardEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}
    [[maybe_unused]] virtual void OnMouseEvent([[maybe_unused]] MouseEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}
    [[maybe_unused]] virtual void OnTouchEvent([[maybe_unused]] TouchEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}
    [[maybe_unused]] virtual void OnWindowEvent([[maybe_unused]] WindowEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}

    [[maybe_unused]] virtual void OnDeviceEvent([[maybe_unused]] DeviceEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}
    [[maybe_unused]] virtual void OnPowerEvent([[maybe_unused]] PowerEventData &data, [[maybe_unused]] const EventListener::EventEmitter &emitter) {}

protected:
    string mName;
};

///
/// @brief Container for the Application Layers
///
class LayerStack {
public:
    LayerStack() = default;
    ~LayerStack() {
        for (auto *layer : Layers) {
            layer->Detach();
            delete layer;
        }
    }

    void PushLayer(Layer *layer) {
        Layers.emplace(Layers.begin() + LayerInsertIndex, layer);
        LayerInsertIndex++;
        layer->Attach();
    }
    void PushOverlay(Layer *overlay) {
        Layers.emplace_back(overlay);
        overlay->Attach();
    }
    void PopLayer(Layer *layer) {
        auto it = std::find(Layers.begin(), Layers.begin() + LayerInsertIndex, layer);
        if (it != Layers.begin() + LayerInsertIndex) {
            layer->Detach();
            Layers.erase(it);
            LayerInsertIndex--;
        }
    }
    void PopOverlay(Layer *overlay) {
        auto it = std::find(Layers.begin() + LayerInsertIndex, Layers.begin(), overlay);
        if (it != Layers.begin() + LayerInsertIndex) {
            overlay->Detach();
            Layers.erase(it);
        }
    }

    auto begin() { return Layers.begin(); }
    auto end() { return Layers.end(); }
    auto rbegin() { return Layers.rbegin(); }
    auto rend() { return Layers.rend(); }

    auto begin() const { return Layers.begin(); }
    auto end() const { return Layers.end(); }
    auto rbegin() const { return Layers.rbegin(); }
    auto rend() const { return Layers.rend(); }

private:
    uint32_t LayerInsertIndex = 0;
    vector<Layer *> Layers;
    vector<Layer *>::iterator LayerInsert;
};

}
