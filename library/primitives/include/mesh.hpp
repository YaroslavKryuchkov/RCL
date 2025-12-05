#ifndef RCL_MESH
#define RCL_MESH

#include <memory>

#include "hittable.hpp"
#include "hittable_list.hpp"
#include "vertex_triangle.hpp"
#include "material.hpp"

namespace rcl
{
    
class Mesh : public Hittable
{
public:
    Mesh() = delete;

    Mesh(const char* file, std::shared_ptr<rcl::Material> mat);

    void Import(const char* file);

    bool hit(const rcl::Ray& ray, const rcl::Interval<double>& interval, rcl::HitRecord& record) const;

    const rcl::AABB& BoundingBox() const;

    rcl::vec3 RandomPointOnSurface() const override;
    rcl::Ray RandomRayFromSurface() const override;
    std::shared_ptr<rcl::Material> GetMaterial() const override;
private:
    rcl::HittableList triangles;
    std::shared_ptr<rcl::Material> mat;
};

}
#endif