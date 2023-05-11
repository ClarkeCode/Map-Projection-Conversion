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

inline pixel_t fromRGB(uint8_t r, uint8_t g, uint8_t b) { return pixel_t{r, g, b, 255}; }
inline pixel_t fromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { return pixel_t{r, g, b, a}; }

struct image_t {
    std::vector<pixel_t> pixels_1d;
    std::string name;
    size_t height, width;

    pixel_t& px_at(size_t x, size_t y);
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