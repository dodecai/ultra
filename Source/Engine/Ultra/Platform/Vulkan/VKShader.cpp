#include "VKShader.h"

namespace Ultra {
VKShader::VKShader(const string &source) {
}
VKShader::VKShader(const string &vertexSource, const string &fragmentSource) {
}
VKShader::~VKShader() {
}
void VKShader::Bind() const {
}
void VKShader::Unbind() const {
}
const string VKShader::GetName() const {
    return string();
}
void VKShader::SetInt(const string &name, int value) {
}
void VKShader::SetIntArray(const string &name, int *values, size_t count) {
}
void VKShader::SetFloat(const string &name, const float value) {
}
void VKShader::SetFloat3(const string &name, const glm::vec3 &values) {
}
void VKShader::SetFloat4(const string &name, const glm::vec4 &values) {
}
void VKShader::SetMat4(const string &name, const glm::mat4 &values) {
}
void VKShader::Compile(unordered_map<unsigned int, string> sources) {
}
unordered_map<unsigned int, string> VKShader::Prepare(string &source) {
    return unordered_map<unsigned int, string>();
}
int32_t VKShader::GetUniformLocation(const string &name) const {
    return int32_t();
}
void VKShader::UploadaUniformInt(const string &name, int values) const {
}
void VKShader::UploadUniformIntArray(const string &name, int *values, size_t count) const {
}
void VKShader::UploadaUniformFloat(const string &name, const float value) const {
}
void VKShader::UploadaUniformFloat2(const string &name, const glm::vec2 &values) const {
}
void VKShader::UploadaUniformFloat3(const string &name, const glm::vec3 &values) const {
}
void VKShader::UploadaUniformFloat4(const string &name, const glm::vec4 &values) const {
}
void VKShader::UploadaUniformMat3(const string &name, const glm::mat3 &matrix) const {
}
void VKShader::UploadaUniformMat4(const string &name, const glm::mat4 &matrix) const {
}
}
