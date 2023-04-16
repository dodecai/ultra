export module Ultra.Renderer.Shader;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.RendererData;

export namespace Ultra {

///
/// @brief Shader Types
/// @{
enum class ShaderType {
    Linked,         // Combination of vertex and fragment shaders
    Compute,        // Shader that performs general-purpose calculations on the GPU
    Fragment,       // Shader that processes pixel or fragment data
    Geometry,       // Shader that processes primitive geometry
    TessControl,    // Shader that controls the tessellation of geometric primitives
    TessEvaluation, // Shader that evaluates the tessellation of geometric primitives
    Vertex,         // Shader that processes vertices and vertex data
};
/// @}


size_t ShaderTypeFromString(const string &type) {
    if (type == "vertex")   return (size_t)ShaderType::Vertex;
    if (type == "fragment") return (size_t)ShaderType::Fragment;

    // Unknown Shader
    return 1000;
}

/// 
/// @brief Agnostic Shader
///
/// @example: Separate
/// auto vertexShader = Shader::Create("source code", ShaderType::Vertex);
/// auto fragmentShader = Shader::Create("source code", ShaderType::Fragment);
/// 
/// @example: Linked
/// auto linkedShaders = Shader::Create("example.glsl");
/// 
class Shader {
protected:
    using ShaderList = unordered_map<size_t, string>;

    Shader(const string &source, const string &entryPoint, const ShaderType type): mSource(source), mEntryPoint(entryPoint), mType(type) {}

public:
    Shader() = default;
    virtual ~Shader() = default;

    static Scope<Shader> Create(const string &source, const ShaderType type = ShaderType::Linked);
    static Scope<Shader> Create(const string &source, const string &entryPoint, const ShaderType type = ShaderType::Linked);

    virtual void Compile(ShaderList shaders) = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

protected:
    ShaderList SplitSources(string &source);

    RendererID mShaderID;
    string mShaderName;
    string mEntryPoint;
    string mSource;
    ShaderType mType;

    ShaderList mShaders;
};

}
