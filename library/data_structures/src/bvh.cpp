#include "bvh.hpp"

#include <algorithm>
#include <memory>

rcl::BVHNode::BVHNode(rcl::HittableList list) : BVHNode(list.objects, 0, list.objects.size()) {}
rcl::BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end)
{
    bbox = rcl::AABB::empty;
    for(size_t objIndex = start; objIndex < end; objIndex++)
        bbox = rcl::AABB(bbox, objects[objIndex]->BoundingBox());

    int axis = bbox.LongestAxis();

    auto comparator = (axis == 0) ? BoxXCompare 
                    : (axis == 1) ? BoxYCompare
                                  : BoxZCompare;
    
    size_t objectSpan = end - start;

    if(objectSpan == 1)
        left = right = objects[start];
    else if(objectSpan == 2)
    {
        left = objects[start];
        right = objects[start + 1];
    }
    else
    {
        std::stable_sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

        size_t mid = start + objectSpan/2;
        left = std::make_shared<BVHNode>(objects, start, mid);
        right = std::make_shared<BVHNode>(objects, mid, end);
    }

    bbox = rcl::AABB(left->BoundingBox(), right->BoundingBox());
}

bool rcl::BVHNode::hit
(const rcl::Ray& ray, const rcl::Interval<double>& interval, rcl::HitRecord& record) 
const
{
    if(!bbox.Hit(ray, interval))
        return false;

    bool hitLeft = left->hit(ray, interval, record);
    
    bool hitRight = right->hit
    (ray, rcl::Interval<double>(interval.min, hitLeft ? record.distance : interval.max), record);

    return hitLeft || hitRight;
}

const rcl::AABB& rcl::BVHNode::BoundingBox() const
{
    return bbox;
}
    
rcl::vec3 rcl::BVHNode::RandomPointOnSurface() const
{
    if (rcl::RandomDouble01() < 0.5)
    {
        auto leftNode = std::dynamic_pointer_cast<BVHNode>(left);
        if (leftNode)
            return leftNode->RandomPointOnSurface();
        else
            return left->RandomPointOnSurface();
    }
    else
    {
        auto rightNode = std::dynamic_pointer_cast<BVHNode>(right);
        if (rightNode)
            return rightNode->RandomPointOnSurface();
        else
            return right->RandomPointOnSurface();
    }
}

    
rcl::Ray rcl::BVHNode::RandomRayFromSurface() const
{
    if (rcl::RandomDouble01() < 0.5)
    {
        auto leftNode = std::dynamic_pointer_cast<BVHNode>(left);
        if (leftNode)
            return leftNode->RandomRayFromSurface();
        else
            return left->RandomRayFromSurface();
    }
    else
    {
        auto rightNode = std::dynamic_pointer_cast<BVHNode>(right);
        if (rightNode)
            return rightNode->RandomRayFromSurface();
        else
            return right->RandomRayFromSurface();
    } 
}
    
std::shared_ptr<rcl::Material> rcl::BVHNode::GetMaterial() const
{
    return nullptr;
}

bool rcl::BVHNode::BoxCompare
(const std::shared_ptr<rcl::Hittable> a, const std::shared_ptr<rcl::Hittable> b, int axisIndex)
{
    auto aAxis = a->BoundingBox().AxisInterval(axisIndex);
    auto bAxis = b->BoundingBox().AxisInterval(axisIndex);
    return aAxis.min < bAxis.min;
}

bool rcl::BVHNode::BoxXCompare(const std::shared_ptr<rcl::Hittable> a, const std::shared_ptr<rcl::Hittable> b)
{
    return BoxCompare(a, b, 0);
}

bool rcl::BVHNode::BoxYCompare(const std::shared_ptr<rcl::Hittable> a, const std::shared_ptr<rcl::Hittable> b)
{
    return BoxCompare(a, b, 1);
}

bool rcl::BVHNode::BoxZCompare(const std::shared_ptr<rcl::Hittable> a, const std::shared_ptr<rcl::Hittable> b)
{
    return BoxCompare(a, b, 2);
}