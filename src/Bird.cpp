#include "Bird.hpp"

#include "SDL2/SDL.h"
#include "Eigen/Dense"
#include <iostream>

Bird::Bird(SDL_Texture *texture, float unit_ratio)
    : obj_(texture)
    , kFlapPower(80 * unit_ratio)
    , kGravitation(-120 * unit_ratio)
{}

Bird::~Bird()
{
    SDL_DestroyTexture(GetObj().GetTexture());
}

void Bird::Update(float delta_time)
{
    // Down is up and up is down, thus -=
    GetObj().position.y() -= fall_speed_ * delta_time;
    fall_speed_ += kGravitation * delta_time;

    flap_delay_timer_ -= delta_time;

    GetObj().rotation += std::clamp(fall_speed_ < 0 ?
                                        kMaxRotation * fall_speed_ / kGravitation - GetObj().rotation :
                                        -kMaxRotation * fall_speed_ / kFlapPower - GetObj().rotation,
                                        -kMaxRotationDifference, kMaxRotationDifference);
    GetObj().rotation = std::clamp(GetObj().rotation, -kMaxRotation, kMaxRotation);
}

void Bird::Flap()
{
    if(flap_delay_timer_ < 0)
    {
        fall_speed_ = kFlapPower;
        flap_delay_timer_ = kFlapDelaySeconds;
    }
}