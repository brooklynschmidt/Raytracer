#include <raytracer.h>
#include <sphere.h>
#include <hittable_list.h>
#include <rayutils.h>
#include <camera.h>
#include <material.h>

#define WIDTH 400
#define SAMPLES 10

using namespace std;

int main() {

    // World
    hittable_list world;
    auto R = std::cos(pi / 4);

    auto material_left = make_shared<Lambertian>(Color(0,0,1));
    auto material_right = make_shared<Lambertian>(Color(1,0,0));

    world.add(make_shared<Sphere>(Point(-R, 0, -1), R, material_left));
    world.add(make_shared<Sphere>(Point(R, 0, -1), R, material_right));

    // Camera
    Camera cam; 
    double ratio = 16.0 / 9.0;
    cam.setAspectRatio(ratio);
    cam.setImageWidth(WIDTH);
    cam.setSampleCount(SAMPLES);
    cam.setMaxRays(50);
    cam.setCameraVFov(90);
    cam.render(world);

}