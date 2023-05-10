#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <tuple>

struct pixel_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

struct image_t {
    std::vector<std::vector<pixel_t>> pixels_2d;
    std::string name;
    size_t height, width;
};

struct vec2_t {
    double x;
    double y;
};

struct latlng_t {
    double lat;
    double lng;
};

image_t load_image(std::string filepath, std::string filename);
void save_image(image_t& img, std::string filepath);
vec2_t calc_centre(image_t& img);