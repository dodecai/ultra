#pragma once

#include "Ultra.pch"

#include <glm/glm.hpp>

#include "Buffer.h"
#include "Material.h"
#include "Pipeline.h"
#include "Texture.h"

struct aiScene;
struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
namespace Assimp {
    class Importer;
}

namespace Ultra {

struct Vertex {
    static const int NumAttributes = 5;
    glm::vec3 Position  = {};
    glm::vec3 Normal    = {};
    glm::vec3 Tangent   = {};
    glm::vec3 Bitangent = {};
    glm::vec2 TexCoords = {};
};

struct AnimatedVertex: public Vertex {
    array<uint32_t, 4> IDs  = { 0 };
    array<float, 4> Weights = { 0.0f };

    void AddBoneData(uint32_t id, float weight) {
        for (size_t i = 0; i < 4; i++) {
            if (Weights[i] == 0.0f) {
                IDs[i] = id;
                Weights[0] = weight;
                return;
            }
        }

        AppLogWarning("[Ultra::Renderer::Mesh]: This object has more than four bones/weights!");
    }
};

struct BoneData {
    glm::mat4 Offset;
    glm::mat4 FinalTransformation;
};

struct VertexBoneData {
    array<uint32_t, 4> IDs  = { 0 };
    array<float, 4> Weights = { 0.0f };

    void AddBoneData(uint32_t id, float weight) {
        for (size_t i = 0; i < 4; i++) {
            if (Weights[i] == 0.0f) {
                IDs[i] = id;
                Weights[0] = weight;
                return;
            }
        }

        AppLogWarning("[Ultra::Renderer::Mesh]: This object has more than four bones/weights!");
    }
};

struct Index {
    uint32_t Vertex1 = 0;
    uint32_t Vertex2 = 0;
    uint32_t Vertex3 = 0;

    Index() {
        //static_assert(sizeof(Index) == 3 / sizeof(uint32_t));
    }
};

struct Triangle {
    Vertex Vertex0 = {};
    Vertex Vertex1 = {};
    Vertex Vertex2 = {};

    Triangle(const Vertex &vertex0, const Vertex &vertex1, const Vertex &vertex2):
        Vertex0(vertex0),
        Vertex1(vertex1),
        Vertex2(vertex2) {
    }
};

struct Submesh {
    uint32_t BaseIndex  = 0;
    uint32_t BaseVertex = 0;
    uint32_t IndexCount = 0;

    glm::mat4 Transform = {};
    string MeshName     = "";
    string NodeName     = "";
};

class Mesh {
    //friend Renderer;
    //friend Browser;

public:
    Mesh(const string &object);
    ~Mesh();

    void Update(Timestamp delta);

    // Accessors
    const string &GetLocation();
    vector<Submesh> &GetSubmeshes();
    Reference<Shader> GetShader();
    vector<Reference<Texture2D>> & GetTextures();
    Reference<IndexBuffer> GetIndexBuffer();
    Reference<VertexBuffer> GetVertexBuffer();
private:
    void BoneTransform(float time);
    void ReadNodeHierarchy(float animationTime, const aiNode *pNode, const glm::mat4 &ParentTransform);
    void TraverseNodes(aiNode *node, const glm::mat4 &parentTransform = glm::mat4(1.0f), uint32_t level = 0);

    const aiNodeAnim *FindNodeAnim(const aiAnimation *animation, const string nodeName);
    uint32_t FindPosition(float animationTime, const aiNodeAnim *pNodeAnim);
    uint32_t FindRotation(float animationTime, const aiNodeAnim *pNodeAnim);
    uint32_t FindScaling(float animationTime, const aiNodeAnim *pNodeAnim);
    glm::vec3 InterpolatePosition(float animationTime, const aiNodeAnim *pNodeAnim);
    glm::vec3 InterpolateRotation(float animationTime, const aiNodeAnim *pNodeAnim);
    glm::vec3 InterpolateScaling(float animationTime, const aiNodeAnim *pNodeAnim);

private:
    Scope<Assimp::Importer> mImporter;
    string mLocation;

    glm::mat4 mInverseTransform;
    std::vector<Submesh> mSubmeshes;

    Reference<Pipeline> mPipeline;
    Reference<VertexBuffer> mVertexBuffer;
    Reference<IndexBuffer> mIndexBuffer;
    //VertexBufferLayout mVertexBufferLayout;

    /// Mesh Properties
    const aiScene *mScene;
    vector<Vertex> mVertices;
    vector<AnimatedVertex> mAnimatedVertices;
    vector<Index> mIndices;
    uint32_t mBoneCount = 0;
    vector<BoneData> mBoneData;
    unordered_map<string, uint32_t> mBoneMapping;
    vector<glm::mat4> mBoneTransforms;

    /// Material Properties
    Reference<Material> mBaseMaterial;
    Reference<MaterialInstance> mMaterials;
    Reference<Shader> mShader;
    vector<Reference<Texture2D>> mTextures;
    vector<Reference<Texture2D>> mNormalMaps;
    unordered_map<uint32_t, vector<Triangle>> mTriangleCache;

    /// Animation Properties
    bool mAnimated = false;
    bool mAnimationPlaying = true;
    float mAnimationTime = 0.0f;
    float mTimeMultiplier = 1.0f;
    float mWorldTime = 0.0f;
};
}
