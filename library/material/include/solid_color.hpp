#ifndef RCL_SOLID_COLOR
#define RCL_SOLID_COLOR

#include "texture.hpp"

namespace rcl
{

class SolidColor : public Texture
{
public:
    SolidColor(color c = color(1));

    color GetColor(const vec2& uv = vec2(0)) const override;
private:
    color col;
};

}
#endif