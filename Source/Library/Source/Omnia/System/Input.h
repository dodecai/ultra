#pragma once

#include <tuple>

#include "Omnia/Core.h"
#include "Omnia/UI/EventData.h"

namespace Omnia {

class Input {
	static Scope<Input> Instance;

public:
	static Scope<Input> Create();
	
	static bool GetKeyState(KeyCode code);
	static bool GetMouseButtonState(MouseButton button);
	static std::pair<float, float> GetMousePosition();

protected:
	virtual bool GetKeyStatePlatform(KeyCode code) const = 0;
	virtual bool GetMouseButtonStatePlatform(MouseButton button) const = 0;
	virtual std::pair<float, float> GetMousePositionPlatform() const = 0;
};

}
