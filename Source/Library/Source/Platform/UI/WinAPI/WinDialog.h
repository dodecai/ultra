#pragma once

#include "Omnia/UI/Dialog.h"

namespace Omnia {

class WinDialog: public Dialog {
public:
    WinDialog() = default;
    virtual ~WinDialog() = default;

    string OpenFile(const char *filter) const override;
    string SaveFile(const char *filter) const override;
};

}
