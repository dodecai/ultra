module;

// Configuration
#include "Ultra/Ultra.inl"

export module Ultra;

// Properties
export import Ultra.Core;

// Prime Extensions
export import Ultra.Config;
export import Ultra.Logger;

// Core Extensions
#ifdef LIB_EXTENSION_CORE
    export import Ultra.Core.Application;
    export import Ultra.Core.Layer;
#endif

// Debug Extensions
#ifdef LIB_EXTENSION_DEBUG
    //export import Ultra.Debug.Memory;
    //export import Ultra.Debug.Profiler;
#endif

// GFX Extensions
#ifdef LIB_EXTENSION_GFX
    export import Ultra.GFX.Context;
    export import Ultra.Engine;
    export import Ultra.Math;
#endif

// System Extensions
#ifdef LIB_EXTENSION_SYSTEM
    export import Ultra.System.Cli;
    export import Ultra.System.FileSystem;
    export import Ultra.System.Input;
    export import Ultra.System.Library;
#endif

// UI Extensions
#ifdef LIB_EXTENSION_UI
    export import Ultra.UI.GUILayer;
    export import Ultra.Math;
    export import Ultra.Engine.Resource;
    export import Ultra.Engine.Renderer.Viewport;
    export import Ultra.Engine.UIRenderer;
    export import Ultra.UI.HmGui;
#endif

// Utility Extensions
#ifdef LIB_EXTENSION_UTILITIES
    export import Ultra.Utility.DateTime;
    export import Ultra.Utility.Delegate;
    export import Ultra.Utility.Dispatcher;
    export import Ultra.Utility.Emitter;
    export import Ultra.Utility.Future;
    export import Ultra.Utility.Random;
    export import Ultra.Utility.Signal;
    export import Ultra.Utility.String;
    export import Ultra.Utility.ThreadPool;
    export import Ultra.Utility.Timer;
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
