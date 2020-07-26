#include "ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include <Omnia/Utility/Random.h>

#include "Engine/Renderer/Renderer2D.h"

namespace Ultra {

ParticleSystem::ParticleSystem(uint32_t maxParticles): ParticlePoolIndex { maxParticles -1 } {
    ParticlePool.resize(maxParticles);
}

void ParticleSystem::Update(Timestamp deltaTime) {
    for (auto &particle : ParticlePool) {
        if (!particle.Active) continue;

        if (particle.LifeRemaining <= 0.0f) {
            particle.Active = false;
            continue;
        }

        particle.LifeRemaining -= deltaTime;
        particle.Position += particle.Velocity * (float)deltaTime;
        particle.Rotation += 0.01f * deltaTime;
    }
}

void ParticleSystem::Render(Camera &camera) {
    Renderer2D::BeginScene(camera);
    for (auto &particle : ParticlePool) {
        if (!particle.Active) continue;

        float life = particle.LifeRemaining / particle.LifeTime;
        glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
        
        float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
        glm::vec3 position = { particle.Position.x, particle.Position.y, 0.2f };
        Renderer2D::DrawRotatedQuad(position, { size, size }, particle.Rotation, color);
    }
    Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProperties &properties) {
    Particle &particle = ParticlePool[ParticlePoolIndex];
    particle.Active = true;
    particle.Position = properties.Position;
    particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

    particle.Velocity = properties.Velocity;
    particle.Velocity.x += properties.VelocityVariation.x * (Random::Float() - 0.5f);
    particle.Velocity.y += properties.VelocityVariation.y * (Random::Float() - 0.5f);

    particle.ColorBegin = properties.ColorBegin;
    particle.ColorEnd = properties.ColorEnd;

    particle.LifeTime = properties.LifeTime;
    particle.LifeRemaining = properties.LifeTime;
    particle.SizeBegin = properties.SizeBegin + properties.SizeVariation * (Random::Float() -0.5f);
    particle.SizeEnd = properties.SizeEnd;

    ParticlePoolIndex = --ParticlePoolIndex % ParticlePool.size();
}

}
