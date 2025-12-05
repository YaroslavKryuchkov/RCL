#include "sphere.hpp"

namespace rcl
{

Sphere::Sphere(const vec3& center, const double radius, std::shared_ptr<Material> mat) 
: center(center), radius(fmax(0,radius)), mat(mat) 
{
    vec3 tpoint = vec3(radius, radius, radius);
    bbox = AABB(center - tpoint, center + tpoint);
}

bool Sphere::hit(const Ray& ray, const Interval<double>& interval, HitRecord& record) const
{
    vec3 oc = center - ray.origin;
    double h = Dot(ray.direction, oc);
    double c = oc.LengthSquared() - radius * radius;
    float discriminant = h*h - c;
    
    if(discriminant < 0) return false;

    float sqrt_disc = sqrt(discriminant);
    float root = (h - sqrt_disc);
    if(!(interval.Contains(root)) || root <= 0)
    {
        root = (h + sqrt_disc);
        if(!(interval.Contains(root)) || root <= 0)
            return false;
    }

    record.distance = root;
    record.point = ray.At(record.distance);
    vec3 outward_normal = (record.point - center) / radius;
    record.SetNormal(ray, outward_normal);
    record.mat = mat;
    record.object = this;
    get_sphere_uv(outward_normal, record.uv);

    return true;
}

const AABB& Sphere::BoundingBox() const
{
    return bbox;
}

vec3 Sphere::RandomPointOnSurface() const
{
    return vec3::RandomUnitVector() * radius;
}
    
Ray Sphere::RandomRayFromSurface() const
{
    vec3 origin = RandomPointOnSurface();
    vec3 n = (origin - center).Unit();
    vec3 dir = Ray::RandomOnHemisphere(n);
    return Ray(origin, dir);    
}

std::shared_ptr<Material> Sphere::GetMaterial() const
{
    return mat;
}

void Sphere::get_sphere_uv(const vec3& p, vec2& uv) 
{
    auto theta = std::acos(-p.y);
    auto phi = std::atan2(-p.z, p.x) + PI;

    uv.u = phi / (2 * PI);
    uv.v = theta / PI;
}

}