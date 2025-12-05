#include "quad.hpp"

#include <iostream>

rcl::Quad::Quad(const rcl::vec3& Q, const rcl::vec3& u, const rcl::vec3& v, std::shared_ptr<rcl::Material> mat) 
: Q(Q), u(u), v(v), mat(mat) 
{
    rcl::vec3 n = rcl::Cross(u, v);
    normal = n.Unit();

    D = rcl::Dot(normal, Q);
    w = n / n.LengthSquared();

    SetBoundingBox();
}

void rcl::Quad::SetBoundingBox() 
{
    auto bbox_diagonal1 = rcl::AABB(Q, Q + u + v);
    auto bbox_diagonal2 = rcl::AABB(Q + u, Q + v);
    bbox = rcl::AABB(bbox_diagonal1, bbox_diagonal2);
}

const rcl::AABB& rcl::Quad::BoundingBox() const
{
    return bbox;
}

bool rcl::Quad::hit(const rcl::Ray& ray, const rcl::Interval<double>& interval, rcl::HitRecord& record) const
{
    double denom = rcl::Dot(normal, ray.direction);

    if(std::fabs(denom) <= 1e-8)
        return false;

    double t = (D - rcl::Dot(normal, ray.origin)) / denom;

    if(!interval.Contains(t))
        return false;

    rcl::vec3 intersect = ray.At(t);

    rcl::vec3 plannarHit = intersect - Q;
    double alpha = rcl::Dot(w, rcl::Cross(plannarHit, v));
    double beta = rcl::Dot(w, rcl::Cross(u, plannarHit));

    if(!IsInterior(alpha, beta))
        return false;

    record.point = intersect;
    record.distance = t;
    record.mat = mat;
    auto temp = (intersect - Q);
    record.uv.v = rcl::Dot(temp, v) / v.LengthSquared();
    record.uv.u = rcl::Dot(temp, u) / u.LengthSquared();
    record.SetNormal(ray, normal);
    record.object = this;

    return true;
}

bool rcl::Quad::IsInterior(double a, double b) const
{
    rcl::Interval<double> uInterval = rcl::Interval<double>(0, 1);

    if(!uInterval.Contains(a) || !uInterval.Contains(b))
    {
        return false;
    }

    return true;
}

rcl::vec3 rcl::Quad::RandomPointOnSurface() const
{
    return Q + u * RandomDouble01() + v * RandomDouble01();
}
    
rcl::Ray rcl::Quad::RandomRayFromSurface() const
{
    rcl::vec3 origin = RandomPointOnSurface();
    rcl::vec3 dir = rcl::Ray::RandomOnHemisphere(normal);
    return rcl::Ray(origin, dir);    
}

std::shared_ptr<rcl::Material> rcl::Quad::GetMaterial() const
{
    return mat;
}