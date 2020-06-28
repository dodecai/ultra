#pragma once

#include <glm/glm.hpp>

#include <Omnia/Core.h>
#include <Omnia/Utility/Timer.h>

#include "Engine/Renderer/Camera.h"

namespace Ultra {

using namespace Omnia;


struct ParticleProperties {
    glm::vec2 Position;
    glm::vec2 Velocity;
    glm::vec2 VelocityVariation;
    glm::vec4 ColorBegin;
    glm::vec4 ColorEnd;

    float SizeBegin;
    float SizeEnd;
    float SizeVariation;
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
    void Render(OrthographicCamera &camera);

    void Emit(const ParticleProperties &properties);
};

}
