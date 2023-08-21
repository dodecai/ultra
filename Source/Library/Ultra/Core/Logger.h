#pragma once

// Workaround, add the debug break after the message.
#define AppAssert(x, ...)   if (AppAssert(x, __VA_ARGS__)) APP_DEBUGBREAK()
