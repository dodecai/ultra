#pragma once

///
/// @brief Logger Extensions (cause performance matters ... sometimes)
///

// Old-School: If anybody wishes preprocessor macros, we have no problem with it.
#define APP_LOG(...)			AppLog			("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_TRACE(...)      AppLogTrace		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_DEBUG(...)      AppLogDebug		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_INFO(...)		AppLogInfo		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_WARN(...)		AppLogWarning	("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_ERROR(...)		AppLogError		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_FATAL(...)	    AppLogFatal	    ("[", __FUNCTION__, "]: ", __VA_ARGS__)

#ifdef APP_MODE_DEBUG
    // Workaround, add the debug break after the message.
    #define AppAssert(x, ...) if (AppAssert(x, __VA_ARGS__)) APP_DEBUGBREAK()
    #define APP_ASSERT(x, ...) { if (!(x)) { AppLogFatal("[", __FUNCTION__, "]: ", __VA_ARGS__); APP_DEBUGBREAK(); } }
#elif APP_MODE_RELEASE
    #define AppAssert(...);
    #define AppLogDebug(...);
    #define AppLogTrace(...);
    
    #define APP_ASSERT(x, ...);
    #define APP_LOG_DEBUG(...);
    #define APP_LOG_TRACE(...);
#elif APP_MODE_DISTRIBUTION
    #define AppAssert(...);
    #define AppLogInfo(...);
    #define AppLogDebug(...);
    #define AppLogTrace(...);
    
    #define APP_ASSERT(x, ...);
    #define APP_LOG_INFO(...);
    #define APP_LOG_DEBUG(...);
    #define APP_LOG_TRACE(...);
#endif
