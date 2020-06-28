#include "SubTexture.h"

namespace Ultra {

SubTexture2D::SubTexture2D(const Reference<Texture2D> &texture, const glm::vec2 &min, const glm::vec2 &max):
    mTexture{ texture } {
    TextureCoordinates[0] = { min.x, min.y };
    TextureCoordinates[1] = { max.x, min.y };
    TextureCoordinates[2] = { max.x, max.y };
    TextureCoordinates[3] = { min.x, max.y };
}

Reference<SubTexture2D> SubTexture2D::CreateFromCoordinates(const Reference<Texture2D> &texture, const glm::vec2 &coordinates, const glm::vec2 &cellSize, const glm::vec2 &spriteSize) {
    glm::vec2 min = { (coordinates.x * cellSize.x) / texture->GetWidth(), (coordinates.y * cellSize.y) / texture->GetHeight() };
    glm::vec2 max = { ((coordinates.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coordinates.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
    Reference<SubTexture2D> result = CreateReference<SubTexture2D>(texture, min, max);
    
    return result;
}

}
