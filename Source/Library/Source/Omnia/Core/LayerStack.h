#pragma once

#include "Omnia/Omnia.pch"
#include "Layer.h"

namespace Omnia {

class LayerStack {
	vector<Layer *> Layers;
	vector<Layer *>::iterator LayerInsert;
	uint32_t LayerInsertIndex = 0;

public:
	LayerStack() = default;
	~LayerStack();

	void PushLayer(Layer *layer);
	void PushOverlay(Layer *overlay);
	void PopLayer(Layer *layer);
	void PopOverlay(Layer *overlay);

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
