#ifndef RCL_QUAD
#define RCL_QUAD

#include <memory>

#include "hittable.hpp"
#include "AABB.hpp"
#include "material.hpp"

namespace rcl
{

class Quad : public rcl::Hittable
{
public:
    Quad(const rcl::vec3& Q, const rcl::vec3& u, const rcl::vec3& v, std::shared_ptr<rcl::Material> mat);

    void SetBoundingBox();

    const rcl::AABB& BoundingBox() const override;

    bool hit(const rcl::Ray& ray, const rcl::Interval<double>& interval, rcl::HitRecord& record) const override;

    bool IsInterior(double a, double b) const;

    rcl::vec3 RandomPointOnSurface() const override;
    rcl::Ray RandomRayFromSurface() const override;
    std::shared_ptr<rcl::Material> GetMaterial() const override;

private:
    rcl::vec3 Q;
    rcl::vec3 u; 
    rcl::vec3 v;
    rcl::vec3 w;
    rcl::AABB bbox;
    rcl::vec3 normal;
    std::shared_ptr<rcl::Material> mat;
    double D;
};

}
#endif