#pragma once

///
/// @brief Logger Extensions (cause performance matters ... sometimes)
///

// Old-School: If anybody wishes preprocessor macros, we have no problem with it.
#define LOG(...)			Log			("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define LOG_TRACE(...)      LogTrace	("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define LOG_DEBUG(...)      LogDebug	("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define LOG_INFO(...)		LogInfo		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define LOG_WARN(...)		LogWarning	("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define LOG_ERROR(...)		LogError	("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define LOG_FATAL(...)	    LogFatal	("[", __FUNCTION__, "]: ", __VA_ARGS__)

#ifdef APP_MODE_DEBUG
    // Workaround, add the debug break after the message.
    #define AppAssert(x, ...)   if (AppAssert(x, __VA_ARGS__)) APP_DEBUGBREAK()

    #define APP_ASSERT(x, ...)  { if (!(x)) { AppLogFatal("[", __FUNCTION__, "]: ", __VA_ARGS__); APP_DEBUGBREAK(); } }
#elif APP_MODE_RELEASE
    #define AppAssert(...);
    #define LogDebug(...);
    #define LogTrace(...);
    
    #define APP_ASSERT(x, ...);
    #define LOG_DEBUG(...);
    #define LOG_TRACE(...);
#elif APP_MODE_DISTRIBUTION
    #define AppAssert(...);
    #define LogInfo(...);
    #define LogDebug(...);
    #define LogTrace(...);
    
    #define APP_ASSERT(x, ...);
    #define LOG_INFO(...);
    #define LOG_DEBUG(...);
    #define LOG_TRACE(...);
#endif
