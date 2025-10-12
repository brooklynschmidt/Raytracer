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
        Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        /* Randomize the reflected direction using a small sphere and choosing a new endpoint for the ray
        We use a random point from the surface of a sphere centered on the original endpoint, scaled by 
        the fuzz factor. The bigger the fuzz sphere, the fuzzier the reflections. 
        Absorb grazing rays or big spheres that scatter below the surface.
        The fuzz sphere is scaled in relation to the reflection vector, thus a normalization is required. 
        */
        bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override {
            Vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit(reflected) + (fuzz * random_unit_vector());
            scattered = Ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    private:
        Color albedo;
        double fuzz;
};

#endif