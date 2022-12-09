#pragma once

#include "Core.h"

import Ultra.Application;
import Ultra.Types;

///
/// @brief Application EntryPoint
/// Attention: Can be used only once!
///

extern Ultra::Application *Ultra::CreateApplication();

int main(int argc, char **argv) {
    // Preparation
    #ifdef APP_PLATFORM_WINDOWS
        // Switch to UTF-8 codepage
        system("chcp 65001 >nul");
    #endif

    // Initialization
    auto app = Ultra::CreateApplication();
    app->SetArguments(Ultra::Arguments({ argv + 1, argv + argc }));

    // Main
    app->Run();

    // Termination
    delete app;
    return 0;
}
