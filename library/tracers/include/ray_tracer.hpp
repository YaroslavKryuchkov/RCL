#ifndef RCL_RAY_TRACER
#define RCL_RAY_TRACER

#include "hittable_list.hpp"
#include "camera.hpp"
#include "picture.hpp"

namespace rcl
{

class RayTracer
{
public:
    virtual ~RayTracer() = default;

    virtual void Render
    (const rcl::HittableList& world, rcl::Camera& cam, rcl::Picture& target, const rcl::HittableList& lights) 
    const = 0;
};

}
#endif