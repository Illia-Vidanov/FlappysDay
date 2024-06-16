#ifndef OBJ_HPP
#define OBJ_HPP

#include <string>

#include "Eigen/Dense"

struct SDL_Texture;

class Obj
{
public:
    Obj(SDL_Texture *texture) : texture_(texture) {}

    Eigen::Vector2i scale = Eigen::Vector2i::Zero();
    Eigen::Vector2f position = Eigen::Vector2f::Zero();
    float rotation = 0;

    inline constexpr SDL_Texture* GetTexture() const { return texture_; }
    inline void SetTexture(SDL_Texture *texture) { texture_ = texture; }

private:
    SDL_Texture *texture_;
};

#endif