#include "vertex_triangle.hpp"

#include <iostream>

#include "materials.hpp"

rcl::VertexTriangle::VertexTriangle(rcl::Vertex a, rcl::Vertex b, rcl::Vertex c) 
: a(a), b(b), c(c)
{
    SetBoundingBox();
}

void rcl::VertexTriangle::SetBoundingBox()
{
    auto bbox_diagonal1 = rcl::AABB(a.coord, b.coord);
    auto bbox_diagonal2 = rcl::AABB(a.coord, c.coord);
    bbox = rcl::AABB(bbox_diagonal1, bbox_diagonal2);
}

const rcl::AABB& rcl::VertexTriangle::BoundingBox() const
{
    return bbox;
}
    
std::shared_ptr<rcl::Material> rcl::VertexTriangle::GetMaterial() const
{
    return nullptr;
}

bool rcl::VertexTriangle::hit
(const rcl::Ray& ray, const rcl::Interval<double>& interval, rcl::HitRecord& record) 
const
{
    rcl::vec3 e1 = b.coord - a.coord;
    rcl::vec3 e2 = c.coord - a.coord;
    rcl::vec3 P = rcl::Cross(ray.direction, e2);
    double dotPe1 = rcl::Dot(P, e1);

    if (std::fabs(dotPe1) <= 1e-8)
        return false;

    double invDotPe1 = 1 / dotPe1;
    rcl::vec3 T = ray.origin - a.coord;
    double u = rcl::Dot(P, T) * invDotPe1;

    if(u > 1 || u < 0)
        return false;

    rcl::vec3 Q = rcl::Cross(T, e1);
    double v = rcl::Dot(Q, ray.direction) * invDotPe1;

    if(u + v > 1 || v < 0)
        return false;

    double t = rcl::Dot(Q, e2) * invDotPe1;

    if(!interval.Contains(t))
        return false;

    record.distance = t;
    record.point = ray.At(t);
    record.uv = t * a.uv + u * b.uv + v * c.uv;
    record.SetNormal(ray, t * a.normal + u * b.normal + v * c.normal);
    //record.mat = std::make_shared<rcl::Lambertian>(rcl::vec3(0.0f));

    return true;
}
    
rcl::vec3 rcl::VertexTriangle::RandomPointOnSurface() const
{
    return a.coord + (b.coord - a.coord) * RandomDouble01() + (c.coord - a.coord) * RandomDouble01();
}
    
rcl::Ray rcl::VertexTriangle::RandomRayFromSurface() const
{
    rcl::vec3 P = RandomPointOnSurface();
    rcl::vec3 v0 = c.coord - a.coord;
    rcl::vec3 v1 = b.coord - a.coord;
    rcl::vec3 v2 = P - a.coord;
    
    double d00 = rcl::Dot(v0, v0);
    double d01 = rcl::Dot(v0, v1);
    double d11 = rcl::Dot(v1, v1);
    double d02 = rcl::Dot(v0, v2);
    double d12 = rcl::Dot(v1, v2);

    double denom = d00 * d11 - d01*d01;

    double beta = (d11 * d02 - d01 * d12) / denom;
    double gamma = (d00 * d12 - d01 * d02) / denom;
    double alpha = 1 - beta - gamma;

    rcl::vec3 n = a.normal * alpha + b.normal * beta + c.normal * gamma;
    rcl::vec3 dir = rcl::Ray::RandomOnHemisphere(n);

    return rcl::Ray(P, dir);    
}