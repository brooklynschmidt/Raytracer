#include <camera.h>
#include <mathutils.h>

Vec3 Camera::sample_square() const {
    return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

Ray Camera::create_camera_ray(int i, int j) {
    auto offset = sample_square();
    auto pixel_sample = this->viewport_origin + ((i + offset.x()) * this->pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
    auto ray_origin = this->camera_center;
    auto ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
}

void Camera::initialize() {
    // Image Properties
    this->image_height = int(this->image_width / this->aspect_ratio);
    this->image_height = (this->image_height < 1) ? 1 : this->image_height;

    this->pixel_samples_scale = 1.0 / this->samples_per_pixel;

    // Camera
    this->camera_center = Point(0, 0, 0);
    double focal_length = 1.0;

    // Viewport
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double(this->image_width) / this->image_height);
    Vec3 viewport_u = Vec3(viewport_width, 0, 0);
    Vec3 viewport_v = Vec3(0, -viewport_height, 0);

    this->pixel_delta_u = viewport_u / this->image_width;
    this->pixel_delta_v = viewport_v / this->image_height;
            
    Point viewport_upper_left = this->camera_center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    this->viewport_origin = viewport_upper_left + 0.5 * (this->pixel_delta_u + this->pixel_delta_v);
}

void Camera::render(const hittable& world) {
    initialize();

    // Rendering
    cout << "P3\n";
    cout << this->image_width << ' ' << this->image_height << "\n";
    cout << "255\n";
    
    for (int j = 0; j < this->image_height; j++) {
        for (int i = 0; i < this->image_width; i++) {
            Color pixel_color(0,0,0);
            for (int sample = 0; sample < this->samples_per_pixel; sample++) {
                Ray r = create_camera_ray(i, j);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, this->pixel_samples_scale * pixel_color);
        }
    }
    clog << "\rDone. \n";
}