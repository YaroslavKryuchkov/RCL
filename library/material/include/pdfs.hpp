#ifndef RCL_PDFS
#define RCL_PDFS

#include <iostream>

#include "vector.hpp"
#include "onb.hpp"
#include "ray.hpp"

namespace rcl
{

class CosinePDF : public PDF
{
public:
    CosinePDF(rcl::vec3 normal) : onb(rcl::ONB(normal)) {};

    rcl::vec3 Generate(const rcl::Ray& in, const rcl::HitRecord& rec) const override
    {
        return onb.Transform(rcl::Ray::RandomCosineDirection());
    }

    double Probability(const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered) const override
    {
        double cosTheta = rcl::Dot(scattered.direction, onb.w());
        return std::fmax(0.0, cosTheta / rcl::PI);
    }
private:
    rcl::ONB onb;
};

class ReflectPDF : public PDF
{
public:
    ReflectPDF(){};

    rcl::vec3 Generate(const rcl::Ray& in, const rcl::HitRecord& rec) const override
    {
        return rcl::Reflect(in.direction, rec.normal);
    }

    double Probability(const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered) const override
    {
        return 1.0;
    }
};

class GGXPDF : public PDF
{
public:
    GGXPDF(rcl::vec3 normal, double roughness) : onb(rcl::ONB(normal)), roughness(roughness) {};

    rcl::vec3 Generate(const rcl::Ray& in, const rcl::HitRecord& rec) const override
    {
        double u1 = rcl::RandomDouble01();
        double u2 = rcl::RandomDouble01();
        double alpha = roughness * roughness;

        double phi = 2.0f * rcl::PI * u1;
        double cosTheta = std::sqrt((1.0f - u2) / (1.0f + (alpha*alpha - 1.0f) * u2));
        double sinTheta = std::sqrt(1.0f - cosTheta*cosTheta);

        rcl::vec3 h_local = rcl::vec3(std::cos(phi) * sinTheta, std::sin(phi) * sinTheta, cosTheta).Unit();

        rcl::vec3 reflected = rcl::Reflect(in.direction, onb.Transform(h_local));
    
        if (rcl::Dot(reflected, rec.normal) <= 0)
            return Generate(in, rec);
        return reflected;
    }

    double Probability(const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered) const override
    {
        rcl::vec3 v = -in.direction;
        rcl::vec3 l = scattered.direction;

        if (rcl::Dot(rec.normal, v) <= 1e-8 || rcl::Dot(rec.normal, l) <= 1e-8)
            return 0.0;

        double alpha = roughness * roughness;
        rcl::vec3 h = (v + l).Unit();

        return D(rec.normal, h, alpha) * rcl::Dot(rec.normal, h) / (4 * rcl::Dot(-in.direction, h));
    }
private:
    rcl::ONB onb;
    double roughness;

    double D(rcl::vec3 normal, rcl::vec3 h, double alpha) const
    {
        double alpha2 = alpha * alpha;
        double NdH = rcl::Dot(normal, h);
        double denom = NdH * NdH * (alpha2 - 1.0f) + 1;
        return alpha2 / (rcl::PI * denom * denom);
    }
};

class GlassPDF : public PDF
{
public:
    GlassPDF(double refraction_index) 
    : refraction_index(refraction_index) 
    {};

    rcl::vec3 Generate(const rcl::Ray& in, const rcl::HitRecord& rec) const override
    {
        double ri = rec.frontFace ? (1.0 / refraction_index) : refraction_index;
        double cosTheta = std::fmin(rcl::Dot(-in.direction, rec.normal), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        bool cannotRefract = ri * sinTheta > 1.0;
        rcl::vec3 direction;

        if(cannotRefract || Reflectance(cosTheta, ri) > rcl::RandomDouble01())
            direction = rcl::Reflect(in.direction, rec.normal);
        else
            direction = rcl::Refract(in.direction, rec.normal, ri);

        return direction;
    }

    double Probability(const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered) const override
    {
        double ri = rec.frontFace ? (1.0 / refraction_index) : refraction_index;
        double cosTheta = std::fmin(rcl::Dot(-in.direction, rec.normal), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
        
        bool cannotRefract = ri * sinTheta > 1.0;

        double ref = Reflectance(cosTheta, ri);

        if(cannotRefract || rcl::Dot(scattered.direction, rec.normal) > 0)
            return ref;
        else
            return (1 - ref);
    }
private:
    double refraction_index;
    
    double Reflectance(double cosine, double refraction_index) const
    {
        double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
        double r02 = r0*r0;
        return r02 + (1 - r02) * std::pow((1 - cosine), 5);
    }
};

}
#endif