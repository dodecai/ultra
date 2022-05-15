#pragma once

#include "Omnia/Core.h"
#include "Omnia/UI/Event.h"
#include "Omnia/Utility/Timer.h"

namespace Omnia {

class Layer {
protected:
	string Name;

public:
	Layer(const string &name = "Layer"): Name { name } {}
	virtual ~Layer() = default;

	virtual void Attach() {}
	virtual void Detach() {}

	virtual void Create() {}
	virtual void Destroy() {}
	virtual void GuiUpdate() {}
	virtual void Update(Timestamp deltaTime) {}

	inline const string &GetName() const { return Name; }

	virtual void Event(void *event) {}
	virtual void ControllerEvent(ControllerEventData data) {}
	virtual void KeyboardEvent(KeyboardEventData data) {}
	virtual void MouseEvent(MouseEventData data) {}
	virtual void TouchEvent(TouchEventData data) {}
	virtual void WindowEvent(WindowEventData data) {}
};

}
