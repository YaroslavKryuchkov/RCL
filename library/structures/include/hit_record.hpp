#ifndef RCL_HIT_RECORD
#define RCL_HIT_RECORD

#include <memory>

#include "ray.hpp"

namespace rcl
{

class Material;
class Hittable;

struct HitRecord
{
    rcl::vec3 point;
    rcl::vec2 uv;
    rcl::vec3 normal;
    std::shared_ptr<rcl::Material> mat;
    double distance;
    bool frontFace;
    mutable const rcl::Hittable* object;
    
    void SetNormal(const rcl::Ray& ray, const rcl::vec3& outward_normal)
    {
        frontFace = rcl::Dot(ray.direction, outward_normal) < 0;
        normal = frontFace ? outward_normal : -outward_normal;
    }
};

}

#endif