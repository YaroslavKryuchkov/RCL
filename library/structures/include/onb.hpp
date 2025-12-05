#ifndef RCL_ONB
#define RCL_ONB

#include "vector.hpp"

namespace rcl
{

class ONB 
{
public:
    ONB(const rcl::vec3& n) 
    {
        axis[2] = n.Unit();
        rcl::vec3 a = (std::fabs(axis[2].x) > 0.99) ? rcl::vec3(0,1,0) : rcl::vec3(1,0,0);
        axis[1] = rcl::Cross(axis[2], a).Unit();
        axis[0] = rcl::Cross(axis[2], axis[1]).Unit();
    }

    const rcl::vec3& u() const { return axis[0]; }
    const rcl::vec3& v() const { return axis[1]; }
    const rcl::vec3& w() const { return axis[2]; }

    rcl::vec3 Transform(const rcl::vec3& v) const 
    {
        return (v.x * axis[0]) + (v.y * axis[1]) + (v.z * axis[2]);
    }

private:
    rcl::vec3 axis[3];
};

}
#endif