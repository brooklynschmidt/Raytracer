#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

// Disable strict warnings for this header from the Microsoft C++ compiler
#ifndef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include <cstdlib>
#include <iostream>

class RTW_Image {
    public:
        RTW_Image() {}
        RTW_Image(const char* image_filename) {
            // Loads image data from the specified file
            auto filename = std::string(image_filename);
            
            // Look for file name
            if (load("images/" + filename)) return;
            std::cerr << "Couldn't locate filename" << std::endl;
        }

        ~RTW_Image() {
            delete[] bdata;
            stbi_image_free(fdata);
        }

        bool load(const std::string& filename) {
            // Loads the linear (gamma=1) image data from the given name
            // Returns true if the load succeeded
            // Resulting data buffer contains three [0.0, 1.0] floating-point values for the first pixel (RGB)
            // Pixels are continuous, going left to right, then row by row
            auto n = bytes_per_pixel; // Dummy out parameter
            fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
            if (fdata == nullptr) return false;

            bytes_per_scanline = image_width * bytes_per_pixel;
            convert_to_bytes();
            return true;
        }

        int width() const { return (fdata == nullptr) ? 0 : image_width; }
        int height() const {return (fdata == nullptr) ? 0 : image_height; }

        const unsigned char* pixel_data(int x, int y) const {
            // Return the address of the three RGB bytes of the pixel at x, y
            // Return magenta if no image data
            static unsigned char magenta[] = { 255, 0, 255 };
            if (bdata == nullptr) return magenta;
            x = clamp(x, 0, image_width);
            y = clamp(y, 0, image_height);

            return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
        }

    private:
        const int bytes_per_pixel = 3;
        float *fdata = nullptr; // Linear floating point pixel data
        unsigned char *bdata = nullptr; // Linear 8-bit pixel data
        int image_width = 0; 
        int image_height = 0;
        int bytes_per_scanline = 0;

        static int clamp(int x, int low, int high) {
            // Return the value clamped to the range [low, high)
            if (x < low) return low;
            if (x < high)  return x;
            return high - 1;
        }

        static unsigned char float_to_byte(float value) {
            if (value <= 0.0) {
                return 0;
            }
            if (1.0 <= value) {
                return 255;
            }

            return static_cast<unsigned char>(256.0 * value);
        }

        void convert_to_bytes() {
            // Convert the linear floating point pixel data to bytes, storing the resulting byte data in the "bdata" member
            int total_bytes = image_width * image_height * bytes_per_pixel;
            bdata = new unsigned char[total_bytes];

            // Iterate through all pixel components, converting from [0.0, 1.0] float values to unsigned [0, 255] bytes values
            auto *bptr = bdata;
            auto *fptr = fdata;
            for (int i = 0; i < total_bytes; i++, fptr++, bptr++) {
                *bptr = float_to_byte(*fptr);
            }
        }
};

#endif