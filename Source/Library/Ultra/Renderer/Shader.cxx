module Ultra.Renderer.Shader;

import Ultra.System.FileSystem;

import Ultra.Graphics.Context;
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
            LogFatal("RenderAPI not supported!");
            return nullptr;
        #endif
        }
    }
}


void Shader::Preprocess(string &source, const string &directory) {
    std::regex includeRegex(R""(#include\s+[<"]([^">]+)[">])"");
    std::smatch match;

    // Process includes
    while (std::regex_search(source, match, includeRegex)) {
        auto includeDirective = match[0].str();
        auto includeFile = match[1].str();
        string includeFilePath;

        // Check if include file is a default shader
        if (includeDirective.find('<') != std::string::npos) {
            includeFilePath = "Assets/Shaders/Defaults/" + includeFile;
        } else {
            includeFilePath = directory + "/" + includeFile;
        }

        if (!Directory::ValidatePath(includeFilePath)) {
            LogError("Include file not found: {}", includeFilePath);
            return;
        }

        auto includeSource = File::LoadAsString(includeFilePath);
        Preprocess(includeSource, File::GetPath(includeFilePath));  // Recursive include processing

        // Remove non-ascii characters
        includeSource.erase(std::remove_if(includeSource.begin(), includeSource.end(), [](char c) { return static_cast<unsigned char>(c) >= 128; }), includeSource.end());
        
        // Replace include directive with include source
        source.replace(match.position(), includeDirective.length(), includeSource);
    }
}

Shader::ShaderList Shader::Convert(string &source) {
    // Check if source is file path and load the code
    auto directory = File::GetPath(source);
    if (Directory::ValidatePath(source)) {
        mShaderName = File::GetName(source);
        source = File::LoadAsString(source);
    }

    // Process includes
    Preprocess(source, directory);

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
        string code(&*codeStart, codeLength);

        // Determine shader type and prepend appropriate define directive
        auto shaderType = (ShaderType)ShaderTypeFromString(string(type));
        string defineDirective;
        switch (shaderType) {
            case ShaderType::Compute:       { defineDirective = "#define COMPUTE_SHADER\n"; break; }
            case ShaderType::Fragment:      { defineDirective = "#define PIXEL_SHADER\n"; break; }
            case ShaderType::Geometry:      { defineDirective = "#define GEOMETRY_SHADER\n"; break; }
            case ShaderType::TessControl:   { defineDirective = "#define TESSELLATION_CONTROL_SHADER\n"; break; }
            case ShaderType::TessEvaluation:{ defineDirective = "#define TESSELLATION_EVALUATION_SHADER\n"; break; }
            case ShaderType::Vertex:        { defineDirective = "#define VERTEX_SHADER\n"; break; }
            default: {
                LogWarning("The shader type couldn't be detected or wasn't specified!");
                break;
            }
        }

        // Find the position of the #version directive
        auto versionPos = code.find("#version");
        if (versionPos != string::npos) {
            // Find the end of the #version directive line
            auto versionEnd = code.find('\n', versionPos);
            if (versionEnd != string::npos) {
                // Insert the define directive after the #version directive
                code.insert(versionEnd + 1, defineDirective);
            }
        } else {
            // If there's no #version directive, prepend the define directive to the code
            code = defineDirective + code;
        }

        // Store the shader code in the map
        shaders[ShaderTypeFromString(string(type))] = code;
    }

    // ... otherwise, return the source as it is, but raise a warning, if the type wasn't specified
    if (shaders.empty()) {
        if (mType == ShaderType::Linked) {
            LogWarning("The shader type couldn't be detected and wasn't specified!");
        }
        shaders[(size_t)(mType)] = source;
    }
    return shaders;
}

}
