#include "GLContext.h"

#include "Omnia/Log.h"
#include "Omnia/UI/Window.h"

#include <glad/gl.h>

#if defined(APP_PLATFORM_WINDOWS)
	#define VC_EXTRALEAN
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#undef APIENTRY
	#include <Windows.h>

	#pragma comment(lib, "opengl32.lib")
	#include <GL/glext.h>
	#include "GL/wglext.h"
#endif

#if !defined(GL_SR8_EXT)
	#define GL_SR8_EXT 0x8FBD
#endif
#if !defined(GL_SRG8_EXT)
	#define GL_SRG8_EXT 0x8FBE
#endif
#if !defined(EGL_OPENGL_ES3_BIT)
	#define EGL_OPENGL_ES3_BIT 0x0040
#endif

// Extensions
#if defined(APP_PLATFORM_WINDOWS)
	inline PROC GetExtension(const char *functionName) { return wglGetProcAddress(functionName); }

	typedef HGLRC WINAPI wglCreateContextAttribsARB_t(HDC hdc, HGLRC hShareContext, const int *attribList);
	wglCreateContextAttribsARB_t *wglCreateContextAttribsARB;

	typedef BOOL WINAPI wglChoosePixelFormatARB_t(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
	wglChoosePixelFormatARB_t *wglChoosePixelFormatARB;

	typedef BOOL WINAPI wglSwapIntervalEXT_t(int interval);
	wglSwapIntervalEXT_t *wglSwapIntervalEXT;
#endif

namespace Omnia {

static void GLAPIENTRY GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	switch (type) {
		case GL_DEBUG_TYPE_ERROR:					{ applog << Log::Error;		break; }
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:		{ applog << Log::Warning;	break; }
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:		{ applog << Log::Warning;	break; }
		case GL_DEBUG_TYPE_PORTABILITY:				{ applog << Log::Trace;		break; }
		case GL_DEBUG_TYPE_PERFORMANCE:				{ applog << Log::Trace;		break; }
		case GL_DEBUG_TYPE_OTHER:					{ applog << Log::Trace;		break; }
		default:									{ applog << Log::Critical;	break; }
	}
	applog << "[Context::GL]" << message << "{" <<
		"ID:"		<< id		<< " | " <<
		"Source:"	<< source	<< " | " <<
		"Severity:" << severity	<<
	"}\n";
}

struct ContextData {
	#if defined(APP_PLATFORM_WINDOWS)
	HWND hWindow;
	HDC hDeviceContext;
	HGLRC hRenderingContext;
	#endif
};

bool GLContext::GetExtensions(int a) {
	bool result = true;
	#if defined(APP_PLATFORM_WINDOWS)
		// Dummy Class
		WNDCLASSEX dClass = {.cbSize = sizeof(WNDCLASSEX), .style =  CS_OWNDC | CS_HREDRAW | CS_VREDRAW, .lpfnWndProc = DefWindowProcA, .hInstance = GetModuleHandle(NULL), .lpszClassName = "DummyClassGL",};
		if (!RegisterClassEx(&dClass)) { result = false; }

		// Dummy Window
		HWND dWindow = CreateWindowEx(NULL, dClass.lpszClassName, "DummyWindowGL", WS_POPUP | WS_DISABLED, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, dClass.hInstance, NULL);
		if (!dWindow) { result = false; }

		// Dummy Device Context
		HDC dDeviceContext = GetDC(dWindow);
		PIXELFORMATDESCRIPTOR pfd = {.nSize = sizeof(PIXELFORMATDESCRIPTOR), .nVersion = 1, .dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL, .iPixelType = PFD_TYPE_RGBA, .cColorBits = 32, .cDepthBits = 24, .cStencilBits = 8, .iLayerType = PFD_MAIN_PLANE};
		int pixelFormat = ChoosePixelFormat(dDeviceContext, &pfd);
		if (!pixelFormat) { result = false; }
		if (!SetPixelFormat(dDeviceContext, pixelFormat, &pfd)) { result = false; }

		// Dummy Render Context
		HGLRC dRenderContext = wglCreateContext(dDeviceContext);
		if (!dRenderContext) { result = false; }
		if (!wglMakeCurrent(dDeviceContext, dRenderContext)) { result = false; }

		// Load Extensions
		wglCreateContextAttribsARB = (wglCreateContextAttribsARB_t *)wglGetProcAddress("wglCreateContextAttribsARB");
		wglChoosePixelFormatARB = (wglChoosePixelFormatARB_t *)wglGetProcAddress("wglChoosePixelFormatARB");
		wglSwapIntervalEXT = (wglSwapIntervalEXT_t *)wglGetProcAddress("wglSwapIntervalEXT");

		// Cleanup
		if (dDeviceContext) wglMakeCurrent(dDeviceContext, 0);
		if (dRenderContext) wglDeleteContext(dRenderContext);
		if (dWindow && dDeviceContext) ReleaseDC(dWindow, dDeviceContext);
		if (dWindow) DestroyWindow(dWindow);
	#endif
	return result;
}


GLContext::GLContext(void *window) {
	// Prepare
	Data = new ContextData();
	ContextProperties properties;

	Data->hWindow = reinterpret_cast<HWND>(window);

	// Check Compatibility Flag
	auto compatibility = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
	if (properties.Compatible) compatibility = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
	// Each version has a different set of majors and minors
	switch (properties.VersionMajor) {
		case 1:
			properties.VersionMinor = {properties.VersionMinor, 0, 5};
			break;
		case 2:
			properties.VersionMinor = {properties.VersionMinor, 0, 1};
			break;
		case 3:
			properties.VersionMinor = {properties.VersionMinor, 0, 3};
			break;
		case 4:
			properties.VersionMinor = {properties.VersionMinor, 0, 6};
			break;
		default:
			properties.VersionMajor = 4;
			properties.VersionMinor = 6;
			applog << Log::Error << "[Context::GL]: Unknown version specified, using default version!" << "\n";
			break;
	}

	// Platform specific stuff
	#if defined(APP_PLATFORM_WINDOWS)
        // This should be only triggered once during application lifecycle
        static bool once = true;
		if(!GetExtensions() && once) { AppLogCritical("[Context::GL]: Could not load OpenGL extensions!"); return; }
        once = false;

		// Get Device Context
		Data->hDeviceContext = GetDC(Data->hWindow);
		if (!Data->hDeviceContext) { AppLogCritical("[Context::GL]: Error occured while acquiring device context!"); return; }
		
		/* Legacy Code (it took a lot of effort to get used to it, so I will leave it here forever) */
		// Drawing Surface Pixel Format: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-pixelformatdescriptor
		//PIXELFORMATDESCRIPTOR pfDrawingSurface = {
		//	.nSize = sizeof(PIXELFORMATDESCRIPTOR),
		//	.nVersion = 1,									// Version
		//	.dwFlags =	PFD_DRAW_TO_WINDOW |				// The buffer can draw to a window or device surface.
		//				PFD_DOUBLEBUFFER |					// The buffer is double-buffered.
		//				PFD_SUPPORT_OPENGL |				// The buffer supports OpenGL drawing.
		//				PFD_SWAP_LAYER_BUFFERS,				// Indicates whether a device can swap individual layer planes with pixel formats that include double-buffered overlay or underlay planes.
		//	.iPixelType = PFD_TYPE_RGBA,					// Each pixel has four components in this order: red, green, blue, and alpha.
		//	.cColorBits = (BYTE)properties.ColorDepth,		// Color Depth
		//	.cRedBits = 0,									// Specifies the number of red bitplanes in each RGBA color buffer.
		//	.cRedShift = 0,									// Specifies the shift count for red bitplanes in each RGBA color buffer.
		//	.cGreenBits = 0,								// Specifies the number of green bitplanes in each RGBA color buffer.
		//	.cGreenShift = 0,								// Specifies the shift count for green bitplanes in each RGBA color buffer.
		//	.cBlueBits = 0,									// Specifies the number of blue bitplanes in each RGBA color buffer.
		//	.cBlueShift = 0,								// Specifies the shift count for blue bitplanes in each RGBA color buffer.
		//	.cAlphaBits = 0,								// Specifies the number of alpha bitplanes in each RGBA color buffer
		//	.cAlphaShift = 0,								// Specifies the shift count for alpha bitplanes in each RGBA color buffer.
		//	.cAccumBits = 0,								// Specifies the total number of bitplanes in the accumulation buffer.
		//	.cAccumRedBits = 0,								// Specifies the number of red bitplanes in the accumulation buffer.
		//	.cAccumGreenBits = 0,							// Specifies the number of green bitplanes in the accumulation buffer.
		//	.cAccumBlueBits = 0,							// Specifies the number of blue bitplanes in the accumulation buffer.
		//	.cAccumAlphaBits = 0,							// Specifies the number of alpha bitplanes in the accumulation buffer.
		//	.cDepthBits = (BYTE)properties.DepthBuffer,		// Depth Buffer (Z-Buffer)
		//	.cStencilBits = (BYTE)properties.StencilBuffer,	// Stencil Buffer
		//	.cAuxBuffers = 0,								// Auxiliary Buffers
		//	.iLayerType = PFD_MAIN_PLANE,					// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used.
		//	.bReserved = 0,									// Specifies the number of overlay and underlay planes. Bits 0 through 3 specify up to 15 overlay planes and bits 4 through 7 specify up to 15 underlay planes.
		//	.dwLayerMask = 0,								// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used.
		//	.dwVisibleMask = 0,								// Specifies the transparent color or index of an underlay plane. When the pixel type is RGBA, dwVisibleMask is a transparent RGB color value.
		//	.dwDamageMask = 0								// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used.
		//};
		//pixelFormat = ChoosePixelFormat(Data->hDeviceContext, &pfDrawingSurface);

		// Describe GL-Context
        PIXELFORMATDESCRIPTOR pfDescription = {};
		int pixelFormats = 0;
		unsigned int formatsMax = 1;
		unsigned int formatsCount = 2;
		int pixelAttribIList[] = {
			WGL_DOUBLE_BUFFER_ARB,			GL_TRUE,
			WGL_DRAW_TO_WINDOW_ARB,			GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,			GL_TRUE,
			WGL_ACCELERATION_ARB,			WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB,				WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,				properties.ColorDepth,
			WGL_ALPHA_BITS_ARB,				properties.AlphaDepth,
			WGL_DEPTH_BITS_ARB,				properties.DepthBuffer,
			WGL_STENCIL_BITS_ARB,			properties.StencilBuffer,
			WGL_SAMPLE_BUFFERS_ARB,			properties.MSAABuffer,
			WGL_SAMPLES_ARB,				properties.MSAASamples,
			WGL_TRANSPARENT_ARB,			GL_TRUE,
			0,
		};
		wglChoosePixelFormatARB(Data->hDeviceContext, pixelAttribIList, NULL, formatsMax, &pixelFormats, &formatsCount);
		if (!formatsCount) { AppLogCritical("[Context::GL]: Error no suiteable pixel format found!"); return; }
		DescribePixelFormat(Data->hDeviceContext, pixelFormats, sizeof(pfDescription), &pfDescription);
		if (!SetPixelFormat(Data->hDeviceContext, pixelFormats, &pfDescription)) { AppLogCritical("[Context::GL]: Error setting pixel format failed {Code:", GetLastError(), "}!"); return; }

		// Create GL-Context
        int flags = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
        #ifdef APP_DEBUG_MODE
            flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
        #endif
		int glContextAttributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,	properties.VersionMajor,
			WGL_CONTEXT_MINOR_VERSION_ARB,	properties.VersionMinor,
            WGL_CONTEXT_PROFILE_MASK_ARB,   compatibility,
			WGL_CONTEXT_FLAGS_ARB,			flags,
            0            
		};
		Data->hRenderingContext = wglCreateContextAttribsARB(Data->hDeviceContext, NULL, glContextAttributes);
		if (!Data->hRenderingContext) { AppLogCritical("[Context::GL]: Error occured while creating GL-Context!"); return; }
	#endif
}

