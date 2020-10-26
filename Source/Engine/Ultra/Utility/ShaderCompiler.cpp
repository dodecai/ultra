#include "ShaderCompiler.h"

#include <utility>

#include <glslang/Public/ShaderLang.h>
#include <spirv/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>
#include <StandAlone/ResourceLimits.h>

//#define APP_MODE_DEBUG

//#include <spirv/disassemble.h>
//#include <glslang/Include/ResourceLimits.h>
//#include <spirv/GlslangToSpv.h>
//#include <spirv/SPVRemapper.h>
//#include <spirv/disassemble.h>
//#include <spirv/doc.h>
//#include <spirv.hpp>
//#include <spirv_glsl.hpp>
//#include <spirv_hlsl.hpp>
//#include <spirv_msl.hpp>

#include <Omnia/Log.h>

namespace Ultra {

namespace ShaderCompiler {

static TBuiltInResource GetShaderResources() {
    TBuiltInResource resources {
        .maxLights                                  = 32,
        .maxClipPlanes                              = 6,
        .maxTextureUnits                            = 32,
        .maxTextureCoords                           = 32,
        .maxVertexAttribs                           = 64,
        .maxVertexUniformComponents                 = 4096,
        .maxVaryingFloats                           = 64,
        .maxVertexTextureImageUnits                 = 32,
        .maxCombinedTextureImageUnits               = 80,
        .maxTextureImageUnits                       = 32,
        .maxFragmentUniformComponents               = 4096,
        .maxDrawBuffers                             = 32,
        .maxVertexUniformVectors                    = 128,
        .maxVaryingVectors                          = 8,
        .maxFragmentUniformVectors                  = 16,
        .maxVertexOutputVectors                     = 16,
        .maxFragmentInputVectors                    = 15,
        .minProgramTexelOffset                      = -8,
        .maxProgramTexelOffset                      = 7,
        .maxClipDistances                           = 8,
        .maxComputeWorkGroupCountX                  = 65535,
        .maxComputeWorkGroupCountY                  = 65535,
        .maxComputeWorkGroupCountZ                  = 65535,
        .maxComputeWorkGroupSizeX                   = 1024,
        .maxComputeWorkGroupSizeY                   = 1024,
        .maxComputeWorkGroupSizeZ                   = 64,
        .maxComputeUniformComponents                = 1024,
        .maxComputeTextureImageUnits                = 16,
        .maxComputeImageUniforms                    = 8,
        .maxComputeAtomicCounters                   = 8,
        .maxComputeAtomicCounterBuffers             = 1,
        .maxVaryingComponents                       = 60,
        .maxVertexOutputComponents                  = 64,
        .maxGeometryInputComponents                 = 64,
        .maxGeometryOutputComponents                = 128,
        .maxFragmentInputComponents                 = 128,
        .maxImageUnits                              = 8,
        .maxCombinedImageUnitsAndFragmentOutputs    = 8,
        .maxCombinedShaderOutputResources           = 8,
        .maxImageSamples                            = 0,
        .maxVertexImageUniforms                     = 0,
        .maxTessControlImageUniforms                = 0,
        .maxTessEvaluationImageUniforms             = 0,
        .maxGeometryImageUniforms                   = 0,
        .maxFragmentImageUniforms                   = 8,
        .maxCombinedImageUniforms                   = 8,
        .maxGeometryTextureImageUnits               = 16,
        .maxGeometryOutputVertices                  = 256,
        .maxGeometryTotalOutputComponents           = 1024,
        .maxGeometryUniformComponents               = 1024,
        .maxGeometryVaryingComponents               = 64,
        .maxTessControlInputComponents              = 128,
        .maxTessControlOutputComponents             = 128,
        .maxTessControlTextureImageUnits            = 16,
        .maxTessControlUniformComponents            = 1024,
        .maxTessControlTotalOutputComponents        = 4096,
        .maxTessEvaluationInputComponents           = 128,
        .maxTessEvaluationOutputComponents          = 128,
        .maxTessEvaluationTextureImageUnits         = 16,
        .maxTessEvaluationUniformComponents         = 1024,
        .maxTessPatchComponents                     = 120,
        .maxPatchVertices                           = 32,
        .maxTessGenLevel                            = 64,
        .maxViewports                               = 16,
        .maxVertexAtomicCounters                    = 0,
        .maxTessControlAtomicCounters               = 0,
        .maxTessEvaluationAtomicCounters            = 0,
        .maxGeometryAtomicCounters                  = 0,
        .maxFragmentAtomicCounters                  = 8,
        .maxCombinedAtomicCounters                  = 8,
        .maxAtomicCounterBindings                   = 1,
        .maxVertexAtomicCounterBuffers              = 0,
        .maxTessControlAtomicCounterBuffers         = 0,
        .maxTessEvaluationAtomicCounterBuffers      = 0,
        .maxGeometryAtomicCounterBuffers            = 0,
        .maxFragmentAtomicCounterBuffers            = 1,
        .maxCombinedAtomicCounterBuffers            = 1,
        .maxAtomicCounterBufferSize                 = 16384,
        .maxTransformFeedbackBuffers                = 4,
        .maxTransformFeedbackInterleavedComponents  = 64,
        .maxCullDistances                           = 8,
        .maxCombinedClipAndCullDistances            = 8,
        .maxSamples                                 = 4,
        .limits = {
            .nonInductiveForLoops                   = true,
            .whileLoops                             = true,
            .doWhileLoops                           = true,
            .generalUniformIndexing                 = true,
            .generalAttributeMatrixVectorIndexing   = true,
            .generalVaryingIndexing                 = true,
            .generalSamplerIndexing                 = true,
            .generalVariableIndexing                = true,
            .generalConstantMatrixVectorIndexing    = true
        }
    };
    return resources;
}

static EShLanguage GetShaderLanguage(const ShaderLanguage language) {
    switch (language) {
        case ShaderLanguage::Compute:          { return EShLanguage::EShLangCompute; }
        case ShaderLanguage::Vertex:           { return EShLanguage::EShLangVertex; }
        case ShaderLanguage::Fragment:         { return EShLanguage::EShLangFragment; }
        case ShaderLanguage::Geometry:         { return EShLanguage::EShLangGeometry; }
        case ShaderLanguage::TessControl:      { return EShLanguage::EShLangTessControl; }
        case ShaderLanguage::TessEvaluation:   { return EShLanguage::EShLangTessEvaluation; }
        deault:                             { return EShLanguage::EShLangCount; }
    }
}


string Compile(const string &source, const InputOptions &input, const OutputOptions &output) {
    static bool initialized = false;
    if (!initialized) {
        glslang::InitializeProcess();
        initialized = true;
    }

    // Input
    EShLanguage language = GetShaderLanguage(input.stage);
    EShMessages messages = static_cast<EShMessages>(EShMsgDefault | EShMsgSpvRules | EShMsgVulkanRules );
    #ifdef APP_MODE_DEBUG
        messages = static_cast<EShMessages>(messages | EShMsgDebugInfo);
    #endif
    TBuiltInResource resources = GetShaderResources();      // glslang::DefaultTBuiltInResource;
    glslang::TProgram program;
    glslang::TShader shader(language);

    auto shaderSource = source.c_str();
    //setStringsWithLengthsAndNames
    shader.setStrings(&shaderSource, 1);
    shader.setPreamble("");

    //shader.setAutoMapBindings(true);
    //shader.setAutoMapLocations(true);

    auto defaultVulkanVersion = glslang::EShTargetVulkan_1_0;
    int defaultVulkanVersionN = 100;
    
    shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, defaultVulkanVersionN);
    shader.setEnvClient(glslang::EShClientVulkan, defaultVulkanVersion);
    // ToDo:: Determine correct vulkan version ...
    // VK_API_VERSION_1_1 ? glslang::EShTargetSpv_1_3 : glslang::EShTargetSpv_1_0
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

