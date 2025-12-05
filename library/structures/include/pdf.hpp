#ifndef RCL_PDF
#define RCL_PDF

#include "vector.hpp"

namespace rcl
{

class PDF
{
public:
    virtual ~PDF() = default;

    virtual rcl::vec3 Generate(const rcl::Ray& in, const rcl::HitRecord& rec) const = 0;
    virtual double Probability(const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered) const = 0;
};

}

#endif