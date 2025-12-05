#include <iostream>
#include <memory>

#include "camera.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "vector.hpp"
#include "materials.hpp"
#include "bvh.hpp"
#include "quad.hpp"
#include "vertex_triangle.hpp"
#include "mesh.hpp"
#include "path_tracer.hpp"
#include "solid_color.hpp"
#include "picture_texture.hpp"

void CornelBox()
{    
    rcl::HittableList world;
    rcl::HittableList lightList;
    auto red = std::make_shared<rcl::Lambertian>(std::make_shared<rcl::SolidColor>(rcl::vec3(.65, .05, .05)));
    auto white = std::make_shared<rcl::Lambertian>(std::make_shared<rcl::SolidColor>(rcl::vec3(.73, .73, .73)));
    auto green = std::make_shared<rcl::Lambertian>(std::make_shared<rcl::SolidColor>(rcl::vec3(.12, .45, .15)));
    auto light = std::make_shared<rcl::Light>(rcl::vec3(1), 1); // Increased intensity intensity
   
    // Left wall (green)
    world.Add
    (
        std::make_shared<rcl::Quad>
        (
            rcl::vec3(555, 0, 0), 
            rcl::vec3(0, 0, 555), 
            rcl::vec3(0, 555, 0), 
            green
        )
    );
    
    // Right wall (red)
    world.Add
    (
        std::make_shared<rcl::Quad>
        (
            rcl::vec3(0, 0, 0), 
            rcl::vec3(0, 0, 555), 
            rcl::vec3(0, 555, 0), 
            red
        )
    );
    
    // Ceiling light
    auto lightObj = std::make_shared<rcl::Quad>(
        rcl::vec3(343, 554, 332), 
        rcl::vec3(-130, 0, 0), 
        rcl::vec3(0, 0, -105), 
        light
    );
    world.Add(lightObj);
    lightList.Add(lightObj); // Add to light list for direct sampling
    
    // Floor (white)
    world.Add(
        std::make_shared<rcl::Quad>(
            rcl::vec3(0, 0, 0), 
            rcl::vec3(0, 0, 555), 
            rcl::vec3(555, 0, 0), 
            white
        )
    );
    
    // Ceiling (white)
    world.Add(
        std::make_shared<rcl::Quad>(
            rcl::vec3(555, 555, 555), 
            rcl::vec3(-555, 0, 0), 
            rcl::vec3(0, 0, -555), 
            white
        )
    );
    
    // Back wall (white)
    world.Add(
        std::make_shared<rcl::Quad>(
            rcl::vec3(0, 0, 555), 
            rcl::vec3(555, 0, 0), 
            rcl::vec3(0, 555, 0), 
            white
        )
    );
    
    // Metal sphere
    auto metalMaterial = std::make_shared<rcl::Metal>(std::make_shared<rcl::SolidColor>(rcl::vec3(0.8)), 0, 1.0);
    world.Add(std::make_shared<rcl::Sphere>(rcl::vec3(190, 90, 190), 90, metalMaterial));
    
    // Glass sphere
    auto glassMaterial = std::make_shared<rcl::Dielectric>(std::make_shared<rcl::SolidColor>(rcl::vec3(1)), 1.5);
    world.Add(std::make_shared<rcl::Sphere>(rcl::vec3(370, 90, 370), 90, glassMaterial));
    
    // Build BVH for efficiency
    world = rcl::HittableList(std::make_shared<rcl::BVHNode>(world));
    
    // Camera setup
    rcl::Camera cam;
    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    
    cam.lookFrom = rcl::vec3(278, 278, -800);
    cam.lookAt = rcl::vec3(278, 278, 0);
    cam.up = rcl::vec3(0, 1, 0);
    cam.vfov = 40;
    cam.defocusAngle = 0;
    cam.focusDistance = 10;
    
    // VCM render
    rcl::Picture target;
    
    // Create VCM renderer with more light paths and samples
    rcl::PathTracer path(100, 100);

    path.Render(world, cam, target, lightList);

    target.GammaCorection();
    target.Export("PathCornelBox.png");
}

int main()
{
    CornelBox();

    return 0;
}