#include <camera.h>
#include <mathutils.h>

Vec3 Camera::sample_square() const {
    return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

Vec3 Camera::sample_square_stratified(int s_i, int s_j) const {
    // Returns the Vector to a random point in the square sub-pixel specified by grid
    // Indices s_i and s_j, for an idealized unit square pixel [-.5, -.5] to [+.5, +.5]
    auto px = ((s_i + random_double()) * recip_sqrt_spp) - 0.5;
    auto py = ((s_j + random_double()) * recip_sqrt_spp) - 0.5;

    return Vec3(px, py, 0);
}

Point Camera::defocus_disk_sample() {
    auto p = random_in_unit_disk();
    return this->camera_center + (p.x() * defocus_disk_u) + (p.y() * defocus_disk_v);
}

Ray Camera::create_camera_ray(int i, int j, int s_i, int s_j) {
    // Construct a camera ray originating from the defocus disk, directed at a randomly
    // sampled point around the pixel location i, j.
    auto offset = sample_square_stratified(s_i, s_j);
    auto pixel_sample = this->viewport_origin + ((i + offset.x()) * this->pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
    auto ray_origin = (this->defocus_angle <= 0) ? this->camera_center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;
    auto ray_time = random_double();

    // Generate rays at a random instant between 0 and 1 frames. 
    return Ray(ray_origin, ray_direction, ray_time);
}

void Camera::initialize() {
    // Image Properties
    this->image_height = int(this->image_width / this->aspect_ratio);
    this->image_height = (this->image_height < 1) ? 1 : this->image_height;

    this->sqrt_spp = int(std::sqrt(samples_per_pixel));
    this->pixel_samples_scale = 1.0 / (this->sqrt_spp * this->sqrt_spp);
    this->recip_sqrt_spp = 1.0 / this->sqrt_spp;

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
            for (int s_j = 0; s_j < this->sqrt_spp; s_j++) {
                for (int s_i = 0; s_i < this->sqrt_spp; s_i++) {
                    Ray r = create_camera_ray(i, j, s_i, s_j);
                    pixel_color += ray_color(r, max_ray_count, world, this->background);
                }
            }
            write_color(std::cout, this->pixel_samples_scale * pixel_color);
        }
    }
    clog << "\rDone. \n";
}