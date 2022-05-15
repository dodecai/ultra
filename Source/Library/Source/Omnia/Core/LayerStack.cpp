#include "LayerStack.h"

namespace Omnia {

LayerStack::~LayerStack() {
	for (Layer *layer : Layers) {
		layer->Detach();
		delete layer;
	}
}

void LayerStack::PushLayer(Layer *layer) {
	Layers.emplace(Layers.begin() + LayerInsertIndex, layer);
	LayerInsertIndex++;
	//layer->Attach();
}

void LayerStack::PushOverlay(Layer *overlay) {
	Layers.emplace_back(overlay);
	//overlay->Attach();
}

void LayerStack::PopLayer(Layer *layer) {
	auto it = std::find(Layers.begin(), Layers.begin() + LayerInsertIndex, layer);
	if (it != Layers.begin() + LayerInsertIndex) {
		layer->Detach();
		Layers.erase(it);
		LayerInsertIndex--;
	}
}

void LayerStack::PopOverlay(Layer *overlay) {
	auto it = std::find(Layers.begin() + LayerInsertIndex, Layers.begin() , overlay);
	if (it != Layers.begin() + LayerInsertIndex) {
		overlay->Detach();
		Layers.erase(it);
	}
}

}
