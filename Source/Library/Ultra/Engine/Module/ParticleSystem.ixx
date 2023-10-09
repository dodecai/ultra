module;

#define GLM_ENABLE_EXPERIMENTAL

export module Ultra.Engine.ParticleSystem;

import Ultra.Core;
import Ultra.Engine.Renderer2D;
import Ultra.Engine.Camera;
import Ultra.Engine.PerspectiveCamera;
import Ultra.Utility.Random;
import Ultra.Utility.Timer;

import <glm/glm.hpp>;
import <glm/gtc/constants.hpp>;
import <glm/gtx/compatibility.hpp>;

export namespace Ultra {

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
public:
    ParticleSystem(uint32_t maxParticles = 1000): ParticlePoolIndex { maxParticles - 1 } {
        ParticlePool.resize(maxParticles);
    }

    void Update(Timestamp deltaTime) {
        for (auto &particle : ParticlePool) {
            if (!particle.Active) continue;

            if (particle.LifeRemaining <= 0.0f) {
                particle.Active = false;
                continue;
            }

            particle.LifeRemaining -= static_cast<float>(deltaTime);
            particle.Position += particle.Velocity * (float)deltaTime;
            particle.Rotation += 0.01f * static_cast<float>(deltaTime);
        }
    }
    void Render(PerspectiveCamera &camera) {
        Renderer2D::StartScene(camera);
        for (auto &particle : ParticlePool) {
            if (!particle.Active) continue;

            float life = particle.LifeRemaining / particle.LifeTime;
            glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

            float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
            glm::vec3 position = { particle.Position.x, particle.Position.y, 0.2f };
            Renderer2D::DrawRotatedQuad(position, { size, size }, particle.Rotation, color);
        }
        Renderer2D::FinishScene();
    }

    void Emit([[maybe_unused]] const ParticleProperties &properties) {
        //Particle &particle = ParticlePool[ParticlePoolIndex];
        //particle.Active = true;
        //particle.Position = properties.Position;
        //particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

        //particle.Velocity = properties.Velocity;
        //particle.Velocity.x += properties.VelocityVariation.x * (Random::Float() - 0.5f);
        //particle.Velocity.y += properties.VelocityVariation.y * (Random::Float() - 0.5f);

        //particle.ColorBegin = properties.ColorBegin;
        //particle.ColorEnd = properties.ColorEnd;

        //particle.LifeTime = properties.LifeTime;
        //particle.LifeRemaining = properties.LifeTime;
        //particle.SizeBegin = properties.SizeBegin + properties.SizeVariation * (Random::Float() - 0.5f);
        //particle.SizeEnd = properties.SizeEnd;

        //ParticlePoolIndex = --ParticlePoolIndex % ParticlePool.size();
    }

private:
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
};

}
