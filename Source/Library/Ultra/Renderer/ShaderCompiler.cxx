module Ultra.Renderer.ShaderCompiler;

import <shaderc/shaderc.hpp>;

import Ultra.Logger;

namespace Ultra::ShaderCompiler {

shaderc_shader_kind GetNativeShaderKind(ShaderType type) {
    switch (type) {
        case ShaderType::Vertex: { return shaderc_vertex_shader; }
        case ShaderType::Fragment: { return shaderc_fragment_shader; }
        case ShaderType::Compute: { return shaderc_compute_shader; }
        case ShaderType::TessControl: { return shaderc_tess_control_shader; }
        case ShaderType::TessEvaluation: { return shaderc_tess_evaluation_shader; }
        //case ShaderType::SpirvAssembly: { return shaderc_spirv_assembly; }
        default: { return shaderc_glsl_infer_from_source; }
    }
}

vector<uint32_t> Compile(const string &name, ShaderType type, const string &source, bool optimize) {
    shaderc::CompileOptions options;
    options.AddMacroDefinition("ENGINE", "Ultra");
    if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);

    shaderc::Compiler compiler;
    auto result = compiler.CompileGlslToSpv(source, GetNativeShaderKind(type), name.c_str(), options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        LogError("{}", result.GetErrorMessage());
        return {};
    }
    return { result.cbegin(), result.cend() };

}

string CompileToAssembly(const string &name, ShaderType type, const string &source, bool optimize) {
    shaderc::CompileOptions options;
    options.AddMacroDefinition("ENGINE", "Ultra");
    if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);

    shaderc::Compiler compiler;
    auto result = compiler.CompileGlslToSpvAssembly(source, GetNativeShaderKind(type), name.c_str(), options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        LogError("{}", result.GetErrorMessage());
        return {};
    }
    return { result.cbegin(), result.cend() };
}

string PreprocessShader(const string &name, ShaderType type, const string &source) {
    shaderc::CompileOptions options;
    options.AddMacroDefinition("ENGINE", "Ultra");

    shaderc::Compiler compiler;
    auto result = compiler.PreprocessGlsl(source, GetNativeShaderKind(type), name.c_str(), options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        LogError("{}", result.GetErrorMessage());
        return "";
    }

    return { result.cbegin(), result.cend() };
}

string Decompile([[maybe_unused]] const string &source) {
    //// Output
    //switch (output.format) {
    //    case ShaderFormat::GLSL: {
    //        spirv_cross::CompilerGLSL glsl(spirvSource);
    //        spirv_cross::CompilerGLSL::Options scoptions;
    //        scoptions.version = output.glslVersion;
    //        scoptions.es = output.es;

    //        glsl.set_common_options(scoptions);
    //        return { glsl.compile(), spirvSource };
    //    }

    //    case ShaderFormat::HLSL: {
    //        spirv_cross::CompilerHLSL hlsl(spirvSource);
    //        spirv_cross::CompilerHLSL::Options hlslOptions;
    //        hlslOptions.shader_model = 500;

    //        hlsl.set_hlsl_options(hlslOptions);
    //        return { hlsl.compile(), spirvSource };
    //    }

    //    case ShaderFormat::SPIRV: {
    //        std::stringstream result;
    //        for (uint32_t byte : spirvSource) {
    //            result << (char)byte;
    //        }
    //        return { result.str(), spirvSource };
    //    }

    //    default: {
    //        return { "", spirvSource };
    //    }
    //}
    return {};
}

}
