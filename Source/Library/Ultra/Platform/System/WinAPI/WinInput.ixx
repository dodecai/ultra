export module Ultra.Platform.System.WinAPI.Input;

import Ultra.System.Input;

export namespace Ultra {

class WinInput: public Input {
protected:
    virtual bool GetKeyStatePlatform(KeyCode code) const override;
    virtual bool GetMouseButtonStatePlatform(MouseButton button) const override;
    virtual std::pair<float, float> GetMousePositionPlatform() const override;
};

}
