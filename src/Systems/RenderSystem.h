#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include <algorithm>

#include <SDL2/SDL.h>

class RenderSystem : public System
{
public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, SDL_Rect& camera)
    {
        struct RenderableEntity {
            TransformComponent transform;
            SpriteComponent sprite;
        };

        std::vector<RenderableEntity> renderableEntities;
        for (auto entity: GetEntities()) {
            RenderableEntity renderableEntity;
            renderableEntity.sprite = entity.GetComponent<SpriteComponent>();
            renderableEntity.transform = entity.GetComponent<TransformComponent>();

            bool isEntityOutsideCameraView = (
                renderableEntity.transform.position.x + (renderableEntity.transform.scale.x * renderableEntity.sprite.width) < camera.x ||
                renderableEntity.transform.position.x > camera.x + camera.w ||
                renderableEntity.transform.position.y + (renderableEntity.transform.scale.y * renderableEntity.sprite.height) < camera.y ||
                renderableEntity.transform.position.y > camera.y + camera.h
            );

            if(isEntityOutsideCameraView && !renderableEntity.sprite.isFixed) {
                continue;
            }

            renderableEntities.emplace_back(renderableEntity);
        }

        std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b){
            return a.sprite.zIndex < b.sprite.zIndex;
        });

        for (auto entity : renderableEntities)
        {
            const auto transform = entity.transform;
            const auto sprite = entity.sprite;

            SDL_Rect srcRect = sprite.srcRect;

            SDL_Rect dstRect = {
                static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
                static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)};

            SDL_RenderCopyEx(
                renderer,
                assetStore->GetTexture(sprite.assetId),
                &srcRect,
                &dstRect,
                transform.rotation,
                NULL,
                sprite.flip);
        }
    }
};
