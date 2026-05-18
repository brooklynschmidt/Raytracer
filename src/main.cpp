#include <raytracer.h>
#include <sphere.h>
#include <hittable_list.h>
#include <rayutils.h>
#include <camera.h>
#include <material.h>
#include "bvh.h"

#define WIDTH 400
#define SAMPLES 10

using namespace std;

void bouncing_spheres() {

    // World
    hittable_list world;

    auto checker = make_shared<CheckerTexture>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Point(0, -1000, 0), 1000, make_shared<Lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point center = Point(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point(4, 0.2, 0)).magnitude() > 0.9) {
                shared_ptr<Material> sphere_material;
                
                if (choose_mat < 0.8) {
                    // Diffuse
                    auto albedo = random_color() * random_color();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // Metal
                    auto albedo = random_color(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // Glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<BVH_Node>(world));

    // Camera
    Camera cam; 
    double ratio = 16.0 / 9.0;
    cam.setAspectRatio(ratio);
    cam.setImageWidth(WIDTH);
    cam.setSampleCount(SAMPLES);
    cam.setMaxRays(50);
    cam.setCameraVFov(20);
    cam.setLookFrom(Point(13, 2, 3));
    cam.setLookAt(Point(0, 0, 0));
    cam.setVup(Vec3(0, 1, 0));
    cam.setDefocusAngle(0.6);
    cam.setFocusDist(10.0);
    cam.render(world);

}

void checkered_spheres() {
    hittable_list world;

    auto checker = make_shared<CheckerTexture>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    world.add(make_shared<Sphere>(Point(0, -10, 0), 10, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(Point(0, 10, 0), 10, make_shared<Lambertian>(checker)));

    Camera cam;
    double ratio = 16.0 / 9.0;
    cam.setAspectRatio(ratio);
    cam.setImageWidth(WIDTH);
    cam.setSampleCount(10);
    cam.setMaxRays(50);
    cam.setCameraVFov(20);
    cam.setLookFrom(Point(13, 2, 3));
    cam.setLookAt(Point(0, 0, 0));
    cam.setVup(Vec3(0, 1, 0));
    cam.setDefocusAngle(0.6);
    cam.setFocusDist(10.0);

    cam.render(world);
}

void earth() {
    auto earth_texture = make_shared<ImageTexture>("earthmap.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto globe = make_shared<Sphere>(Point(0, 0, 0), 2, earth_surface);

    Camera cam;
    double ratio = 16.0 / 9.0;
    cam.setAspectRatio(ratio);
    cam.setImageWidth(WIDTH);
    cam.setSampleCount(10);
    cam.setMaxRays(50);
    cam.setCameraVFov(20);
    cam.setLookFrom(Point(0, 0, 12));
    cam.setLookAt(Point(0, 0, 0));
    cam.setVup(Vec3(0, 1, 0));
    cam.setDefocusAngle(0.6);
    cam.setFocusDist(10.0);
    cam.render(hittable_list(globe));
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cout << "Proper usage: ./main [int] > [file_name].ppm" << endl;
        exit(1);
    }
    switch (argv[1][0]) {
        case '1': bouncing_spheres(); break;
        case '2': checkered_spheres(); break;
        case '3': earth(); break;
        default: std::cout << "Couldn't render image, wrong input?" << endl;
    }
}