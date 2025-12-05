#ifndef RCL_SPHERE
#define RCL_SPHERE

#include "hittable.hpp"
#include "aabb.hpp"

namespace rcl
{

class Sphere : public rcl::Hittable
{
public:
    Sphere() = delete;
    Sphere(const rcl::vec3& center, const double radius, std::shared_ptr<Material> mat);

    bool hit(const rcl::Ray& ray, const rcl::Interval<double>& interval, rcl::HitRecord& record) const override;

    const rcl::AABB& BoundingBox() const override;

    rcl::vec3 RandomPointOnSurface() const override;
    rcl::Ray RandomRayFromSurface() const override;
    std::shared_ptr<rcl::Material> GetMaterial() const override;
    
private:
    rcl::vec3 center;
    std::shared_ptr<Material> mat;
    double radius;
    AABB bbox;

    static void get_sphere_uv(const rcl::vec3& p, rcl::vec2& uv);
};

}
#endif