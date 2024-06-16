#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "Eigen/Dense"

#include "Obj.hpp"

class Bird;
class World;

class Game
{
public:
    void Start();

private:
    void Loop();
    void Render();
    void Update();
    void Events();
    void Exit();

    void CalculateDeltaTime();
    void RenderObj(const Obj &obj) const;
    void CollisionCheck();
    void GameOver();
    void GameSetup();

    static SDL_Rect GetObjRect(const Obj &obj) { return { static_cast<int>(obj.position.x()), static_cast<int>(obj.position.y()), obj.scale.x(), obj.scale.y() }; }

    SDL_Window *window_ = nullptr;
    SDL_Point window_size_;
    SDL_Renderer *renderer_ = nullptr;
    bool running_ = true;
    bool game_over_ = false;

    Bird *bird_;
    World *world_;
    Obj *game_over_screen_;

    float delta_time_;
    float unit_ratio_;
};

#endif