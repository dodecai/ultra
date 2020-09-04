#pragma once

#include <glm/glm.hpp>

#include "Ultra.pch"
#include "Ultra/Core.h"

#include "VertexArray.h"

namespace Ultra {

// ToDo: RenderAPIData.h?
using RendererID = uint32_t;

enum class PrimitiveType {
    Null            = 0x0,
    Line            = 0x1,
    Circle          = 0x2,
    Triangle        = 0x3,
};

enum class PolygonMode {
    Null            = 0x0,
    Solid           = 0x1,
    Wireframe       = 0x2,
};

enum class Renderers {
    Null            = 0x0,
    DirectX         = 0x1,
    OpenGL          = 0x2,
    Vulkan          = 0x3,
};

struct RendererCapabilities {
	string Vendor;
	string Model;
	string Version;
	string SLVersion;
	string Extensions[2];

	float MaxAnisotropy = 0.0f;
	int MaxSamples = 0;
	int MaxTextureUnits = 0;

	void Log() {
		AppLogInfo("[Renderer] Vendor:  '", Vendor, "'");
		AppLogInfo("[Renderer] Model:   '", Model, "'");
		AppLogInfo("[Renderer] Version: '", Version, "'");
		AppLogDebug("[Renderer] Shader Version: '", SLVersion, "'");
		AppLogTrace("[Renderer] Extensions: '", Extensions[0], " | ", Extensions[1], "'");
		AppLogTrace("[Renderer] Max Anisotropy: '", MaxAnisotropy, "'");
		AppLogTrace("[Renderer] Max Samples: '", MaxSamples, "'");
		AppLogTrace("[Renderer] Max TextureUnits: '", MaxTextureUnits, "'");
	}
};


class RendererAPI {
public:
	// Enumerations
	enum class API {
		Null		= 0x0,
		OpenGL		= 0x1,
	};

	enum class Type {
		Null	= 0,
		Triangles,
		Lines,
	};

public:
	// Default
	static RendererAPI *Create();
	virtual void Load() = 0;
	virtual void Unload() = 0;

	//
	virtual void Clear() = 0;
	virtual void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t indexCount = 0) = 0;
	virtual void DrawIndexed(uint32_t count, Type type, bool depthTest = true) = 0;

	// Accessors
	inline static API GetAPI() { return s_API; };
	static RendererCapabilities &GetCapabilities() {
		static RendererCapabilities capabilities;
		return capabilities;
	}

	// Mutators
	virtual void SetClearColor(const glm::vec4 &color) = 0;
	virtual void SetLineThickness(float value = 1.0f) = 0;
	virtual void SetViewport(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) = 0;
	virtual void SetPolygonMode(PolygonMode mode = PolygonMode::Solid) = 0;

private:
    // Properties
    static API s_API;
};

}
