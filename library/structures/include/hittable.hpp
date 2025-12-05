#ifndef RCL_HITTABLE
#define RCL_HITTABLE

#include <memory>

#include "ray.hpp"
#include "interval.hpp"
#include "aabb.hpp"
#include "hit_record.hpp"

namespace rcl
{

class Material;

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& ray, const Interval<double>& interval, HitRecord& record) const = 0;

    virtual const AABB& BoundingBox() const = 0;

    virtual vec3 RandomPointOnSurface() const = 0;
    virtual Ray RandomRayFromSurface() const = 0;
    virtual std::shared_ptr<Material> GetMaterial() const = 0;
};

}

#endif