#pragma once

#include <glm/glm.hpp>

#include <Omnia/Utility/Timer.h>

#include "Ultra.pch"
#include "Ultra/Core.h"

#include "Ultra/Renderer/Camera.h"

namespace Ultra {

using namespace Omnia;


struct ParticleProperties {
    glm::vec2 Position = {};
    glm::vec2 Velocity = {};
    glm::vec2 VelocityVariation = {};
    glm::vec4 ColorBegin = {};
    glm::vec4 ColorEnd = {};

    float SizeBegin = 0.0f;
    float SizeEnd = 0.0f;
    float SizeVariation = 0.0f;
    float LifeTime = 1.0f;
};

class ParticleSystem {
    struct Particle {
        bool Active = false;

        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 ColorBegin;
        glm::vec4 ColorEnd;
        float Rotation = 0.0f;
        float SizeBegin;
        float SizeEnd;

        float LifeTime = 1.0f;
        float LifeRemaining = 0.0f;
    };

    vector<Particle> ParticlePool;
    uint32_t ParticlePoolIndex = 0;

public:
    ParticleSystem(uint32_t maxParticles = 1000);

    void Update(Timestamp deltaTime);
    void Render(PerspectiveCamera &camera);

    void Emit(const ParticleProperties &properties);
};

}
