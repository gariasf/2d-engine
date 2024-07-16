#pragma once

#include <SDL2/SDL.h>
#include <memory>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"

const int FPS = 60;
const int MS_PER_FRAME = 1000 / FPS;


class Game
{
private:
    bool isRunning;
    int msPreviousFrame = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> assetStore;

public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void ProcessInput();
    void Setup();
    void LoadLevel(int level);
    void Update();
    void Render();
    void Destroy();

    int windowWidth;
    int windowHeight;
};
