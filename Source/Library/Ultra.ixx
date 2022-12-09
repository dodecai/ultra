module;

// Configuration
#include "Ultra/Ultra.inl"

export module Ultra;

// Properties
export import Ultra.Core;

// Prime Extensions
export import Ultra.Config;
export import Ultra.Logger;
export import "Ultra/Logger.h";

// Core Extensions
#ifdef LIB_EXTENSION_CORE
    export import Ultra.Application;
    export import Ultra.Core.Layer;
#endif

// Debug Extensions
#ifdef LIB_EXTENSION_DEBUG
    export import Ultra.Debug.Profiler;
#endif

// GFX Extensions
#ifdef LIB_EXTENSION_GFX
#endif

// System Extensions
#ifdef LIB_EXTENSION_SYSTEM
    export import Ultra.System.Cli;
#endif

// UI Extensions
#ifdef LIB_EXTENSION_UI
#endif

// Utility Extensions
#ifdef LIB_EXTENSION_UTILITIES
    export import Ultra.Utility.DateTime;
    export import Ultra.Utility.Delegate;
    export import Ultra.Utility.Enum;
    export import Ultra.Utility.Thread;
    export import Ultra.Utility.Timer;
    export import Ultra.Utility.String;
    export import Ultra.Utility.UUID;
#endif

export namespace Ultra {

void ShowLibraryInfo() {
    logger << "Library Information\n"
        << " - Caption:     " << LibCaption     << "\n"
        << " - Description: " << LibDescription << "\n"
        << " - Release:     " << LibRelease     << "\n"
        << " - Version:     " << LibVersion     << "\n";

    logger << "Library Features\n" << std::boolalpha
        << " - Prime:   " << Features::LibPrimeExtensions   << "\n"
        << " - Core:    " << Features::LibCoreExtensions    << "\n"
        << " - Debug:   " << Features::LibDebugExtensions   << "\n"
        << " - GFX:     " << Features::LibGfxExtensions     << "\n"
        << " - System:  " << Features::LibSystemExtensions  << "\n"
        << " - UI:      " << Features::LibUiExtensions      << "\n"
        << " - Utility: " << Features::LibUtilityExtensions << "\n"
        << std::noboolalpha;

    logger << "\n";
}

}
