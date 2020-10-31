#include "Mesh.h"

#include <glad/gl.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

namespace Ultra {
Mesh::Mesh(const string &object) {
    // constructor
    //Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    //    this->vertices = vertices;
    //    this->indices = indices;
    //    this->textures = textures;
    //
    //    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    //    setupMesh();
    //}
    // initializes all the buffer objects/arrays
    //void setupMesh() {
    //    // create buffers/arrays
    //    glGenVertexArrays(1, &VAO);
    //    glGenBuffers(1, &VBO);
    //    glGenBuffers(1, &EBO);
    //
    //    glBindVertexArray(VAO);
    //    // load data into vertex buffers
    //    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //    // A great thing about structs is that their memory layout is sequential for all its items.
    //    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    //    // again translates to 3/2 floats which translates to a byte array.
    //    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    //
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    //
    //    // set the vertex attribute pointers
    //    // vertex Positions
    //    glEnableVertexAttribArray(0);
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    //    // vertex normals
    //    glEnableVertexAttribArray(1);
    //    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    //    // vertex texture coords
    //    glEnableVertexAttribArray(2);
    //    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    //    // vertex tangent
    //    glEnableVertexAttribArray(3);
    //    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
    //    // vertex bitangent
    //    glEnableVertexAttribArray(4);
    //    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));
    //
    //    glBindVertexArray(0);
    //}
}

Mesh::~Mesh() {
}

void Mesh::Update(Timestamp delta) {

    // // render the mesh
    // void Draw(Shader shader) {
    //     // bind appropriate textures
    //     unsigned int diffuseNr = 1;
    //     unsigned int specularNr = 1;
    //     unsigned int normalNr = 1;
    //     unsigned int heightNr = 1;
    //     for(unsigned int i = 0; i < textures.size(); i++) {
    //         glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
    //         // retrieve texture number (the N in diffuse_textureN)
    //std::string number;
    //std::string name = textures[i].type;
    //         if(name == "texture_diffuse")
    //             number = std::to_string(diffuseNr++);
    //         else if(name == "texture_specular")
    //             number = std::to_string(specularNr++); // transfer unsigned int to stream
    //         else if(name == "texture_normal")
    //             number = std::to_string(normalNr++); // transfer unsigned int to stream
    //         else if(name == "texture_height")
    //             number = std::to_string(heightNr++); // transfer unsigned int to stream

    //         // now set the sampler to the correct texture unit
    //         //glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
    //         // and finally bind the texture
    //         glBindTexture(GL_TEXTURE_2D, textures[i].id);
    //     }

    //     // draw mesh
    //     glBindVertexArray(VAO);
    //     glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //     glBindVertexArray(0);

    //     // always good practice to set everything back to defaults once configured.
    //     glActiveTexture(GL_TEXTURE0);
    // }
}

// Accessors
const string &Mesh::GetLocation() {
    return mLocation;
}

vector<Submesh> &Mesh::GetSubmeshes() {
    return mSubmeshes;
}

Reference<Shader> Mesh::GetShader() {
    return mShader;
}

vector<Reference<Texture2D>> &Mesh::GetTextures() {
    return mTextures;
}

Reference<IndexBuffer> Mesh::GetIndexBuffer() {
    return mIndexBuffer;
}

Reference<VertexBuffer> Mesh::GetVertexBuffer() {
    return mVertexBuffer;
}

void Mesh::BoneTransform(float time) {
}

void Mesh::ReadNodeHierarchy(float animationTime, const aiNode *pNode, const glm::mat4 &ParentTransform) {
}

void Mesh::TraverseNodes(aiNode *node, const glm::mat4 &parentTransform, uint32_t level) {
}

const aiNodeAnim *Mesh::FindNodeAnim(const aiAnimation *animation, const string nodeName) {
    return nullptr;
}

uint32_t Mesh::FindPosition(float animationTime, const aiNodeAnim *pNodeAnim) {
    return uint32_t();
}

uint32_t Mesh::FindRotation(float animationTime, const aiNodeAnim *pNodeAnim) {
    return uint32_t();
}

uint32_t Mesh::FindScaling(float animationTime, const aiNodeAnim *pNodeAnim) {
    return uint32_t();
}

glm::vec3 Mesh::InterpolatePosition(float animationTime, const aiNodeAnim *pNodeAnim) {
    return glm::vec3();
}

glm::vec3 Mesh::InterpolateRotation(float animationTime, const aiNodeAnim *pNodeAnim) {
    return glm::vec3();
}

glm::vec3 Mesh::InterpolateScaling(float animationTime, const aiNodeAnim *pNodeAnim) {
    return glm::vec3();
}


}
