export module Ultra.UI.Dialog;

import Ultra.Core;

export namespace Ultra {

class Dialog {
public:
    // Default
    Dialog() = default;
    virtual ~Dialog() = default;
    static Scope<Dialog> Create();

    virtual string OpenFile(const char *filter = "All\0*.*\0") const = 0;
    virtual string SaveFile(const char *filter = "All\0*.*\0") const = 0;
};

}
