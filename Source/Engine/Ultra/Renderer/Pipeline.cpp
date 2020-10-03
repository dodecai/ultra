#include "Pipeline.h"

#include <Omnia/GFX/Context.h>

#include "Ultra/Platform/OpenGL/GLPipeline.h"
#include "Ultra/Platform/Vulkan/VKPipeline.h"

namespace Ultra {

Reference<Pipeline> Pipeline::Create(const PipelineProperties &properties) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        case GraphicsAPI::OpenGL:	{ return CreateReference<GLPipeline>(properties); }
        case GraphicsAPI::Vulkan:	{ return CreateReference<VKPipeline>(properties); }
        default:                    { break; }
    }
    AppLogCritical("[Engine::Renderer::Pipeline] ", "The current graphics API doesn't support Pipelines!");
    return nullptr;
}

}
