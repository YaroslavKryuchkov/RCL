#include "aabb.hpp"

rcl::AABB::AABB() {}
rcl::AABB::AABB
(const rcl::Interval<double>& x, const rcl::Interval<double>& y, const rcl::Interval<double>& z) 
: x(x), y(y), z(z) 
{
    PadToMinimum();
}

rcl::AABB::AABB(const rcl::vec3& a, const rcl::vec3& b)
{
    x = (a.x <= b.x) ? rcl::Interval<double>(a.x, b.x) : rcl::Interval<double>(b.x, a.x);
    y = (a.y <= b.y) ? rcl::Interval<double>(a.y, b.y) : rcl::Interval<double>(b.y, a.y);
    z = (a.z <= b.z) ? rcl::Interval<double>(a.z, b.z) : rcl::Interval<double>(b.z, a.z);
    PadToMinimum();
}
rcl::AABB::AABB(const rcl::AABB& a, const rcl::AABB& b)
{
    x = rcl::Interval<double>(a.x, b.x);
    y = rcl::Interval<double>(a.y, b.y);
    z = rcl::Interval<double>(a.z, b.z);
    PadToMinimum();
}

const rcl::Interval<double>& rcl::AABB::AxisInterval(int i) const
{
    if(i == 1) return y;
    if(i == 2) return z;
    return x;
}

int rcl::AABB::LongestAxis() const
{
    if(x.Size() > y.Size())
        return x.Size() > z.Size() ? 0 : 2;
    else
        return y.Size() > z.Size() ? 1 : 2;
}

double rcl::AABB::Volume() const
{
    return x.Size() * y.Size() * z.Size();
}
    
double rcl::AABB::DiagonalLength() const
{
    rcl::vec3 v1 = rcl::vec3(x.max, y.max, z.max);
    rcl::vec3 v2 = rcl::vec3(x.min, y.min, z.min);
    return (v1 - v2).Length();
}

void rcl::AABB::PadToMinimum()
{
    double delta = 0.0001;
    if(x.Size() < delta) x = x.Expanded(delta);
    if(y.Size() < delta) y = y.Expanded(delta);
    if(z.Size() < delta) z = z.Expanded(delta);
}

bool rcl::AABB::Hit(const rcl::Ray& ray, rcl::Interval<double> t) const
{
    for (int axis = 0; axis < 3; axis++) 
    {
        const rcl::Interval<double>& ax = AxisInterval(axis);
        const double adinv = 1.0 / ray.direction[axis];

        auto t0 = (ax.min - ray.origin[axis]) * adinv;
        auto t1 = (ax.max - ray.origin[axis]) * adinv;

        if (t0 < t1) {
            if (t0 > t.min) t.min = t0;
            if (t1 < t.max) t.max = t1;
        } else {
            if (t1 > t.min) t.min = t1;
            if (t0 < t.max) t.max = t0;
        }

        if (t.max <= t.min)
            return false;
    }
    return true;
}

const rcl::AABB rcl::AABB::empty 
= rcl::AABB(rcl::Interval<double>::empty, rcl::Interval<double>::empty, rcl::Interval<double>::empty);

const rcl::AABB rcl::AABB::universal = 
rcl::AABB(rcl::Interval<double>::universal, rcl::Interval<double>::universal, rcl::Interval<double>::universal);