module;

#include <assimp/DefaultLogger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

///
/// @brief Hack: This is an nasty fix for Microsoft's STL implementation
/// The logger fails in multiple modules due to chrono template resolutions
///
/// @note
/// type_traits(1344,53): error C2794: 'type': is not a member of any direct or indirect base class of 'std::common_type<_Rep1,_Rep2>'
///     with
///     [
///         _Rep1 = __int64,
///         _Rep2 = __int64
///     ]
/// __msvc_chrono.hpp(268,35): error C2938: 'std::common_type_t' : Failed to specialize alias template
/// __msvc_chrono.hpp(268,56): error C2752: 'std::common_type<_Rep1,_Rep2>': more than one partial specialization matches the template argument list
///     with
///     [
///         _Rep1 = __int64,
///         _Rep2 = __int64
///     ]
/// __msvc_chrono.hpp(113,54): error C2955: 'std::chrono::duration': use of class template requires template argument list
/// __msvc_chrono.hpp(118,54): error C2955: 'std::chrono::duration': use of class template requires template argument list
///
#include <chrono>

export module Ultra.Engine.Model;

import Ultra.Core;
import Ultra.Logger;
import Ultra.Engine.Mesh;
import Ultra.Engine.Renderer.Texture;
import Ultra.System.FileSystem;

export namespace Ultra {

using Meshes = vector<Mesh>;

class Model {
public:
    Model(const string &path, bool gamma = false): mGammaCorrection(gamma) {
        Load(path);
    }
    ~Model() = default;

    void Draw(CommandBuffer *commandBuffer) {
        for (auto &mesh: mMeshes) {
            mesh.Draw(commandBuffer);
        }
    }

private:
    void Load(const string &path) {
        Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
        Assimp::LogStream *stderrStream = Assimp::LogStream::createDefaultStream(aiDefaultLogStream_STDERR);
        Assimp::DefaultLogger::get()->attachStream(stderrStream, Assimp::Logger::NORMAL | Assimp::Logger::DEBUGGING | Assimp::Logger::VERBOSE);

        Assimp::Importer importer;
        const auto *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LogError("Assimp::Importer {}", importer.GetErrorString());
            return;
        }
        mDirectory = GetFilePath(path);
        ProcessNode(scene->mRootNode, scene);


        Assimp::DefaultLogger::kill();
    }

    void ProcessNode(aiNode *node, const aiScene *scene) {
        for (size_t i = 0; i < node->mNumMeshes; i++) {
            auto *mesh = scene->mMeshes[node->mMeshes[i]];
            mMeshes.push_back(ProcessMesh(mesh, scene));
        }
        for (size_t i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene) {
        Vertices vertices {};
        Indices indices {};
        Textures textures {};
        vector<TextureData> info {};
        MaterialData material {};

        // Vertices
        for (size_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            if (mesh->mNormals) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;

                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;

                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            } else {
                vertex.TexCoords = { 0.0f, 0.0f };
            }

            vertices.push_back(vertex);
        }

        // Indices
        for (size_t i = 0; i < mesh->mNumFaces; i++) {
            auto face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        // Materials
        if (mesh->mMaterialIndex >= 0) {
            auto *material = scene->mMaterials[mesh->mMaterialIndex];

            auto [diffuseMaps, diffuseInfo] = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "Diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            info.insert(info.end(), diffuseInfo.begin(), diffuseInfo.end());

            auto [normalMaps, normalInfo] = LoadMaterialTextures(material, aiTextureType_HEIGHT, "Normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            info.insert(info.end(), normalInfo.begin(), normalInfo.end());

            auto [specularMaps, specularInfo] = LoadMaterialTextures(material, aiTextureType_SPECULAR, "Specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            info.insert(info.end(), specularInfo.begin(), specularInfo.end());

            auto [heightMaps, heightInfo] = LoadMaterialTextures(material, aiTextureType_HEIGHT, "Height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
            info.insert(info.end(), heightInfo.begin(), heightInfo.end());

            auto [ambientMaps, ambientInfo] = LoadMaterialTextures(material, aiTextureType_AMBIENT, "Ambient");
            textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
            info.insert(info.end(), ambientInfo.begin(), ambientInfo.end());
        }
        if (textures.empty() && mesh->mMaterialIndex >= 0) {
            material = LoadMaterial(scene->mMaterials[mesh->mMaterialIndex]);
        }

        return Mesh(vertices, indices, textures, info, material);
    }

    MaterialData LoadMaterial(aiMaterial *material) {
        MaterialData result;
        aiColor3D color(0.0f, 0.0f, 0.0f);
        float shininess;

        material->Get(AI_MATKEY_COLOR_AMBIENT, color);
        result.Ambient = glm::vec3(color.r, color.g, color.b);

        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        result.Diffuse = glm::vec3(color.r, color.g, color.b);

        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        result.Specular = glm::vec3(color.r, color.g, color.b);

        material->Get(AI_MATKEY_SHININESS, shininess);
        if (shininess < 0.0f) {
            shininess = 32.0f;
        }
        result.Shininess = shininess;

        return result;
    }

    std::tuple<Textures, TextureInfo> LoadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName) {
        vector<Reference<Texture>> textures {};
        TextureInfo info {};

        for (size_t i = 0; i < material->GetTextureCount(type); i++) {
            aiString str;
            material->GetTexture(type, i, &str);
            bool skip = false;
            for (size_t j = 0; j < mTexturesLoaded.size(); j++) {
                if (strcmp(mTexturesLoaded[j].Path.data(), str.C_Str()) == 0) {
                    info.push_back(mTexturesLoaded[j]);
                    textures.push_back(mTextures[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                string path = mDirectory + '/' + str.C_Str();
                TextureData data;
                auto texture = Texture::Create({}, path);
                data.ID = texture->GetRendererID();
                data.Type = typeName;
                data.Path = str.C_Str();

                mTextures.push_back(texture);
                info.push_back(data);
                mTexturesLoaded.push_back(data);
            }
        }
        return { textures, info };
    }

private:
    string mDirectory;
    Meshes mMeshes;
    bool mGammaCorrection;

    TextureInfo mTexturesLoaded {};
    Textures mTextures {};
};

}
