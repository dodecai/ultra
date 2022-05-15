#pragma once

/**
* @brief	This file should only be included by an external application.
*/

// Configuration: The extension was choosen, to hide the file from other source files
#include "Omnia.settings"

// Default Extensions: By the way, the order is important!
#include "Omnia/Core.h"
#include "Omnia/Platform.h"
#include "Omnia/Types.h"

// Prime Extensions: These are needed in nearly every daily development
#if defined(APP_LIBRARY_PRIME_EXTENSIONS)
	#include "Omnia/Config.h"
	#include "Omnia/Log.h"
#endif

// Core Extensions
#if defined(APP_LIBRARY_CORE_EXTENSIONS)
	#include "Omnia/Core/Application.h"
	#include "Omnia/Core/Layer.h"
#endif

// Debug Extensions
#if defined(APP_LIBRARY_DEBUG_EXTENSIONS)
	#include "Omnia/Debug/Instrumentor.h"
	#include "Omnia/Debug/Memory.h"
#endif

// Graphic Extensions
#if defined(APP_LIBRARY_GFX_EXTENSIONS)
	#include "Omnia/GFX/Context.h"
#endif

// System Extensions
#if defined(APP_LIBRARY_SYSTEM_EXTENSIONS)
	#include "Omnia/System/Cli.h"
	#include "Omnia/System/FileSystem.h"
	#include "Omnia/System/Input.h"
#endif

// UI Extensions
#if defined(APP_LIBRARY_UI_EXTENSIONS)
    #include "Omnia/UI/Dialog.h"
    #include "Omnia/UI/Event.h"
    #include "Omnia/UI/GuiLayer.h"
	#include "Omnia/UI/Window.h"
#endif

// Utility Extensions
#if defined(APP_LIBRARY_UTILITIES_EXTENSIONS)
	#include "Omnia/Utility/DateTime.h"
	#include "Omnia/Utility/Enum.h"
    #include "Omnia/Utility/Future.h"
	#include "Omnia/Utility/Message.h"
	#include "Omnia/Utility/Property.h"
    #include "Omnia/Utility/Random.h"
    #include "Omnia/Utility/String.h"
    #include "Omnia/Utility/ThreadPool.h"
	#include "Omnia/Utility/Timer.h"
    #include "Omnia/Utility/UUID.h"
#endif
