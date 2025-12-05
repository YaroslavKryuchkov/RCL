#include "materials.hpp"

#include <iostream>

#include "vector.hpp"
#include "onb.hpp"
#include "pdf.hpp"
#include "pdfs.hpp"

namespace rcl
{

Lambertian::Lambertian(const std::shared_ptr<rcl::Texture>& c) : albedo(c) {}
    
bool Lambertian::Scatter
(const rcl::Ray& in, const rcl::HitRecord& rec, rcl::ScatterRecord& scatterRec)
const 
{
    auto PDF = std::make_shared<CosinePDF>(rec.normal);
    scatterRec.skipBRDF = false;
    scatterRec.albedo = albedo->GetColor(rec.uv);
    scatterRec.outVec = PDF->Generate(in, rec);
    scatterRec.probability = PDF->Probability(in, rec, rcl::Ray(rec.point, scatterRec.outVec));
    return true;
}
    
rcl::vec3 Lambertian::BRDF
(const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered)
const 
{
    return albedo->GetColor(rec.uv) / rcl::PI;
}

Metal::Metal(const std::shared_ptr<rcl::Texture>& c, double roughness, double metallic) 
: albedo(c), 
  roughness(roughness < 1 ? roughness : 1), 
  metallic(metallic < 1 ? metallic : 1)
{}
    
bool Metal::Scatter
(const rcl::Ray& in, const rcl::HitRecord& rec, rcl::ScatterRecord& scatterRec)
const
{
    if(roughness <= 1e-8)
    {
        // Perfect mirror reflection for perfectly smooth surfaces
        auto PDF = std::make_shared<ReflectPDF>();
        scatterRec.skipBRDF = true;
        scatterRec.outVec = PDF->Generate(in, rec);
        scatterRec.probability = PDF->Probability(in, rec, rcl::Ray(rec.point, scatterRec.outVec));
    }
    else
    {
        // Use GGX distribution for rough surfaces
        auto PDF = std::make_shared<GGXPDF>(rec.normal, roughness);
        scatterRec.skipBRDF = false;
        scatterRec.outVec = PDF->Generate(in, rec);
        scatterRec.probability = PDF->Probability(in, rec, rcl::Ray(rec.point, scatterRec.outVec));
    }
    scatterRec.albedo = albedo->GetColor(rec.uv);
    return true;
}

// GGX Distribution term - Fixed normalization
double Metal::D(const rcl::vec3& normal, const rcl::vec3& h, double alpha) const
{
    double alpha2 = alpha * alpha;
    double NdH = rcl::Dot(normal, h);
   
    // Safety check to prevent numerical issues
    if (NdH <= 0.0) return 0.0;
   
    double NdH2 = NdH * NdH;
    double denom = NdH2 * (alpha2 - 1.0) + 1.0;
    denom = denom * denom;
    
    // Proper normalization factor
    return alpha2 / (rcl::PI * denom);
}

// Smith geometric shadowing term (single direction) - Fixed masking function
double Metal::G1(const rcl::vec3& dir, const rcl::vec3& normal, double alpha) const
{
    double NdV = rcl::Dot(normal, dir);
    if(NdV <= 0.0) return 0.0; // No masking for back-facing directions
   
    double alpha2 = alpha * alpha;
    double NdV2 = NdV * NdV;
    double tan2Theta = (1.0 - NdV2) / NdV2;
   
    if (tan2Theta <= 0.0) return 1.0; // Perfect alignment
   
    double lambda = (-1.0 + std::sqrt(1.0 + alpha2 * tan2Theta)) * 0.5;
    return 1.0 / (1.0 + lambda);
}

// Smith geometric shadowing term (combined) - Height-correlated masking-shadowing
double Metal::G(const rcl::vec3& inDir, const rcl::vec3& outDir, const rcl::vec3& normal, double alpha) const
{
    double NoV = rcl::Dot(normal, -inDir); // View direction (towards surface)
    double NoL = rcl::Dot(normal, outDir);  // Light direction
    
    if (NoV <= 0.0 || NoL <= 0.0) return 0.0;
    
    double alpha2 = alpha * alpha;
    
    // Lambda for view direction
    double NoV2 = NoV * NoV;
    double tan2ThetaV = (1.0 - NoV2) / NoV2;
    double lambdaV = (-1.0 + std::sqrt(1.0 + alpha2 * tan2ThetaV)) * 0.5;
    
    // Lambda for light direction
    double NoL2 = NoL * NoL;
    double tan2ThetaL = (1.0 - NoL2) / NoL2;
    double lambdaL = (-1.0 + std::sqrt(1.0 + alpha2 * tan2ThetaL)) * 0.5;
    
    // Height-correlated masking-shadowing
    return 1.0 / (1.0 + lambdaV + lambdaL);
}

// Schlick's approximation for Fresnel - Fixed for metals
rcl::vec3 Metal::F(const rcl::vec3& F0, const rcl::vec3& inDir, const rcl::vec3& h) const
{
    double VdH = std::fmax(0.0, rcl::Dot(-inDir, h)); // View-half angle
   
    rcl::vec3 dielectricF0(0.04, 0.04, 0.04);
    rcl::vec3 actualF0 = rcl::Lerp(dielectricF0, F0, metallic);
   
    double fresnel = std::pow(std::fmax(0.0, 1.0 - VdH), 5.0);
    return actualF0 + (rcl::vec3(1.0) - actualF0) * fresnel;
}
   
rcl::vec3 Metal::BRDF
(const rcl::Ray& in, const rcl::HitRecord& rec, const rcl::Ray& scattered)
const
{
    rcl::vec3 v = (-in.direction).Unit(); // View direction (towards surface)
    rcl::vec3 l = scattered.direction.Unit(); // Light direction
    rcl::vec3 n = rec.normal.Unit(); // Surface normal
   
    double NoV = rcl::Dot(n, v);
    double NoL = rcl::Dot(n, l);
   
    // Early exit for directions below the surface
    if (NoV <= 0.0 || NoL <= 0.0)
    {
        return rcl::vec3(0.0);
    }
    
    // Half vector
    rcl::vec3 h = (v + l).Unit();
    double NoH = rcl::Dot(n, h);
    
    if (NoH <= 0.0) return rcl::vec3(0.0);
    
    double alpha = roughness * roughness;
    rcl::vec3 baseColor = albedo->GetColor(rec.uv);
   
    // Compute components of the Cook-Torrance specular BRDF
    rcl::vec3 F_term = F(baseColor, v, h);
    double D_term = D(n, h, alpha);
    double G_term = G(v, l, n, alpha);
   
    // Specular component (Cook-Torrance microfacet model)
    double denominator = 4.0 * NoV * NoL;
    rcl::vec3 specular = rcl::vec3(0.0);
    if (denominator > 1e-8) {
        specular = F_term * G_term * D_term / denominator;
    }
   
    // Diffuse component (energy conservation)
    // kS is Fresnel, kD is what's left over
    rcl::vec3 kS = F_term;
    rcl::vec3 kD = rcl::vec3(1.0) - kS;
    
    // Metallic surfaces have no diffuse reflection
    kD = kD * (1.0 - metallic);
    
    rcl::vec3 diffuse = kD * baseColor / rcl::PI;
   
    return specular + diffuse;
}

bool Metal::IsSpecular() const
{
    // Consider metals with very low roughness as specular
    return true;
}
    
bool Metal::IsReflective() const 
{ 
    return true; 
}

Dielectric::Dielectric(const std::shared_ptr<rcl::Texture>& c, const double refractionFactor) 
: albedo(c), refractionFactor(refractionFactor) {}
    
bool Dielectric::Scatter
(const rcl::Ray& in, const rcl::HitRecord& rec, rcl::ScatterRecord& scatterRec)
const 
{
    auto PDF = std::make_shared<GlassPDF>(refractionFactor);
    scatterRec.outVec = PDF->Generate(in, rec);
    scatterRec.probability = PDF->Probability(in, rec, rcl::Ray(rec.point, scatterRec.outVec));
    
    // Calculate Fresnel reflectance to attenuate albedo at grazing angles
    double cosTheta = std::fmin(rcl::Dot(-in.direction, rec.normal), 1.0);
    double reflectFactor = Reflectance(std::fabs(cosTheta), refractionFactor);
    
    // For glass materials, we don't want to modulate the albedo in the same way as metals
    // Just pass the albedo directly
    scatterRec.albedo = albedo->GetColor(rec.uv);
    
    scatterRec.skipBRDF = true;
    return true;
}
    
double Dielectric::Reflectance(double cosine, double refraction_index) const
{
    // Schlick's approximation for Fresnel reflectance
    double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    double r02 = r0 * r0;
    
    // Improved version with more physically correct behavior at grazing angles
    double x = 1.0 - cosine;
    return r02 + (1.0 - r02) * x * x * x * x * x;
}

rcl::vec3 Dielectric::BRDF
(const rcl::Ray& ray_in, const rcl::HitRecord& rec, const rcl::Ray& scattered) 
const
{
    // Glass is actually not well-represented by a traditional BRDF
    // Since it's perfectly specular, its BRDF is a delta function
    
    vec3 unit_direction = ray_in.direction.Unit();
    double cos_theta = fmin(rcl::Dot(-unit_direction, rec.normal), 1.0);
    
    double etai_over_etat = rec.frontFace ? (1.0 / refractionFactor) : refractionFactor;
    
    // Reflection probability using Schlick's approximation
    double fresnel = Reflectance(cos_theta, etai_over_etat);
    
    // For photon gathering, we need to return proper values
    // The important distinction is that glass doesn't have colored reflections like metal
    // It should have white/neutral reflections modulated by albedo for transmission
    
    // Check if this is a reflection or refraction
    bool isReflection = rcl::Dot(ray_in.direction + scattered.direction, rec.normal) < 0;
    
    if (isReflection) {
        // For reflections, use a more neutral (less colored) specular highlight
        return vec3(fresnel); // White specular reflection
    } else {
        // For refraction, use the albedo color
        return albedo->GetColor(rec.uv) * (1.0 - fresnel);
    }
}

bool Dielectric::IsSpecular() const
{
    return true;  // Glass is specular
}
    
bool Dielectric::IsRefractive() const
{ 
    return true; 
}

Light::Light(const vec3& col, const double intensity) : albedo(col), intensity(intensity) {}
    
rcl::vec3 Light::Emitted(const rcl::HitRecord& rec) const
{
    return albedo;
}

rcl::vec3 Light::IntenseEmitted(const rcl::HitRecord& rec) const
{
    return albedo * intensity;
}

// Helper function for linear interpolation between vectors
vec3 Lerp(const vec3& a, const vec3& b, double t)
{
    return a * (1.0 - t) + b * t;
}

}