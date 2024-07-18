#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

struct ProjectileEmitterComponent {
    glm::vec2 velocity;
    uint32_t repeatFrequency;
    int duration;
    int hitPercentDamage;
    bool isFriendly;
    uint32_t lastEmissionTime;

    ProjectileEmitterComponent(
        glm::vec2 velocity = glm::vec2(0),
        uint32_t repeatFrequency = 0,
        int duration = 10000,
        int hitPercentDamage = 10,
        bool isFriendly = true
    ) {
        this->velocity = velocity;
        this->repeatFrequency = repeatFrequency;
        this->duration = duration;
        this->hitPercentDamage = hitPercentDamage;
        this->isFriendly = isFriendly;
        this->lastEmissionTime = SDL_GetTicks();
    }
};
