export module Ultra.Platform.UI.WinAPI.Dialog;

import Ultra.Core;
import Ultra.UI.Dialog;

export namespace Ultra {

class WinDialog: public Dialog {
public:
    WinDialog() = default;
    virtual ~WinDialog() = default;

    string OpenFile(const char *filter = "All\0*.*\0") const;
    string SaveFile(const char *filter = "All\0*.*\0") const;
};

}
