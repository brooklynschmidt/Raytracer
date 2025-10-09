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
            auto scatter_direction = rec.normal + random_unit_vector();

            /* 
            Prevents the case where random_unit_vector is the exact opposite of rec.normal
            This would lead to a zero vector and NaN / infinity bugs
            */
            if (near_zero(scatter_direction)) {
                scatter_direction = rec.normal;
            }
            
            scattered = Ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        Color albedo;
};

#endif