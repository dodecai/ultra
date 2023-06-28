export module Ultra.Engine.Utilty.ShaderCompiler;

import Ultra.Core;
import <exception>;
import <iterator>;
import <stdexcept>;

#include <exception>
#include <stdexcept>
#include <iterator>

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

string Compile(const string &source, const InputOptions &ioptions, const OutputOptions &ooptions);

string Decompile(const string &source);

}

}
