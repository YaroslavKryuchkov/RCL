#ifndef RCL_PATH_TRAYCER
#define RCL_PATH_TRAYCER

#include "ray_tracer.hpp"
#include "sampler.hpp"

namespace rcl
{

class PathTracer : public RayTracer
{
public:
    PathTracer(int sapmles, int maxDepth);
    void Render
    (const HittableList& world, Camera& cam, Picture& target, const HittableList& lights = HittableList()) 
    const override;

private:
    int samplePerPixel = 10;
    double pixelSamplesScale;
    Sampler sampler;
    int maxDepth = 50;
    vec3 backgroundColor = vec3(0.5);
    
    vec3 RayColor
    (const Ray& ray, int depth, const HittableList& world, const HittableList& lights = HittableList())
    const;
};

}
#endif