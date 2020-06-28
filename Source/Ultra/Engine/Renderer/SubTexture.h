#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Omnia/Core.h>

#include "Texture.h"

namespace Ultra {

using namespace Omnia;

class SubTexture2D {
    Reference<Texture2D> mTexture;
    glm::vec2 TextureCoordinates[4];

public:
    SubTexture2D(const Reference<Texture2D> &texture, const glm::vec2 &min, const glm::vec2 &max);
    static Reference<SubTexture2D> CreateFromCoordinates(const Reference<Texture2D> &texture, const glm::vec2 &coordinates, const glm::vec2 &cellSize, const glm::vec2 &spriteSize = { 1, 1 });

    const Reference<Texture2D> GetTexture() const { return mTexture; }
    const glm::vec2 *GetTextCoordinates() const { return TextureCoordinates;  }
};

}
