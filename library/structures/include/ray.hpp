#ifndef RCL_RAY
#define RCL_RAY

#include "vector.hpp"

namespace rcl
{

struct Ray
{
public:
    rcl::vec3 origin;
    rcl::vec3 direction;

    Ray(rcl::vec3 origin = rcl::vec3(0), rcl::vec3 direction = rcl::vec3(0, 0, -1));
    Ray(const rcl::Ray& original);

    rcl::vec3 At(float t) const;
    
    static rcl::vec3 RandomOnHemisphere(const rcl::vec3& normal);
    static rcl::vec3 GetRandomDiskRay();
    static rcl::vec3 RandomCosineDirection();
};

}//namespace rcl
#endif