#include <iostream>
#include <stdio.h>
#include <typed_vec3.h>
#include <ray.h>
#include <rayutils.h>

using namespace std;

int main() {

    // Image Properties
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    Point camera_center = Point(0, 0, 0);
    double focal_length = 1.0;

    // Viewport 
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double(image_width) / image_height);
    Vec3 viewport_u = Vec3(viewport_width, 0, 0);
    Vec3 viewport_v = Vec3(0, -viewport_height, 0);

    Vec3 pixel_delta_u = viewport_u / image_width;
    Vec3 pixel_delta_v = viewport_v / image_height;

    Point viewport_upper_left = camera_center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    Point viewport_origin = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Rendering
    cout << "P3\n";
    cout << image_width << ' ' << image_height << "\n";
    cout << "255\n";
    
    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            Point pixel_center = viewport_origin + (i * pixel_delta_u) + (j * pixel_delta_v);
            Vec3 ray_direction = pixel_center - camera_center;
            Ray ray = Ray(pixel_center, ray_direction);
            Color pixel_color = ray_color(ray);
            write_color(std::cout, pixel_color);
        } 
    }
    clog << "\rDone. \n";
}