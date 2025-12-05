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

void Spheres()
{
    rcl::HittableList world;
    rcl::HittableList lightList;

    auto materialGround = std::make_shared<rcl::Lambertian>(std::make_shared<rcl::SolidColor>(rcl::vec3(0.5)));
    world.Add(std::make_shared<rcl::Sphere>(rcl::vec3(0,-1000,-1), 1000, materialGround));

    for(int a = -11; a < 11; a++)
        for(int b = -11; b < 11; b++)
        {
            double chooseMat = rcl::RandomDouble01();
            rcl::vec3 center = rcl::vec3(a + 0.9 * rcl::RandomDouble01(), 0.2, b + 0.9 * rcl::RandomDouble01());
            
            if((center - rcl::vec3(4, 0.2, 0)).Length() > 0.9)
            {
                std::shared_ptr<rcl::Material> sphereMaterial;
                if (chooseMat < 0.8)
                {
                    rcl::vec3 albedo = rcl::vec3::RandomVector(0, 1) * rcl::vec3::RandomVector(0, 1);
                    sphereMaterial = std::make_shared<rcl::Lambertian>(std::make_shared<rcl::PictureTexture>("PathCornelBox.png"));
                    world.Add(std::make_shared<rcl::Sphere>(center, 0.2, sphereMaterial));
                } 
                else if (chooseMat < 0.95)
                {
                    rcl::vec3 albedo = rcl::vec3::RandomVector(0.5, 1);
                    double metalic = rcl::RandomDoubleMinMax(0, 1);
                    sphereMaterial = std::make_shared<rcl::Metal>(std::make_shared<rcl::SolidColor>(albedo), 0, metalic);
                    world.Add(std::make_shared<rcl::Sphere>(center, 0.2, sphereMaterial));
                }
                else
                {
                    rcl::vec3 albedo = rcl::vec3::RandomVector(0.9, 1);
                    double refractorFactor = rcl::RandomDoubleMinMax(1, 2);
                    sphereMaterial = std::make_shared<rcl::Dielectric>(std::make_shared<rcl::SolidColor>(albedo), refractorFactor);
                    world.Add(std::make_shared<rcl::Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    
    auto material1 = std::make_shared<rcl::Dielectric>(std::make_shared<rcl::SolidColor>(rcl::vec3(1)), 1.5);
    world.Add(std::make_shared<rcl::Sphere>(rcl::vec3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<rcl::Lambertian>(std::make_shared<rcl::SolidColor>(rcl::vec3(0.4, 0.2, 0.1)));
    world.Add(std::make_shared<rcl::Sphere>(rcl::vec3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<rcl::Metal>(std::make_shared<rcl::SolidColor>(rcl::vec3(0.7, 0.6, 0.5)), 0.0, 0.0);
    world.Add(std::make_shared<rcl::Sphere>(rcl::vec3(4, 1, 0), 1.0, material3));

    auto material4 = std::make_shared<rcl::Light>(rcl::vec3(0, 1, 0), 4);
    world.Add(std::make_shared<rcl::Sphere>(rcl::vec3(0, 3, 0), 1.0, material4));

    world = rcl::HittableList(std::make_shared<rcl::BVHNode>(world));

    rcl::Camera cam;

    //cam.imageWidth = 1366;
    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 800;
    
    cam.lookFrom = rcl::vec3(13,2,3);
    cam.lookAt   = rcl::vec3(0,0,0);
    cam.up       = rcl::vec3(0,1,0);
    
    cam.vfov = 20;

    cam.defocusAngle = 0;
    cam.focusDistance = 10;
    
    rcl::Picture target;
    
    rcl::PathTracer vcm(50, 50);

    vcm.Render(world, cam, target, lightList);

    target.GammaCorection();
    target.Export("PathCornelBox.png");
}

void CornelBox()
{    
    rcl::HittableList world;
    rcl::HittableList lightList;
    auto red = std::make_shared<rcl::Lambertian>(std::make_shared<rcl::SolidColor>(rcl::vec3(.65, .05, .05)));
    auto white = std::make_shared<rcl::Lambertian>(std::make_shared<rcl::SolidColor>(rcl::vec3(.73, .73, .73)));
    auto green = std::make_shared<rcl::Lambertian>(std::make_shared<rcl::SolidColor>(rcl::vec3(.12, .45, .15)));
    auto light = std::make_shared<rcl::Light>(rcl::vec3(1), 1); // Increased intensity

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
    rcl::PathTracer vcm(50, 50);

    vcm.Render(world, cam, target, lightList);

    target.GammaCorection();
    target.Export("PathCornelBox.png");
}

int main()
{
    Spheres();
    //CornelBox();

    return 0;
}