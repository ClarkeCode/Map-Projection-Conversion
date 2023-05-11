#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <math.h>

#include <lodepng.h>
#include <cassert>

#include "auxi.hpp"

pixel_t& image_t::px_at(size_t x, size_t y) {
    assert(x < width);
    assert(y < height);
    return pixels_1d[x + y * width];
}

image_t load_image(std::string filepath, std::string filename) {
    std::vector<uint8_t> raw_image;
    unsigned width, height;

    uint32_t error = lodepng::decode(raw_image, width, height, filepath.c_str());

    std::vector<pixel_t> pixels_1d;
    for (size_t i = 0; i < width * height * 4; i += 4) {
        pixels_1d.push_back(fromRGBA(raw_image[i], raw_image[i + 1], raw_image[i + 2], raw_image[i + 3]));
    }
    return image_t { pixels_1d, filename, height, width };
}

void save_image(image_t& img, std::string filepath) {
    std::vector<uint8_t> pixels;

    for (pixel_t const& px : img.pixels_1d) {
        pixels.push_back(px.red);
        pixels.push_back(px.green);
        pixels.push_back(px.blue);
        pixels.push_back(px.alpha);
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