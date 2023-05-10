#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <math.h>

#include <lodepng.h>

#include "auxi.hpp"

image_t load_image(std::string filepath, std::string filename) {
    std::vector<uint8_t> raw_image;
    unsigned width, height;

    uint32_t error = lodepng::decode(raw_image, width, height, filepath.c_str());

    std::vector<std::vector<pixel_t>> pixels_2d;

    for (size_t i = 0; i < height; ++i) {
        pixels_2d.push_back(std::vector<pixel_t>());
    }

    // Convert the 1D pixel map to a 2D array for convenience
    // I suck at math so not having to do the index -> x,y is nice
    size_t pix_count = 0;
    for (size_t i = 0; i < width * height * 4; i += 4) {
        pixel_t current_pixel = { raw_image[i], raw_image[i + 1], raw_image[i + 2], raw_image[i + 3] };
        size_t y = pix_count % height;
        size_t x = pix_count / height;

        pixels_2d[y].push_back(current_pixel);
        ++pix_count; // Need to use pixel_count to correctly keep track of the where we are in the 2d array
        // Could probably also just be done by (i / 4)
    }

    return image_t { pixels_2d, filename, height, width };
}

void save_image(image_t& img, std::string filepath) {
    std::vector<uint8_t> pixels;

    // Convert the 2D pixel array back into the 1D pixel array used by LodePNG
    for (size_t y = 0; y < img.height; ++y) {
        for (size_t x = 0; x < img.width; ++x) {
            pixels.push_back(img.pixels_2d[y][x].red);
            pixels.push_back(img.pixels_2d[y][x].green);
            pixels.push_back(img.pixels_2d[y][x].blue);
            pixels.push_back(img.pixels_2d[y][x].green);
        }
    }

    unsigned error = lodepng::encode(filepath.c_str(), pixels, img.width, img.height);
}

// Gets the centre of an image
vec2_t calc_centre(image_t& img) {
    return vec2_t {
        static_cast<double>(img.width / 2),
        static_cast<double>(img.height / 2),
    };
}