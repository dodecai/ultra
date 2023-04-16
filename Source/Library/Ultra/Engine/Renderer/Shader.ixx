export module Ultra.Renderer.Shader;

export import Ultra.Core;
export import Ultra.Logger;
export import Ultra.Engine.RendererData;

export namespace Ultra {

///
/// @brief Shader Types
/// @{
enum class ShaderType {
    Vertex,         // Shader that processes vertices and vertex data
    Fragment,       // Shader that processes pixel or fragment data
    Linked,         // Combination of vertex and fragment shaders
    Compute,        // Shader that performs general-purpose calculations on the GPU
    Geometry,       // Shader that processes primitive geometry
    TessControl,    // Shader that controls the tessellation of geometric primitives
    TessEvaluation, // Shader that evaluates the tessellation of geometric primitives
};
/// @}

/// 
/// @brief Agnostic Shader
///
/// @example: How-To
/// auto vertexShader = Shader::Create({ ShaderType::Vertex, "source code", "main" });
/// auto fragmentShader = Shader::Create({ ShaderType::Fragment, "source code", "main" });
/// 
class Shader {
protected:
    Shader(ShaderType type, const std::string &source, const std::string &entryPoint): mType(type), mSource(source), mEntryPoint(entryPoint) {}

public:
    Shader() = default;
    virtual ~Shader() = default;

    static Scope<Shader> Create(ShaderType type, const std::string &source, const std::string &entryPoint);

    virtual void Compile() = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

protected:
    RendererID mShaderID;
    string mEntryPoint;
    string mSource;
    ShaderType mType;
};

}
