module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module Ultra.Engine.Mesh;

import Ultra.Core;
import Ultra.Logger;
export import Ultra.Engine.Renderer.CommandBuffer;
export import Ultra.Engine.Renderer.PipelineState;
export import Ultra.Engine.Renderer.Texture;

export namespace Ultra {

constexpr int MaxBoneInfluences = 4;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent {};
    glm::vec3 Bitangent {};

    //int BoneIDs[MaxBoneInfluences] = { 0, 0, 0, 0 };
    //float Weights[MaxBoneInfluences] = { 0.0f, 0.0f, 0.0f, 0.0f };
};

struct TextureData {
    uint32_t ID;
    string Path;
    string Type;
};

using Vertices = vector<Vertex>;
using Indices = vector<uint32_t>;
using Textures = vector<Reference<Texture>>;
using TextureInfo = vector<TextureData>;

class Mesh {
public:
    Mesh(Vertices vertices, Indices indices, Textures textures, TextureInfo info):
        mVertices(vertices),
        mIndices(indices),
        mTextures(textures),
        mTextureData(info) {
        SetupMesh();
    }
    ~Mesh() = default;

    void Draw(CommandBuffer *commandBuffer);
    uint32_t GetIndices() const { return mIndices.size(); }

private:
    void SetupMesh() {
        PipelineProperties properties;
        properties.DepthTest = true;
        properties.Wireframe = false;
        properties.Layout = {
            { ShaderDataType::Float3, "aPosition"  },
            { ShaderDataType::Float3, "aNormal"    },
            { ShaderDataType::Float2, "aTexCoords" },
            { ShaderDataType::Float3, "aTangent"   },
            { ShaderDataType::Float3, "aBitangent" },
            //{ ShaderDataType::Int4,   "aBoneIDs"   },
            //{ ShaderDataType::Float4, "aWeights"   },
        };
        mPipeline = PipelineState::Create(properties);

        mVertexBuffer = Buffer::Create(BufferType::Vertex, mVertices.data(), mVertices.size() * sizeof(Vertex));
        mIndexBuffer = Buffer::Create(BufferType::Index, mIndices.data(), mIndices.size() * sizeof(uint32_t));
    }

private:
    Reference<PipelineState> mPipeline;
    Vertices mVertices;
    Indices mIndices;
    Textures mTextures;

    TextureInfo mTextureData;

    Reference<Buffer> mVertexBuffer;
    Reference<Buffer> mIndexBuffer;
};

}

module: private;

namespace Ultra {

void Mesh::Draw(CommandBuffer *commandBuffer) {
    mPipeline->Bind();
    mVertexBuffer->Bind();
    mIndexBuffer->Bind();

    uint32_t diffuseCount = 1;
    uint32_t specularCount = 1;
    uint32_t normalCount = 1;
    uint32_t heightCount = 1;
    for (size_t i = 0; i < mTextures.size(); i++) {
        mTextures[i]->Bind(i);

        //if (mTextureData[i].Type == "texture_diffuse") {
        //    //mPipeline->SetUniform("uMaterial.Diffuse[" + std::to_string(diffuseCount++) + "]", i);
        //} else if (mTextureData[i].Type == "texture_specular") {
        //    //mPipeline->SetUniform("uMaterial.Specular[" + std::to_string(specularCount++) + "]", i);
        //} else if (mTextureData[i].Type == "texture_normal") {
        //    //mPipeline->SetUniform("uMaterial.Normal[" + std::to_string(normalCount++) + "]", i);
        //} else if (mTextureData[i].Type == "texture_height") {
        //    //mPipeline->SetUniform("uMaterial.Height[" + std::to_string(heightCount++) + "]", i);
        //}

        //if (mTextures[i]->GetType() == TextureType::Diffuse) {
        //    mPipeline.SetUniform("uMaterial.Diffuse[" + std::to_string(diffuseCount++) + "]", i);
        //} else if (mTextures[i]->GetType() == TextureType::Specular) {
        //    mPipeline.SetUniform("uMaterial.Specular[" + std::to_string(specularCount++) + "]", i);
        //} else if (mTextures[i]->GetType() == TextureType::Normal) {
        //    mPipeline.SetUniform("uMaterial.Normal[" + std::to_string(normalCount++) + "]", i);
        //} else if (mTextures[i]->GetType() == TextureType::Height) {
        //    mPipeline.SetUniform("uMaterial.Height[" + std::to_string(heightCount++) + "]", i);
        //}
    }

    commandBuffer->DrawIndexed(mIndices.size(), PrimitiveType::Triangle, true);
}
}