GLContext::~GLContext() {
	#if defined(APP_PLATFORM_WINDOWS)
		wglMakeCurrent(Data->hDeviceContext, NULL);
		wglDeleteContext(Data->hRenderingContext);
	#endif
	delete Data;
    gladLoaderUnloadGL();
}


void GLContext::Attach() {
	#if defined(APP_PLATFORM_WINDOWS)
		if(!IsCurrentContext()) wglMakeCurrent(Data->hDeviceContext, Data->hRenderingContext);
	#endif
}

void GLContext::Detach() {
	#if defined(APP_PLATFORM_WINDOWS)
		if(IsCurrentContext()) wglMakeCurrent(Data->hDeviceContext, NULL);
	#endif
}

void GLContext::Load() {
	if (!gladLoaderLoadGL()) {
		AppLogCritical("[Context: Failed to load OpenGL!");
		return;
	}
    #ifdef APP_DEBUG_MODE
    if (glDebugMessageCallback) {
        glEnable(GL_DEBUG_OUTPUT);
        //glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLMessageCallback, 0);
        //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    } else {
        AppLogWarning("[Ultra::RendererAPI::GL]: ", "The feature 'DebugMessageCallback' isn't available!");
    }
    #endif
}


void *GLContext::GetNativeContext() {
	return reinterpret_cast<void *>(Data->hRenderingContext);
	return nullptr;
}

bool const GLContext::IsCurrentContext() {
	#if defined(APP_PLATFORM_WINDOWS)
		return (wglGetCurrentContext() == Data->hRenderingContext);
	#endif
	return false;
}


void GLContext::SetViewport(uint32_t width, uint32_t height, int32_t x, int32_t y) {
	glViewport(x, y, width, height);
}

void GLContext::SetVSync(bool activate) {
	wglSwapIntervalEXT(activate ? 1 : 0);
}

void GLContext::SwapBuffers() {
	#if defined(APP_PLATFORM_WINDOWS)
		::SwapBuffers(Data->hDeviceContext);
	#endif
}

}
