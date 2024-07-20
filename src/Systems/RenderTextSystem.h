#pragma once

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "../Components/TextLabelComponent.h"
#include <SDL2/SDL_ttf.h>

class RenderTextSystem: public System {
    public:
        RenderTextSystem() {
            RequireComponent<TextLabelComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera) {
           for(auto entity: GetEntities()) {
               const auto textLabel = entity.GetComponent<TextLabelComponent>();

               SDL_Surface* surface = TTF_RenderText_Blended(
                   assetStore->GetFont(textLabel.assetId),
                   textLabel.text.c_str(),
                   textLabel.color
               );

               SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
               SDL_FreeSurface(surface);

               int labelWidth;
               int labelHeight;

               SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

               SDL_Rect dstRect = {
                   static_cast<int>(textLabel.position.x - (textLabel.isFixed ? 0 : camera.x)),
                   static_cast<int>(textLabel.position.y - (textLabel.isFixed ? 0 : camera.y)),
                   labelWidth,
                   labelHeight
               };

               SDL_RenderCopy(renderer, texture, NULL, &dstRect);
           }
        }
};
