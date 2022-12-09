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
        for (Layer *layer : Layers) {
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

    vector<Layer *>::iterator begin() { return Layers.begin(); }
    vector<Layer *>::iterator end() { return Layers.end(); }
    vector<Layer *>::reverse_iterator rbegin() { return Layers.rbegin(); }
    vector<Layer *>::reverse_iterator rend() { return Layers.rend(); }

    vector<Layer *>::const_iterator begin() const { return Layers.begin(); }
    vector<Layer *>::const_iterator end() const { return Layers.end(); }
    vector<Layer *>::const_reverse_iterator rbegin() const { return Layers.rbegin(); }
    vector<Layer *>::const_reverse_iterator rend() const { return Layers.rend(); }
};

}
