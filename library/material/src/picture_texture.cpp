#include "picture_texture.hpp"

#include <iostream>

namespace rcl
{

PictureTexture::PictureTexture(const char* path) : pic(std::make_shared<Picture>(path)) {}

vec3 PictureTexture::GetColor(const vec2& uv) const
{
    return pic->GetPixel(uv);
}

void PictureTexture::SetPicture(std::shared_ptr<Picture> copy)
{
    pic = copy;
}

void PictureTexture::SetPicture(const char* path)
{
    pic = std::make_shared<Picture>(path);
}

}