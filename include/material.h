#ifndef MATERIAL_H
#define MATERIAL_H

#include <hittable.h>


// Abstract Material Class to encapsulate unique behaviors and materials
class Material {
    public:
        virtual ~Material() = default;
        
        // We choose hit_record so we can encapsulate arguments and send them as a group
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const {
            return false;
        }    
};

// We choose to always scatter and attenuate light according to its reflectance
class Lambertian : public Material {
    public: 
        Lambertian(const Color& albedo) : albedo(albedo) {}

        bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override {
            auto scatterDirection = rec.normal + random_unit_vector();
            // Accounts for case where random unit vector is the exact opposite of the normal
            if (near_zero(scatterDirection)) {
                scatterDirection = rec.normal;
            }
            scattered = Ray(rec.p, scatterDirection);
            attenuation = albedo;
            return true;
        }

    private:
        Color albedo;
};

class Metal : public Material {
    public:
        Metal(const Color& albedo) : albedo(albedo) {}

        bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override {
            Vec3 reflected = reflect(r_in.direction(), rec.normal);
            scattered = Ray(rec.p, reflected);
            attenuation = albedo;
            return true;
        }
    private:
        Color albedo;
};

#endif