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
    glm::vec3 Position {};
    glm::vec3 Normal {};
    glm::vec2 TexCoords {};
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
        properties.BlendMode = BlendMode::Alpha;
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

        mVertexBuffer = Buffer::Create(BufferType::Vertex, mVertices.data(), sizeof_vector(mVertices));
        mIndexBuffer = Buffer::Create(BufferType::Index, mIndices.data(), sizeof_vector(mIndices));
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
    mVertexBuffer->Bind();
    mPipeline->Bind();
    mIndexBuffer->Bind();

    for (size_t i = 0; i < mTextures.size(); i++) {
        if (i >= 3) break;
        mTextures[i]->Bind(i);
    }
    commandBuffer->DrawIndexed(mIndices.size(), PrimitiveType::Triangle, true);
    mPipeline->Unbind();
}
}
