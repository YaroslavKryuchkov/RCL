#ifndef RCL_AABB
#define RCL_AABB

#include "interval.hpp"
#include "vector.hpp"
#include "ray.hpp"

namespace rcl
{

struct AABB
{
    Interval<double> x, y, z;

    AABB();
    AABB(const Interval<double>& x, const Interval<double>& y, const Interval<double>& z);
    AABB(const vec3& a, const vec3& b);
    AABB(const AABB& a, const AABB& b);

    const Interval<double>& AxisInterval(int i) const;
    int LongestAxis() const;
    double Volume() const;
    double DiagonalLength() const;

    void PadToMinimum();

    bool Hit(const Ray& ray, Interval<double> t) const;

    static const AABB empty;
    static const AABB universal;
};

}
#endif