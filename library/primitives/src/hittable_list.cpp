#include "hittable_list.hpp"

#include <iostream>

namespace rcl
{

rcl::HittableList::HittableList(std::shared_ptr<rcl::Hittable> object)
{ 
    Add(object); 
}

rcl::HittableList::~HittableList()
{
    Clear();
}

void rcl::HittableList::Clear() 
{ 
    objects.clear(); 
}

void rcl::HittableList::Add(std::shared_ptr<Hittable> object) 
{
    objects.push_back(object);
    bbox = rcl::AABB(bbox, object->BoundingBox());
}

bool HittableList::hit(const Ray& r, const Interval<double>& interval, HitRecord& rec) const 
{
    HitRecord temp_rec;
    temp_rec.distance = rcl::infinity;
    bool hit_anything = false;
    double closest_so_far = interval.max;
    std::shared_ptr<rcl::Hittable> hitObject = nullptr;

    for (const std::shared_ptr<rcl::Hittable>& object : objects) 
    {
        if (object->hit(r, rcl::Interval<double>(interval.min, closest_so_far), temp_rec)) 
        {
            hit_anything = true;
            closest_so_far = temp_rec.distance;
            rec = temp_rec;
        }
    };

    return hit_anything;
}

const AABB& HittableList::BoundingBox() const
{
    return bbox;
}
    
vec3 HittableList::RandomPointOnSurface() const
{
    return objects[RandomIntMinMax(0, objects.size())]->RandomPointOnSurface();
}
    
Ray HittableList::RandomRayFromSurface() const
{
    return objects[RandomIntMinMax(0, objects.size())]->RandomRayFromSurface();
}
    
std::shared_ptr<Material> HittableList::GetMaterial() const
{
    return nullptr;
}

}//namespace rcl