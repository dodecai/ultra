#pragma once

#include "Omnia/Omnia.pch"
#include "Omnia/Core.h"
#include "Omnia/Log.h"
#include "Omnia/Utility/Message.h"

#include "WindowData.h"

namespace Omnia {

class Window {
public:
	// Default
	Window() = default;
	virtual ~Window() = default;
	static Scope<Window> Create(const WindowProperties &properties = WindowProperties());

	// Events
	Subject<bool &, void *> EventCallback;
	virtual void Update() = 0;

	// Accessors
	virtual void *GetNativeWindow() = 0;
	virtual const WindowProperties GetProperties() const = 0;
	virtual const WindowSize GetContexttSize() const = 0;
	virtual const WindowPosition GetDisplayPosition() const = 0;
	virtual const WindowSize GetDisplaySize() const = 0;
	virtual const WindowSize GetScreenSize() const = 0;
	virtual const bool GetState(WindowState state = WindowState::Alive) const = 0;
	virtual const string GetTitle() const = 0;

	// Modifiers
	virtual void SetProperties(const WindowProperties &properties) = 0;
	virtual void SetCursorPosition(const int32_t x, const int32_t y) = 0;
	virtual void SetDisplayPosition(const int32_t x, const int32_t y) = 0;
	virtual void SetDisplaySize(const uint32_t width, const uint32_t height) = 0;
	virtual void SetProgress(const float progress) = 0;
	virtual void SetTitle(const string_view title) = 0;
};

}
