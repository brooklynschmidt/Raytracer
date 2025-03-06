#include <iostream>
#include <stdio.h>
#include <glm/vec3.hpp>

using namespace std;

int main() {

    // Image Properties
    int width = 256;
    int height = 256;

    // PPM File
    cout << "P3\n";
    cout << width << "\n" << height << "\n";
    cout << "256\n";
    
    for (int i = 0; i < height; i++) {
        clog << "\r Scanlines remaining: " << height - i << "\n" << flush;
        for (int j = 0; j < width; j++) {
            int r = j;
            int g = i;
            int b = 0;

            cout << r << " " << g << " " << b << "\n";
        }
    }
    clog << "\rDone. \n";
}