#ifndef BIRD_HPP
#define BIRD_HPP

#include <string>

#include "Obj.hpp"

struct SDL_Texture;

class Bird
{
public:
    Bird(SDL_Texture *texture, const float unit_ratio);
    ~Bird();

    inline constexpr Obj& GetObj() { return obj_; }

    void Update(const float delta_time);
    void Flap();
    void GameOver() { flap_delay_timer_ = 9999; }
    
private:
    Obj obj_;
    float fall_speed_ = 0;
    float flap_delay_timer_ = 0;

    const float kFlapPower;
    const float kGravitation;

    static constexpr inline float kFlapDelaySeconds = 0.35;

    static constexpr inline float kMaxRotation = 60;
    static constexpr inline float kMaxRotationDifference = 1;
};

#endif