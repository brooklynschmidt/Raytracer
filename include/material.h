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

class Dielectric : public Material {
    public:
        Dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override {
            // Always 1, glass surfaces absorb nothing
            attenuation = Color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

            Vec3 unit_direction = unit(r_in.direction());
            double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            Vec3 direction; 

            if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, ri);
            }

            scattered = Ray(rec.p, direction);
            return true;
        }
    private:
        // Refractive index in vacuum or air, or the ratio of the material's refractive index over
        // the refractive index of the enclosing media
        double refraction_index;

        static double reflectance(double cosine, double refraction_index) {
            // Schlick's approximation for reflectance
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow((1- cosine), 5);
        }
};

#endif