#pragma once

#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class MovementSystem: public System {
    public:
        MovementSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<RigidBodyComponent>();
        }

        void SubscribeToEvents(const std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
        }

        void OnCollision(CollisionEvent& event) {
            Entity a = event.a;
            Entity b = event.b;
            Logger::Log("Movement system recieved collision event between " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()) + ".");

            if(a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles")) {
                OnEnemyHitsObstacle(a, b);
            }

            if(a.BelongsToGroup("obstacles") && b.BelongsToGroup("enemies")) {
                OnEnemyHitsObstacle(b, a);
            }
        }

        void OnEnemyHitsObstacle(Entity enemy, Entity obstacle) {
            if(enemy.HasComponent<RigidBodyComponent>() && enemy.HasComponent<SpriteComponent>()) {
                auto& rigidbody = enemy.GetComponent<RigidBodyComponent>();
                auto& sprite = enemy.GetComponent<SpriteComponent>();

                if(rigidbody.velocity.x != 0) {
                    rigidbody.velocity.x *= -1;
                    sprite.flip = (sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
                }

                if(rigidbody.velocity.y != 0) {
                    rigidbody.velocity.y *= -1;
                    sprite.flip = (sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
                }
            }
        }

        void Update(double deltaTime) {
            for (auto entity : GetEntities())
            {
                auto& transform = entity.GetComponent<TransformComponent>();
                const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

                transform.position.x += rigidbody.velocity.x * deltaTime;
                transform.position.y += rigidbody.velocity.y * deltaTime;

                bool isEntityOutsideMap = (
                    transform.position.x < 0 ||
                    transform.position.x > Game::mapWidth ||
                    transform.position.y < 0 ||
                    transform.position.y > Game::mapHeight
                );

                if(isEntityOutsideMap && !entity.HasTag("player")) {
                    entity.Kill();
                }
            }
        }
};
