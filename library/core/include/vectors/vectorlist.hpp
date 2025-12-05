#include "vector1.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

namespace rcl
{
    
using vec1 = rcl::Vector<1, float>;
using vec2 = rcl::Vector<2, float>;
using uv = rcl::Vector<2, float>;
using vec3 = rcl::Vector<3, float>;
using color = rcl::Vector<3, float>;
using vec4 = rcl::Vector<4, float>;

using dvec1 = rcl::Vector<1, double>;
using dvec2 = rcl::Vector<2, double>;
using dvec3 = rcl::Vector<3, double>;
using dvec4 = rcl::Vector<4, double>;

using bvec1 = rcl::Vector<1, bool>;
using bvec2 = rcl::Vector<2, bool>;
using bvec3 = rcl::Vector<3, bool>;
using bvec4 = rcl::Vector<4, bool>;

using ivec1 = rcl::Vector<1, int>;
using ivec2 = rcl::Vector<2, int>;
using ivec3 = rcl::Vector<3, int>;
using ivec4 = rcl::Vector<4, int>;

using uvec1 = rcl::Vector<1, unsigned int>;
using uvec2 = rcl::Vector<2, unsigned int>;
using uvec3 = rcl::Vector<3, unsigned int>;
using uvec4 = rcl::Vector<4, unsigned int>;

}