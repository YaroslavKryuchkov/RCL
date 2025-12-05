#ifndef RCL_HITTABLE_LIST
#define RCL_HITTABLE_LIST

#include <memory>
#include <vector>

#include "hittable.hpp"
#include "aabb.hpp"
#include "material.hpp"
#include "interval.hpp"

namespace rcl
{

class HittableList : public Hittable 
{
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> object);
    ~HittableList();

    void Clear();

    void Add(std::shared_ptr<Hittable> object);

    bool hit(const Ray& r, const Interval<double>& interval, HitRecord& rec) const override;
    const AABB& BoundingBox() const override;
    vec3 RandomPointOnSurface() const override;
    Ray RandomRayFromSurface() const override;
    std::shared_ptr<Material> GetMaterial() const override;
private:
    AABB bbox;
};

}//namespace rcl
#endif