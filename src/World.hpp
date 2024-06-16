#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>

#include "Obj.hpp"

struct SDL_Texture;
struct SDL_Point;

class World
{
public:
    World(SDL_Texture *background_texture, SDL_Texture *piepe_top_texture, SDL_Texture *piepe_texture, const SDL_Point &window_size, const float unit_ratio);
    ~World();

    void Update(float delta_time);

    inline constexpr const std::vector<Obj>& GetBackground() const { return background_; }
    inline constexpr const std::vector<Obj>& GetPiepes() const { return piepes_; }

private:
    std::vector<Obj> piepes_;
    std::vector<Obj> background_;

    SDL_Texture *background_texture_;
    SDL_Texture *piepe_top_texture_;
    SDL_Texture *piepe_texture_;

    bool leading_background_ = 0;

    const float kBackgroundSpeed; // Pixels per second
    const float kMinPiepeDistance; // In pixels
    const float kMaxPiepeDistance; // In pixels
    const float kMinPiepeSize; // In pixels
    const float kMaxPiepeSize; // In pixels
    const float kMinHoleSize; // In pixels
    const float kMaxHoleSize; // In pixels
    const float kPiepeTopWidth; // In pixels
    const float kPiepeTopHeight; // In pixels
    const float kPiepeWidth; // In pixels

    float pixelsAdvanced_ = 10000; // A lot
    int nextPiepeDistance_ = 0;

};

#endif