#include "path_tracer.hpp"

#include <iostream>
#include <thread>
#include <future>
#include <vector>

namespace rcl
{

PathTracer::PathTracer(int sapmles, int maxDepth) : samplePerPixel(sapmles), maxDepth(maxDepth), sampler(sapmles)
{
    pixelSamplesScale = 1.0 / sapmles;
}

void PathTracer::Render
(const HittableList& world, Camera& cam, Picture& target, const HittableList& lights) const
{
    cam.Initialize();
    target = Picture(cam);
    unsigned int height = cam.GetImageHeight();
    unsigned int width = cam.GetImageWidth();
    
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 1; // fallback if detection fails
    
    int rowsPerThread = height / numThreads;
    int remainder = height % numThreads;
    
    std::vector<std::future<void>> futures;
    
    for (unsigned int t = 0; t < numThreads; ++t)
    {
        int startRow = t * rowsPerThread;
        int endRow = (t == numThreads - 1) ? startRow + rowsPerThread + remainder : startRow + rowsPerThread;
        
        futures.push_back(std::async(std::launch::async, [this, startRow, endRow, width, &world, &cam, &target, &lights]()
        {
            for (int i = startRow; i < endRow; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    vec3 pixelColor(0);
                    for(int s = 0; s < samplePerPixel; s++)
                    {
                        Ray r = cam.GetRay(i, j, sampler.GetSampleOffset(s));
                        pixelColor += RayColor(r, 1, world, lights);
                    }

                    pixelColor *= pixelSamplesScale;

                    pixelColor.x /= pixelColor.x + 1;
                    pixelColor.y /= pixelColor.y + 1;
                    pixelColor.z /= pixelColor.z + 1;

                    target.WritePixel(i, j, pixelColor);
                }
            }
        }));
    }
    
    for (auto& future : futures)
    {
        future.wait();
    }
}

vec3 PathTracer::RayColor
(const Ray& ray, int depth, const HittableList& world, const HittableList& lights)
const
{    
    if(depth > maxDepth)
        return vec3(0);

    HitRecord rec;
    if(!world.hit(ray, Interval<double>(0.0001, +infinity), rec))
        return backgroundColor;

    ScatterRecord scatterRec;
    vec3 emission = rec.mat->IntenseEmitted(rec);
    if(!rec.mat->Scatter(ray, rec, scatterRec))
        return emission;

    Ray scattered(rec.point, scatterRec.outVec);
    vec3 scatter;
    if(scatterRec.skipBRDF)
        scatter = scatterRec.albedo * RayColor(scattered, depth+1, world, lights);
    else
        scatter = rec.mat->BRDF(ray, rec, scattered) * RayColor(scattered, depth+1, world, lights)
                * Dot(rec.normal, scattered.direction)
                / scatterRec.probability;
    
    return emission + scatter;
}

}