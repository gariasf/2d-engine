#pragma once

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"
#include <SDL2/SDL.h>

class ProjectileLifecycleSystem: public System {
    public:
        ProjectileLifecycleSystem() {
            RequireComponent<ProjectileComponent>();
        }

        void Update() {
           for(auto entity: GetEntities()) {
                const auto projectile = entity.GetComponent<ProjectileComponent>();

                if(SDL_GetTicks() - projectile.startTime > projectile.duration) {
                    entity.Kill();
                }
           }
        }
};
