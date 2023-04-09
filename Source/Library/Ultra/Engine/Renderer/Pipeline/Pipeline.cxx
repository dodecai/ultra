module Ultra.Engine.Pipeline;

//import Ultra.Platform.Engine.GLPipeline;
//import Ultra.Platform.Engine.VKPipeline;

namespace Ultra {

Reference<Pipeline> Pipeline::Create(const PipelineProperties &properties) {
    switch (Context::API) {
        case GraphicsAPI::Null:		{ return nullptr; }
        //case GraphicsAPI::OpenGL:	{ return CreateReference<GLPipeline>(properties); }
        //case GraphicsAPI::Vulkan:	{ return CreateReference<VKPipeline>(properties); }
        default:                    { break; }
    }
    LogFatal("[Engine::Renderer::Pipeline] ", "The current graphics API doesn't support Pipelines!");
    return nullptr;
}

}
