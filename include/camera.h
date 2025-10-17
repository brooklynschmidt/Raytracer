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

        inline void setMaxRays(const int max) {
            this->max_ray_count = max;
        }

        inline void setCameraVFov(const double fov) {
            this->vfov = fov;
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
        double vfov = 90; // Vertical View Angle (FOV)

        // This variable prevents stack blowout from recursive calls in the ray_color function. 
        int max_ray_count; // Max number of rays that bounce in the scene

        void initialize();
        Vec3 sample_square() const;

        // Construct Camera Ray starting from origin, directed at randomly sampled point around pixel (i, j)
        Ray create_camera_ray(int i, int j);
};

#endif