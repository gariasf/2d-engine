#pragma once

#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

class AnimationSystem: public System {
    public:
        AnimationSystem() {
            RequireComponent<SpriteComponent>();
            RequireComponent<AnimationComponent>();
        }

        void Update(double deltaTime) {
            for (auto entity : GetEntities())
            {
                auto& sprite = entity.GetComponent<SpriteComponent>();
                auto& animation = entity.GetComponent<AnimationComponent>();

                animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;
                sprite.srcRect.x = animation.currentFrame * sprite.width;
            }
        }
};
