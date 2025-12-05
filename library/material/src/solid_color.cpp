#include "solid_color.hpp"

namespace rcl
{

SolidColor::SolidColor(color c) : col(c) {}

color SolidColor::GetColor(const uv&  uv) const
{
    return col;
}

}//namespace rcl