#ifndef RCL_MATERIALS
#define RCL_MATERIALS

#include "material.hpp"
#include "vector.hpp"
#include "texture.hpp"

namespace rcl
{

vec3 Lerp(const vec3& a, const vec3& b, double t);

class Lambertian : public rcl::Material
{
public:
    Lambertian(const std::shared_ptr<rcl::Texture>& texture);
    
    bool Scatter
    (const rcl::Ray& in, const rcl::HitRecord& rec, rcl::ScatterRecord& scatterRec) 
    const override;
    
    rcl::vec3 BRDF
    (const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered)
    const override;

private:
    std::shared_ptr<rcl::Texture> albedo;
};

class Metal : public rcl::Material
{
public:
    Metal(const std::shared_ptr<rcl::Texture>& texture, double roughness, double metallic);
    
    bool Scatter
    (const rcl::Ray& in, const rcl::HitRecord& rec, rcl::ScatterRecord& scatterRec) 
    const override;
    
    rcl::vec3 BRDF
    (const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered)
    const override;

    bool IsSpecular() const override;
    bool IsReflective() const override;

private:
    std::shared_ptr<rcl::Texture> albedo;
    double metallic;
    double roughness;

    double D(const rcl::vec3& normal, const rcl::vec3& h, double alpha) const;
    double G1(const rcl::vec3& dir, const rcl::vec3& normal, double alpha) const;
    double G(const rcl::vec3& inDir, const rcl::vec3& outDir, const rcl::vec3& normal, double alpha) const;
    rcl::vec3 F(const rcl::vec3& F0, const rcl::vec3& inDir, const rcl::vec3& h) const;
};

class Dielectric : public rcl::Material
{
public:
    Dielectric(const std::shared_ptr<rcl::Texture>& texture, const double refractionFactor);
    
    bool Scatter
    (const rcl::Ray& in, const rcl::HitRecord& rec, rcl::ScatterRecord& scatterRec) 
    const override;
    
    rcl::vec3 BRDF
    (const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered)
    const override;

    bool IsSpecular() const override;
    bool IsRefractive() const override;

private:
    double refractionFactor;
    std::shared_ptr<rcl::Texture> albedo;
    double Reflectance(double cosine, double refraction_index) const;
};

class Light : public rcl::Material
{
public:
    Light(const vec3& col, const double intensity);
    
    virtual rcl::vec3 Emitted(const rcl::HitRecord& rec) const override;
    virtual rcl::vec3 IntenseEmitted(const rcl::HitRecord& rec) const override;

private:
    vec3 albedo;
    double intensity;
};

}
#endif