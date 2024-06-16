#include "Game.hpp"

#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Eigen/Dense"

#include <iostream>
#include "Bird.hpp"
#include "World.hpp"

void Game::Start()
{
    // SDL2 Setup
    SDL_Init(SDL_INIT_EVERYTHING);
    window_ = SDL_CreateWindow("Flappy's Day!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 360,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(window_), &dm);
    window_size_ = { dm.h * 16 / 9, dm.w * 9 / 16 };
    SDL_SetWindowSize(window_, window_size_.x, window_size_.y);
    SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
    
    //window_size_ = { 640, 360 };

    unit_ratio_ = static_cast<float>(window_size_.x) / 640.0f;

    renderer_ = SDL_CreateRenderer(window_, -1, 0);
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // Game Setup

    game_over_screen_ = new Obj(IMG_LoadTexture(renderer_, "C:/Programming/c++/FlappysDay/res/Textures/Game over.png"));
    game_over_screen_->scale = { window_size_.x, window_size_.y };
    GameSetup();


    // Start main loop

    Loop();
}

void Game::Loop()
{
    while(running_)
    {
        Events();
        Update();
        Render();
    }

    Exit();
}

void Game::Events()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running_ = false;
            break;

        case SDL_KEYDOWN:
            if(game_over_)
            {
                delete bird_;
                delete world_;
                GameSetup();
            }

            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                bird_->Flap();
                break;

            case SDLK_ESCAPE:
                running_ = false;
                break;
            
            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}

void Game::Update()
{
    CalculateDeltaTime();

    bird_->Update(delta_time_);

    if(game_over_)
        return;

    world_->Update(delta_time_);

    CollisionCheck();
}

void Game::Render()
{
    SDL_RenderClear(renderer_);

    for(const auto &obj : world_->GetBackground())
        RenderObj(obj);
    for(const auto &obj : world_->GetPiepes())
        RenderObj(obj);

    RenderObj(bird_->GetObj());

    if(game_over_)
        RenderObj(*game_over_screen_);

    SDL_RenderPresent(renderer_);
}

void Game::Exit()
{
    delete world_;
    delete bird_;

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Game::CalculateDeltaTime()
{
    static uint64_t curr = SDL_GetPerformanceCounter();
    static uint64_t last = SDL_GetPerformanceCounter();
    const static uint64_t frequency = SDL_GetPerformanceFrequency();

    last = curr;
    curr = SDL_GetPerformanceCounter();

    delta_time_ = static_cast<float>(curr - last) / frequency;
}

void Game::RenderObj(const Obj &obj) const
{
    SDL_Rect rect = GetObjRect(obj);
    SDL_RenderCopyEx(renderer_, obj.GetTexture(), NULL, &rect, obj.rotation, NULL, SDL_FLIP_NONE);
}

void Game::CollisionCheck()
{
    SDL_Rect bird_rect = GetObjRect(bird_->GetObj());
    for(const auto &obj : world_->GetPiepes())
    {
        SDL_Rect piepe_rect = GetObjRect(obj);
        if(SDL_HasIntersection(&bird_rect, &piepe_rect))
            GameOver();
    }

    if(bird_->GetObj().position.y() < 0 || bird_->GetObj().position.y() > window_size_.y - bird_->GetObj().scale.y())
        GameOver();
}

void Game::GameOver()
{
    game_over_ = true;
    bird_->GameOver();
    std::cout << "Game over\n";
}

void Game::GameSetup()
{
    game_over_ = false;

    bird_ = new Bird(IMG_LoadTexture(renderer_, "C:/Programming/c++/FlappysDay/res/Textures/bird.png"), unit_ratio_);
    bird_->GetObj().position = { static_cast<int>(window_size_.x * 0.05), static_cast<int>(window_size_.y * 0.3) }; 
    bird_->GetObj().scale = { static_cast<int>(window_size_.x * 0.05), static_cast<int>(window_size_.x * 0.05) };

    world_ = new World(IMG_LoadTexture(renderer_, "C:/Programming/c++/FlappysDay/res/Textures/Background.png"),
                    IMG_LoadTexture(renderer_, "C:/Programming/c++/FlappysDay/res/Textures/PiepeTop.png"),
                    IMG_LoadTexture(renderer_, "C:/Programming/c++/FlappysDay/res/Textures/Piepe.png"),
                    window_size_, unit_ratio_);
}
