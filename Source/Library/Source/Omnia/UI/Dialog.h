#pragma once

#include "Omnia/Omnia.pch"
#include "Omnia/Core.h"

namespace Omnia {

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
