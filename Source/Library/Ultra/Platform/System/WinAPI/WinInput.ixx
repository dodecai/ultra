export module Ultra.Platform.System.WinAPI.Input;

import Ultra.System.Input;

export namespace Ultra {

class WinInput: public Input {
protected:
    bool GetKeyStatePlatform(KeyCode code) const override;
    bool GetMouseButtonStatePlatform(MouseButton button) const override;
    std::pair<float, float> GetMousePositionPlatform() const override;
    std::pair<float, float> GetMousePositionDeltaPlatform() const override;
    float GetMouseWheelDeltaPlatform() const override;
};

}
