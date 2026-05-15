#ifndef TEXTURE_H
#define TEXTURE_H

#include <raytracer.h>

class Texture {
    public:
        virtual ~Texture() = default;

        virtual Color value(double u, double v, const Point& p) const = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor(const Color& albedo) : albedo(albedo) {}
        SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}
        Color value(double u, double v, const Point& p) const override {
            return albedo;
        }
    private:
        Color albedo;
    
};

class CheckerTexture : public Texture {
    public:
        CheckerTexture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd) :
            inv_scale(1.0 / scale), even(even), odd(odd) {}

        CheckerTexture(double scale, const Color& c1, const Color& c2) :
            CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}
        
        Color value(double u, double v, const Point& p) const override {
            auto xInteger = int(std::floor(inv_scale * p.x()));
            auto yInteger = int(std::floor(inv_scale * p.y()));
            auto zInteger = int(std::floor(inv_scale * p.z()));

            bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }
    private:
        double inv_scale;
        shared_ptr<Texture> even;
        shared_ptr<Texture> odd;
};



#endif