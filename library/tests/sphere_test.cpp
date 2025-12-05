#include <iostream>

#include "sphere.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "hittable_list.hpp"

int main()
{
    auto A = std::make_shared<rcl::Sphere>(rcl::vec3(0, 0, -1), 1, nullptr);
    auto B = std::make_shared<rcl::Sphere>(rcl::vec3(0, 0, -5), 1, nullptr);
    auto C = std::make_shared<rcl::Sphere>(rcl::vec3(0, 3, -1), 1, nullptr);
    auto Ray = rcl::Ray(rcl::vec3(0, 0, 1), rcl::vec3(0, 0, -1));
    rcl::HitRecord rec;
    rcl::Interval<double> interval(0.001f, rcl::infinity);

    if(A->hit(Ray, interval, rec))
        std::cout << "Point of intersect with sphere A:(" << rec.point << ")" << std::endl;
    else
        std::cout << "Ray not intersect with sphere A." << std::endl;

    if(B->hit(Ray, interval, rec))
        std::cout << "Point of intersect with sphere B:(" << rec.point << ")" << std::endl;
    else
        std::cout << "Ray not intersect with sphere B." << std::endl;

    if(C->hit(Ray, interval, rec))
        std::cout << "Point of intersect with sphere C:(" << rec.point << ")" << std::endl;
    else
        std::cout << "Ray not intersect with sphere C." << std::endl;

    rcl::HittableList list;
    list.Add(A);
    list.Add(B);
    list.Add(C);

    if(list.hit(Ray, interval, rec))
        std::cout << "Nearest point of intersect with spheres:(" << rec.point << ")" << std::endl;
    else
        std::cout << "Ray not intersect with spheres." << std::endl;

    int F;
    std::cin >> F;
}