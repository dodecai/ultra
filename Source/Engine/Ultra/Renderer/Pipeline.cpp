#include "Pipeline.h"

#include <Omnia/GFX/Context.h>

#include "Ultra/Platform/OpenGL/GLPipeline.h"

namespace Ultra {

Reference<Pipeline> Pipeline::Create(const PipelineProperties &properties) {
    switch (Context::API) {
        case GraphicsAPI::Null:		    { return nullptr; }
        case GraphicsAPI::OpenGL:		{ return CreateReference<GLPipeline>(properties); }
        //case GraphicsAPI::Vulkan:		{ return CreateReference<VKPipeline>(properties); }
    }
    AppLogCritical("[Ultra::Renderer::Pipeline::Create]: ", "The selected graphics API is currently not supported!");
    return nullptr;
}

}
