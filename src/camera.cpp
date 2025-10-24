#include <camera.h>
#include <mathutils.h>

Vec3 Camera::sample_square() const {
    return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

Point Camera::defocus_disk_sample() {
    auto p = random_in_unit_disk();
    return this->camera_center + (p.x() * defocus_disk_u) + (p.y() * defocus_disk_v);
}

Ray Camera::create_camera_ray(int i, int j) {
    // Construct a camera ray originating from the defocus disk, directed at a randomly
    // sampled point around the pixel location i, j.
    auto offset = sample_square();
    auto pixel_sample = this->viewport_origin + ((i + offset.x()) * this->pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
    auto ray_origin = (this->defocus_angle <= 0) ? this->camera_center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
}

void Camera::initialize() {
    // Image Properties
    this->image_height = int(this->image_width / this->aspect_ratio);
    this->image_height = (this->image_height < 1) ? 1 : this->image_height;

    this->pixel_samples_scale = 1.0 / this->samples_per_pixel;

    // Camera
    this->camera_center = this->lookFrom;
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / 2);

    // Calculate u, v, w unit basis vectors for camera coordinate frame.
    w = unit(this->lookFrom - this->lookAt);
    u = unit(cross(vup, w));
    v = cross(w, u);

    // Viewport
    double viewport_height = 2.0 * h * this->focus_dist;
    double viewport_width = viewport_height * (double(this->image_width) / this->image_height);
    Vec3 viewport_u = viewport_width * u; // Vector across viewport horizontal edge
    Vec3 viewport_v = viewport_height * -v; // Vector down 

    this->pixel_delta_u = viewport_u / this->image_width;
    this->pixel_delta_v = viewport_v / this->image_height;
            
    Point viewport_upper_left = this->camera_center - (this->focus_dist * w) - viewport_u / 2 - viewport_v / 2;
    this->viewport_origin = viewport_upper_left + 0.5 * (this->pixel_delta_u + this->pixel_delta_v);

    auto defocus_radius = this->focus_dist * std::tan(degrees_to_radians(this->defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;

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
                pixel_color += ray_color(r, max_ray_count, world);
            }
            write_color(std::cout, this->pixel_samples_scale * pixel_color);
        }
    }
    clog << "\rDone. \n";
}