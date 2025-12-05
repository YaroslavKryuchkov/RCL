#include "ray.hpp"

#include "vector.hpp"
#include "constants.hpp"

namespace rcl
{

Ray::Ray(rcl::vec3 origin, rcl::vec3 direction) : origin(origin), direction(direction.Unit()){};
Ray::Ray(const rcl::Ray& original) : origin(original.origin), direction(original.direction){};

rcl::vec3 Ray::RandomOnHemisphere(const rcl::vec3& normal)
{
    rcl::vec3 w = normal.Unit();
    rcl::vec3 a = (fabs(w.x) > 0.9f ? rcl::vec3(0, 1, 0) : rcl::vec3(1, 0, 0));
    rcl::vec3 u = Cross(w, a).Unit();
    rcl::vec3 v = Cross(w, u);

    rcl::vec3 dir = rcl::vec3::RandomUnitVector();

    if (dir.z < 0)
        dir.z = -dir.z;
    
    return dir.x * u + dir.y * v + dir.z * w;
}

rcl::vec3 Ray::GetRandomDiskRay()
{
    while(true)
    {
        rcl::vec3 p(RandomDoubleMinMax(-1, 1), RandomDoubleMinMax(-1, 1), 0);
        if(p.LengthSquared() < 1)
            return p;
    }
}

rcl::vec3 Ray::At(float t) const
{
    return origin + t * direction;
}

rcl::vec3 Ray::RandomCosineDirection() 
{
    double r1 = rcl::RandomDouble01();
    double r2 = rcl::RandomDouble01();

    double phi = 2 * rcl::PI * r1;
    double x = std::cos(phi) * std::sqrt(r2);
    double y = std::sin(phi) * std::sqrt(r2);
    double z = std::sqrt(1-r2);

    return rcl::vec3(x, y, z).Unit();
}

}//namespace rcl