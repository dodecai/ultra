module Ultra.Engine.Renderer.Shader;

import Ultra.System.FileSystem;

import Ultra.GFX.Context;
import Ultra.Platform.Renderer.DXShader;
import Ultra.Platform.Renderer.GLShader;
import Ultra.Platform.Renderer.VKShader;

namespace Ultra {

Scope<Shader> Shader::Create(const string &source, ShaderType type) {
    return Shader::Create(source, "main", type);
}

Scope<Shader> Shader::Create(const string &source, const string &entryPoint, const ShaderType type) {
    switch (Context::API) {
        case GraphicsAPI::DirectX:  { return CreateScope<DXShader>(source, entryPoint, type); }
        case GraphicsAPI::OpenGL:   { return CreateScope<GLShader>(source, entryPoint, type); }
        case GraphicsAPI::Vulkan:   { return CreateScope<VKShader>(source, entryPoint, type); }

        default: {
        #if APP_MODE_DEBUG
            throw std::runtime_error("Renderer::Shader: RenderAPI not supported!");
        #else
            LogFatal("Renderer::Shader: RenderAPI not supported!");
        #endif
            return nullptr;
        }
    }
}


Shader::ShaderList Shader::Convert(string &source) {
    // Check if source is file path and load the code
    if (CheckValidPath(source)) {
        mShaderName = GetFileName(source);
        source = ReadFile(source);
    }

    // Detect newline format (Linux, Windows, Mac)
    auto eolConversion = '\n';
    auto eolSize = 1u;
    auto cr = source.find_first_of('\r');
    auto lf = source.find_first_of('\n');
    if (cr != string::npos) {
        eolConversion = '\r';
        if (lf != string::npos) eolSize = 2u;
    }

    // Detect type and split source ...
    ShaderList shaders;
    string_view elements = source;
    string_view token = "#type ";
    for (auto element : std::views::split(elements, token)) {
        // Skip the first element and empty elements (since they don't contain a "#type" definition)
        if (element.begin() == elements.begin() || element.empty()) continue;

        // Extract the shader type
        auto typeEnd = std::ranges::find(element, eolConversion);
        auto typeLength = std::ranges::distance(element.begin(), typeEnd);
        string_view type(&*element.begin(), typeLength);

        // Extract the shader code
        auto codeStart = typeEnd + eolSize;
        auto codeLength = std::ranges::distance(codeStart, element.end());
        string_view code(&*codeStart, codeLength);

        // Store the shader code in the map
        shaders[ShaderTypeFromString(string(type))] = string(code);
    }

    // ... otherwise, return the source as it is, but raise a warning, if the type wasn't specified
    if (shaders.empty()) {
        if (mType == ShaderType::Linked) {
            LogWarning("Engine::Shader: The shader type couldn't be detected and wasn't specified!");
        }
        shaders[(size_t)(mType)] = source;
    }
    return shaders;
}

}
