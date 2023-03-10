export module Ultra.Core.LayerStack;

import Ultra.Core;
import Ultra.Core.Layer;

export namespace Ultra {

class LayerStack {
    vector<Layer *> Layers;
    vector<Layer *>::iterator LayerInsert;
    uint32_t LayerInsertIndex = 0;

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
};

}
