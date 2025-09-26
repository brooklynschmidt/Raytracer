#include <raytracer.h>
#include <sphere.h>
#include <hittable_list.h>
#include <rayutils.h>
#include <camera.h>

#define WIDTH 400
#define SAMPLES 10

using namespace std;

int main() {

    // World
    hittable_list world;
    world.add(make_shared<Sphere>(Point(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point(0, -100.5, -1), 100));

    // Camera
    Camera cam; 
    double ratio = 16.0 / 9.0;
    cam.setAspectRatio(ratio);
    cam.setImageWidth(WIDTH);
    cam.setSampleCount(SAMPLES);
    cam.render(world);

}