    string preResult;
    DirStackFileIncluder Includer;

    if (!shader.preprocess(&resources, defaultVulkanVersion, ENoProfile, false, false, messages, &preResult, Includer)) {
        AppLogWarning("[Ultra::ShaderCompiler]: Preprocess failed!");
        AppLog(shader.getInfoLog());
        AppLog(shader.getInfoDebugLog());
    }

    auto shaderSourcePre = preResult.c_str();
    //setStringsWithLengthsAndNames
    shader.setStrings(&shaderSourcePre, 1);
    //shader.setPreamble("");

    if (!shader.parse(&resources, defaultVulkanVersion, true, messages)) {
        AppLogWarning("[Ultra::ShaderCompiler]: Parsing Failed!");
        AppLog(shader.getInfoLog());
        AppLog(shader.getInfoDebugLog());
    }

    program.addShader(&shader);
    if (!program.link(messages) || !program.mapIO()) {
        AppLogWarning("[Ultra::ShaderCompiler]: The shader could not be linked!");
        AppLog(shader.getInfoLog());
        AppLog(shader.getInfoDebugLog());
    }
    //program.buildReflection();

    glslang::SpvOptions spvOptions;
    #ifdef APP_MODE_DEBUG
        spvOptions.generateDebugInfo = true;
        spvOptions.disableOptimizer = true;
        spvOptions.optimizeSize = false;
    #else
        //spvOptions.generateDebugInfo = false;
        //spvOptions.disableOptimizer = false;
        //spvOptions.optimizeSize = true;
    #endif



    spv::SpvBuildLogger logger;
    std::vector<uint32_t> spirv;
    glslang::GlslangToSpv(*program.getIntermediate(language), spirv, &logger, &spvOptions);
    if (logger.getAllMessages().length() > 0) {
        AppLog(logger.getAllMessages());
    }
    //glslang::FinalizeProcess();

    std::stringstream result;
    for (auto &byte : spirv) {
        result.write((const char *)&byte, sizeof(byte));
    }
    return result.str();
}

string Decompile(const string &source) {
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

}
