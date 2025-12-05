#ifndef RCL_TEXTURE
#define RCL_TEXTURE

#include "vector.hpp"

namespace rcl
{

class Texture
{
public: 
    virtual ~Texture() = default; 
    virtual color GetColor(const uv&  uv) const = 0;
};

}
#endif