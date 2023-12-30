export module Ultra.Platform.System.WinAPI.Event;

import Ultra.Core;
import Ultra.System.Event;

export namespace Ultra {

///
/// WinEvents
/// Source: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644958%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
///
class WinEventListener: public EventListener {
public:
    // Default
    WinEventListener() = default;
    virtual ~WinEventListener() = default;

    // Events
    virtual bool Callback(void *event) override;
    virtual void Update() override;

private:
    intptr_t Register(void *event);
};

}
