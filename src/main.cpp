#include <raytracer.h>
#include <sphere.h>
#include <hittable_list.h>
#include <rayutils.h>
#include <camera.h>
#include <material.h>
#include "bvh.h"
#include <quad.h>
#include <constant_medium.h>


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
    cam.setBackgroundColor(Color(0.7, 0.8,  1.0));
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
    cam.setBackgroundColor(Color(0.7, 0.8,  1.0));

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
    cam.setBackgroundColor(Color(0.7, 0.8,  1.0));

    cam.render(hittable_list(globe));
}

void perlin_spheres() {
    hittable_list world;

    auto pertext = make_shared<NoiseTexture>(8);
    world.add(make_shared<Sphere>(Point(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    world.add(make_shared<Sphere>(Point(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

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
    cam.setBackgroundColor(Color(0.7, 0.8,  1.0));


    cam.render(world);
}

void quads() {
    hittable_list world;

    // Materials
    auto red = make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto green = make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto blue = make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto orange = make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto teal = make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<Quad>(Point(-3, -2, 5), Vec3(0, 0, -4), Vec3(0, 4, 0), red));
    world.add(make_shared<Quad>(Point(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), green));
    world.add(make_shared<Quad>(Point(3, -2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), blue));
    world.add(make_shared<Quad>(Point(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), orange));
    world.add(make_shared<Quad>(Point(-2, -3, 5), Vec3(4, 0, 0), Vec3(0, 0, -4), teal));

    Camera cam;
    cam.setAspectRatio(1.0);
    cam.setImageWidth(WIDTH);
    cam.setSampleCount(10);
    cam.setMaxRays(50);
    cam.setCameraVFov(80);
    cam.setLookFrom(Point(0, 0, 9));
    cam.setLookAt(Point(0, 0, 0));
    cam.setVup(Vec3(0, 1, 0));
    cam.setDefocusAngle(0.6);
    cam.setFocusDist(10.0);
    cam.setBackgroundColor(Color(0.7, 0.8,  1.0));

    cam.render(world);

}

void simple_light() {
    hittable_list world;

    auto pertext = make_shared<NoiseTexture>(4);
    world.add(make_shared<Sphere>(Point(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    world.add(make_shared<Sphere>(Point(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    auto difflight = make_shared<DiffuseLight>(Color(4, 4, 4));
    world.add(make_shared<Sphere>(Point(0, 7, 0), 2, difflight));
    world.add(make_shared<Quad>(Point(3, 1, -2), Vec3(2, 0, 0), Vec3(0, 2, 0), difflight));

    Camera cam;
    auto ratio = 16.0 / 9.0;
    cam.setAspectRatio(ratio);
    cam.setImageWidth(WIDTH);
    cam.setSampleCount(10);
    cam.setMaxRays(50);
    cam.setCameraVFov(20);
    cam.setLookFrom(Point(26, 3, 6));
    cam.setLookAt(Point(0, 2, 0));
    cam.setVup(Vec3(0, 1, 0));
    cam.setDefocusAngle(0.6);
    cam.setFocusDist(10.0);
    cam.setBackgroundColor(Color(0, 0, 0));

    cam.render(world);
}

void cornell_box() {
    hittable_list world;

    auto red = make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto green = make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    world.add(make_shared<Quad>(Point(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.add(make_shared<Quad>(Point(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.add(make_shared<Quad>(Point(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
    world.add(make_shared<Quad>(Point(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
    world.add(make_shared<Quad>(Point(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    shared_ptr<hittable> box1 = box(Point(0, 0, 0), Point(165, 330, 165), white);
    box1 = make_shared<Rotate_Y>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(Point(0, 0, 0), Point(165, 165, 165), white);
    box2 = make_shared<Rotate_Y>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
    world.add(box2);

    Camera cam;
    cam.setAspectRatio(1.0);
    cam.setImageWidth(600);
    cam.setSampleCount(200);
    cam.setMaxRays(50);
    cam.setCameraVFov(40);
    cam.setLookFrom(Point(278, 278, -800));
    cam.setLookAt(Point(278, 278, 0));
    cam.setVup(Vec3(0, 1, 0));
    cam.setDefocusAngle(0.6);
    cam.setFocusDist(10.0);
    cam.setBackgroundColor(Color(0, 0, 0));

    cam.render(world);

}

void cornell_smoke() {
    hittable_list world;
    auto red = make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto green = make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));

    world.add(make_shared<Quad>(Point(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.add(make_shared<Quad>(Point(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.add(make_shared<Quad>(Point(113, 554, 127), Vec3(555, 0, 0), Vec3(0, 0, 555), light));
    world.add(make_shared<Quad>(Point(0, 555, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    shared_ptr<hittable> box1 = box(Point(0, 0, 0), Point(165, 165, 165), white);
    box1 = make_shared<Rotate_Y>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
    
    shared_ptr<hittable> box2 = box(Point(0, 0, 0), Point(165, 165, 165), white);
    box2 = make_shared<Rotate_Y>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));

    world.add(make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
    world.add(make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

    Camera cam;
    cam.setAspectRatio(1.0);
    cam.setImageWidth(600);
    cam.setSampleCount(10);
    cam.setMaxRays(50);
    cam.setCameraVFov(40);
    cam.setLookFrom(Point(278, 278, -800));
    cam.setLookAt(Point(278, 278, 0));
    cam.setVup(Vec3(0, 1, 0));
    cam.setDefocusAngle(0.6);
    cam.setFocusDist(10.0);
    cam.setBackgroundColor(Color(0, 0, 0));

    cam.render(world);
}

void part2_final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;
            boxes1.add(box(Point(x0, y0, z0), Point(x1, y1, z1), ground));
        }
    }

    hittable_list world;
    world.add(make_shared<BVH_Node>(boxes1));

    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
    world.add(make_shared<Quad>(Point(123, 554, 147), Vec3(300, 0, 0), Vec3(0, 0, 256), light));

    auto center1 = Point(400, 400, 200);
    Vec3 offset = Vec3(30, 0, 0);
    auto px = center1.x() + offset.x();
    auto py = center1.y() + offset.y();
    auto pz = center1.z() + offset.z();
    auto center2 = Point(px, py, pz);
    auto sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.add(make_shared<Sphere>(center1, center2, 50, sphere_material));

    world.add(make_shared<Sphere>(Point(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    world.add(make_shared<Sphere>(Point(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<Sphere>(Point(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    world.add(make_shared<ConstantMedium>(boundary, 0.0001, Color(1, 1, 1)));

    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("earthmap.jpg"));
    world.add(make_shared<Sphere>(Point(400, 200, 400), 100, emat));
    auto pertext = make_shared<NoiseTexture>(0.2);
    world.add(make_shared<Sphere>(Point(220, 280, 300), 80, make_shared<Lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<Sphere>(Point(random_int(0, 165), random_int(0, 165), random_int(0, 165)), 10, white));
    }

    world.add(make_shared<Translate>(make_shared<Rotate_Y>(make_shared<BVH_Node>(boxes2), 15), Vec3(-100, 270, 395)));

    Camera cam;
    cam.setAspectRatio(1.0);
    cam.setImageWidth(600);
    cam.setSampleCount(10);
    cam.setMaxRays(50);
    cam.setCameraVFov(40);
    cam.setLookFrom(Point(478, 278, -600));
    cam.setLookAt(Point(278, 278, 0));
    cam.setVup(Vec3(0, 1, 0));
    cam.setDefocusAngle(0);
    cam.setFocusDist(10.0);
    cam.setBackgroundColor(Color(0, 0, 0));

    cam.render(world);
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
        case '4': perlin_spheres(); break;
        case '5': quads(); break;
        case '6': simple_light(); break;
        case '7': cornell_box(); break;
        case '8': cornell_smoke(); break;
        case '9': part2_final_scene(); break;
        default: std::cout << "Couldn't render image, wrong input?" << endl;
    }
}