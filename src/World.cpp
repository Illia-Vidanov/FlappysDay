#include "World.hpp"

#include <vector>
#include <iostream>

#include "SDL2/SDL.h"
#include "Eigen/Dense"
#include "random.hpp"

using Random = effolkronium::random_static;

#include "Obj.hpp"

World::World(SDL_Texture *background_texture, SDL_Texture *piepe_top_texture, SDL_Texture *piepe_texture, const SDL_Point &window_size, const float unit_ratio)
    : background_texture_(background_texture)
    , piepe_top_texture_(piepe_top_texture)
    , piepe_texture_(piepe_texture)
    , kBackgroundSpeed(-50 * unit_ratio)
    , kMinPiepeDistance(100 * unit_ratio)
    , kMaxPiepeDistance(200 * unit_ratio)
    , kMinPiepeSize(50 * unit_ratio)
    , kMaxPiepeSize(200 * unit_ratio)
    , kMinHoleSize(80 * unit_ratio)
    , kMaxHoleSize(100 * unit_ratio)
    , kPiepeTopWidth(84 * unit_ratio)
    , kPiepeTopHeight(32 * unit_ratio)
    , kPiepeWidth(64 * unit_ratio)
{
    background_.push_back(Obj(background_texture_));
    background_.push_back(Obj(background_texture_));
    background_[0].scale = { window_size.x, window_size.y };
    background_[1].scale = { window_size.x, window_size.y };
    background_[1].position = { window_size.x, 0 };
}

World::~World()
{
    SDL_DestroyTexture(background_texture_);
    SDL_DestroyTexture(piepe_top_texture_);
    SDL_DestroyTexture(piepe_texture_);
}

void World::Update(float delta_time)
{
    // Background

    float pixelChange = kBackgroundSpeed * delta_time;

    background_[0].position.x() += pixelChange;
    background_[1].position.x() += pixelChange;

    if(background_[leading_background_].position.x() < -background_[0].scale.x())
    {
        background_[leading_background_].position.x() += background_[0].scale.x() * 2;
        leading_background_ = !leading_background_;
    }

    // Piepes

    pixelsAdvanced_ -= pixelChange;

    if(pixelsAdvanced_ > nextPiepeDistance_)
    {
        nextPiepeDistance_ = Random::get(kMinPiepeDistance, kMaxPiepeDistance) + kPiepeWidth;
        pixelsAdvanced_ = 0;

        float piepe_size = Random::get(kMinPiepeSize, kMaxPiepeSize);
        float top_piepe_size = background_[0].scale.y() - piepe_size - Random::get(kMinHoleSize, kMaxHoleSize);

        piepes_.reserve(piepes_.size() + 4);

        Obj &piepe_top = piepes_.emplace_back(piepe_top_texture_);
        Obj &piepe = piepes_.emplace_back(piepe_texture_);

        Obj &top_piepe_top = piepes_.emplace_back(piepe_top_texture_);
        Obj &top_piepe = piepes_.emplace_back(piepe_texture_);

        piepe.position = { background_[0].scale.x() + (kPiepeTopWidth - kPiepeWidth) / 2, background_[0].scale.y() - piepe_size + kPiepeTopHeight };
        piepe_top.position = { background_[0].scale.x(), background_[0].scale.y() - piepe_size };

        top_piepe.position = { piepe.position.x(), 0 };
        top_piepe_top.position = { piepe_top.position.x(), top_piepe_size - kPiepeTopHeight };

        piepe.scale = { kPiepeWidth, piepe_size - kPiepeTopHeight };
        piepe_top.scale = { kPiepeTopWidth, kPiepeTopHeight };

        top_piepe.scale = { kPiepeWidth, top_piepe_size - kPiepeTopHeight };
        top_piepe_top.scale = { kPiepeTopWidth, kPiepeTopHeight };
    }

    for(std::size_t i = 0; i < piepes_.size(); i++)
    {
        piepes_[i].position.x() += pixelChange;

        if(piepes_[i].position.x() < -kPiepeTopWidth)
        {
            // Order of instantiation is the folowing: piepe top, piepe, top piepe top, top piepe, so we can assume that next 3 values need to be erased too
            piepes_.erase(piepes_.begin() + i, piepes_.begin() + i + 4);
            i += 3;
        }
    }
}