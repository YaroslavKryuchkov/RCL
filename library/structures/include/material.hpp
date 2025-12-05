#ifndef RCL_MATERIAL
#define RCL_MATERIAL

#include "ray.hpp"
#include "hittable.hpp"

namespace rcl
{

struct ScatterRecord
{
    bool skipBRDF;
    rcl::vec3 albedo;
    rcl::vec3 outVec;
    double probability;
};

class Material
{
public:
    virtual ~Material() = default;

    virtual rcl::vec3 Emitted(const rcl::HitRecord& rec) const
    {
        return rcl::vec3(0);
    }

    virtual rcl::vec3 IntenseEmitted(const rcl::HitRecord& rec) const
    {
        return rcl::vec3(0);
    }

    virtual bool Scatter
    (const rcl::Ray& in, const rcl::HitRecord& hitRec, rcl::ScatterRecord& scatterRec) 
    const
    {
        return false;
    }
    
    virtual rcl::vec3 BRDF
    (const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered)
    const 
    {
        return rcl::vec3(1);
    }

    virtual bool IsSpecular() const 
    {
        return false;
    }

    virtual bool IsRefractive() const 
    {
        return false;
    }

    virtual bool IsReflective() const 
    {
        return false;
    }
};

}
#endif