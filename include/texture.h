#ifndef TEXTURE_H
#define TEXTURE_H

#include <raytracer.h>
#include <rtw_stb_image.h>

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

class ImageTexture : public Texture {
    public: 
        ImageTexture(const char* filename) : image(filename) {}
        Color value(double u, double v, const Point& p) const override {
            // Return cyan for debugging aid if no texture data
            if (image.height() <= 0) return Color(0, 1, 1);

            // Clamp input texture coordinates to [0, 1] x [1, 0]
            u = Interval(0, 1).clamp(u);
            v = 1.0 - Interval(0, 1).clamp(v); // Flip V to image coordinates

            auto i = int(u * image.width());
            auto j = int(v * image.height());
            auto pixel = image.pixel_data(i, j);

            auto color_scale = 1.0 / 255.0;
            return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        }

    private:
        RTW_Image image;    
};



#endif