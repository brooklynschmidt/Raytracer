#ifndef CAMERA_H
#define CAMERA_H

#include <raytracer.h>
#include <hittable_list.h>
#include <rayutils.h>

using namespace std;

class Camera {
    public:
        void render(const hittable& world);

        inline void setAspectRatio(const double ratio) {
            this->aspect_ratio = ratio;
        }

        inline void setImageWidth(const int width) {
            this->image_width = width;
        }

        inline void setSampleCount(const int count) {
            this->samples_per_pixel = count;
        }

    private:
        
        // Fields
        double aspect_ratio;
        int image_width;
        int image_height;
        Point camera_center;
        Point viewport_origin; // Pixel (0, 0) location 
        Vec3 pixel_delta_u;
        Vec3 pixel_delta_v;
        int samples_per_pixel;
        double pixel_samples_scale; // Color scale factor for a sum of pixel samples

        void initialize();
        Vec3 sample_square() const;

        // Construct Camera Ray starting from origin, directed at randomly sampled point around pixel (i, j)
        Ray create_camera_ray(int i, int j);
};

#endif