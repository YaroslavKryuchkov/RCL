#ifndef RCL_TRIANGLE
#define RCL_TRIANGLE

#include <memory>

#include "hittable.hpp"
#include "vector.hpp"
#include "Material.hpp"

namespace rcl
{

struct Vertex
{
    rcl::vec3 coord;
    rcl::vec3 normal;
    rcl::vec2 uv;

    Vertex(){}
    Vertex(rcl::vec3 coord, rcl::vec3 n, rcl::vec2 uv) : coord(coord), normal(n), uv(uv) {}
};

class VertexTriangle : public Hittable
{
public:
    VertexTriangle(rcl::Vertex a, rcl::Vertex b, rcl::Vertex c);

    void SetBoundingBox();
    const rcl::AABB& BoundingBox() const;

    bool hit(const rcl::Ray& ray, const rcl::Interval<double>& interval, rcl::HitRecord& record) const;

    rcl::vec3 RandomPointOnSurface() const override;
    rcl::Ray RandomRayFromSurface() const override;
    std::shared_ptr<rcl::Material> GetMaterial() const override;
private:
    rcl::AABB bbox;
    rcl::Vertex a;
    rcl::Vertex b; 
    rcl::Vertex c;
};

}
#endif