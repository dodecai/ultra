#pragma once

#include "Omnia/System/Input.h"

namespace Omnia {

class WinInput: public Input {
protected:
	virtual bool GetKeyStatePlatform(KeyCode code) const override;
	virtual bool GetMouseButtonStatePlatform(MouseButton button) const override;
	virtual std::pair<float, float> GetMousePositionPlatform() const override;
};

}
