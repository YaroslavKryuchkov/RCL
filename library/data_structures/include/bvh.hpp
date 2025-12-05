#ifndef RCL_BHV
#define RCL_BHV

#include <vector>
#include <memory>

#include "hittable.hpp"
#include "hittable_list.hpp"

namespace rcl
{

class BVHNode : public Hittable
{
public:
    BVHNode(HittableList list);
    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end);

    bool hit(const Ray& ray, const Interval<double>& interval, HitRecord& record) const override;

    const AABB& BoundingBox() const override;
    vec3 RandomPointOnSurface() const override;
    Ray RandomRayFromSurface() const override;
    std::shared_ptr<Material> GetMaterial() const override;
private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB bbox;

    static bool BoxCompare
    (const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axisIndex);

    static bool BoxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
    static bool BoxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
    static bool BoxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
};

}//namespace rcl
#endif