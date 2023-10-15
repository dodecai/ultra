export module Ultra.Renderer.ShaderCompiler;

import Ultra.Core;
import Ultra.Renderer.Shader;

export namespace Ultra {

namespace ShaderCompiler {

enum ShaderFormat {
    GLSL,
    HLSL,
    SPIRV,
    Undefined,
};

enum ShaderLanguage {
    Vertex,
    TessControl,
    TessEvaluation,
    Geometry,
    Fragment,
    Compute,
    Undefined2,
};

struct InputOptions {
    // All
    ShaderFormat format;
    ShaderLanguage stage;
    bool ignoreErrors;

    // GLSL
    unsigned glslVersion;
    bool es;

    // HLSL
    //unsigned hlhlShaderModel;

    // MSL
    //unsigned metalPlatform;
};

struct OutputOptions {
    // All
    ShaderFormat format;

    // GLSL
    unsigned glslVersion;
    bool es;

    // HLSL
    //unsigned hlhlShaderModel;

    // MSL
    //unsigned metalPlatform;
};

vector<uint32_t> Compile(const string &name, ShaderType type, const string &source, bool optimize = false);

string CompileToAssembly(const string &name, ShaderType type, const string &source, bool optimize = false);

string PreprocessShader(const string &name, ShaderType type, const string &source);

string Decompile(const string &source);

}

}
