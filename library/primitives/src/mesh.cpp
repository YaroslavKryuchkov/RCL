#include "mesh.hpp"

#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>

#include "bvh.hpp"
#include "model_workers.hpp"

rcl::Mesh::Mesh(const char* path, std::shared_ptr<rcl::Material> mat) : mat(mat)
{
    Import(path);
}

void rcl::Mesh::Import(const char* path)
{
    triangles.Clear();

    std::string ext = strrchr(path, '.');
    if (!ext.size())
    {
        std::cerr << "Error: No file extension found in " << path << std::endl;
        return;
    }
    
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (strcmp(ext.c_str(), ".obj") == 0)
    {
        ImportOBJ(path, triangles);
    }
    else
    {
        std::cerr << "Error: Unsupported file format " << ext << std::endl;
    }
	
    triangles = rcl::HittableList(std::make_shared<rcl::BVHNode>(triangles));
}

bool rcl::Mesh::hit(const rcl::Ray& ray, const rcl::Interval<double>& interval, rcl::HitRecord& record) const
{
    if(triangles.hit(ray, interval, record))
    {
        record.mat = mat;
        record.object = this;
        return true;
    }
    return false;
}

const rcl::AABB& rcl::Mesh::BoundingBox() const
{
    return triangles.BoundingBox();
}

rcl::vec3 rcl::Mesh::RandomPointOnSurface() const
{
    return triangles.objects[rcl::RandomIntMinMax(0, triangles.objects.size())]->RandomPointOnSurface();
}
    
rcl::Ray rcl::Mesh::RandomRayFromSurface() const
{
    return triangles.objects[rcl::RandomIntMinMax(0, triangles.objects.size())]->RandomRayFromSurface(); 
}

std::shared_ptr<rcl::Material> rcl::Mesh::GetMaterial() const
{
    return mat;
}