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

    // Utilities
    export import Ultra.Core.DateTime;
    export import Ultra.Core.Delegate;
    export import Ultra.Core.Dispatcher;
    export import Ultra.Core.Emitter;
    export import Ultra.Core.Future;
    export import Ultra.Core.Random;
    export import Ultra.Core.Signal;
    //export import Ultra.Core.String;
    export import Ultra.Core.ThreadPool;
    export import Ultra.Core.Timer;
#endif

// Debug Extensions
#ifdef LIB_EXTENSION_DEBUG
    //export import Ultra.Debug.Memory;
    //export import Ultra.Debug.Profiler;
#endif

// Engine Extensions
#ifdef LIB_EXTENSION_ENGINE
    export import Ultra.Animation;
    export import Ultra.Asset;
    export import Ultra.Asset.Manager;
    export import Ultra.Math;
    export import Ultra.Media;
    export import Ultra.Physics;
    export import Ultra.Renderer;
    export import Ultra.Scene;
    export import Ultra.Scripting;
    export import Ultra.Serializer;
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
    export import Ultra.UI.HmGui;
    export import Ultra.UI.GUILayer;
#endif

export namespace Ultra {

void ShowLibraryInfo() {
    logger << "Library Information\n"
        << " - Caption:     " << LibCaption     << "\n"
        << " - Description: " << LibDescription << "\n"
        << " - Release:     " << LibRelease     << "\n"
        << " - Version:     " << LibVersion     << "\n";

    logger << "Library Features\n" << std::boolalpha
        << " - Core:    " << Features::LibCoreExtensions    << "\n"
        << " - Debug:   " << Features::LibDebugExtensions   << "\n"
        << " - Engine:  " << Features::LibEngineExtensions  << "\n"
        << " - System:  " << Features::LibSystemExtensions  << "\n"
        << " - UI:      " << Features::LibUiExtensions      << "\n"
        << std::noboolalpha;

    logger << "\n";
}

}
