#pragma once

#include "Ultra/Core/Private/Core.h"
#if __INTELLISENSE__
    #include "Ultra/Core/Private/Types.h"
#endif

// Library
import Ultra.Core;
import Ultra.Core.Application;

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
