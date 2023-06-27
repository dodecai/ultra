#pragma once

///
/// @brief Logger Extensions (cause performance matters)
/// Old-School: If anybody wishes preprocessor macros, we have no problem with it.
///

#define LOG(...)			Log			("[", __FUNCTION__, "]: ", __VA_ARGS__)
#ifdef APP_MODE_DEBUG
    #define APP_ASSERT(x, ...)  { if (!(x)) { LogFatal("[", __FUNCTION__, "]: ", __VA_ARGS__); APP_DEBUGBREAK(); } }
    #define LOG_TRACE(...)      LogTrace	("[", __FUNCTION__, "]: ", __VA_ARGS__)
    #define LOG_DEBUG(...)      LogDebug	("[", __FUNCTION__, "]: ", __VA_ARGS__)
    #define LOG_INFO(...)		LogInfo		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#elif APP_MODE_DISTRIBUTION
    #define APP_ASSERT(x, ...)
    #define LOG_TRACE(...);
    #define LOG_DEBUG(...);
    #define LOG_INFO(...)		LogInfo		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#elif APP_MODE_RELEASE
    #define APP_ASSERT(x, ...);
    #define LOG_TRACE(...);
    #define LOG_DEBUG(...);
    #define LOG_INFO(...);
#endif
#define LOG_WARN(...)		LogWarning	("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define LOG_ERROR(...)		LogError	("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define LOG_FATAL(...)	    LogFatal	("[", __FUNCTION__, "]: ", __VA_ARGS__)
