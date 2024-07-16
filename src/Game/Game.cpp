#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include <iostream>
#include "glm/glm.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Game::Game()
{
    isRunning = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    Logger::Log("Game created.");
}
Game::~Game()
{
    Logger::Log("Game destroyed.");
}

void Game::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Err("Error initializing SDL.");
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    windowWidth = 800;
    windowHeight = 600;

    window = SDL_CreateWindow(
        "Engine Experiments",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS);

    if (!window)
    {
        Logger::Err("Error creating window.");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        Logger::Err("Error creating renderer.");
        return;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    isRunning = true;
}
void Game::ProcessInput()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
            break;
        }
    }
}

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup()
{
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();

    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");

    Entity tank = registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(2.0, 2.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(40.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32);

    Entity truck = registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(50.0, 100.0), glm::vec2(2.0, 2.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 50.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32);
}

void Game::Update()
{
    int msToDelay = MS_PER_FRAME - (SDL_GetTicks() - msPreviousFrame);
    if (msToDelay > 0 && msToDelay <= MS_PER_FRAME)
    {
        SDL_Delay(msToDelay);
    }

    double deltaTime = (SDL_GetTicks() - msPreviousFrame) / 1000.0;

    msPreviousFrame = SDL_GetTicks();

    registry->Update();

    registry->GetSystem<MovementSystem>().Update(deltaTime);
}
void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

    SDL_RenderPresent(renderer);
}
void Game::Run()
{
    Setup();
    while (isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }
}
void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}