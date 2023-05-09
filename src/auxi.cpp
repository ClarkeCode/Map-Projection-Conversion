#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <math.h>

#include <lodepng.h>

#include "auxi.hpp"

#define PI 3.14159265358979323846

image_t load_image(std::string filepath, std::string filename) {
    std::vector<uint8_t> raw_image;
    unsigned width, height;

    uint32_t error = lodepng::decode(raw_image, width, height, filepath.c_str());

    std::vector<std::vector<pixel_t>> pixels_2d;

    for (size_t i = 0; i < height; ++i) {
        pixels_2d.push_back(std::vector<pixel_t>());
    }

    size_t pix_count = 0;
    for (size_t i = 0; i < width * height * 4; i += 4) {
        pixel_t current_pixel = { raw_image[i], raw_image[i + 1], raw_image[i + 2], raw_image[i + 3] };
        size_t y = pix_count % height;
        size_t x = pix_count / height;

        pixels_2d[y].push_back(current_pixel);
        ++pix_count;
    }

    return image_t { pixels_2d, filename, height, width };
}

void save_image(image_t& img, std::string filepath) {
    std::vector<uint8_t> pixels;

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

vec2_t calc_centre(image_t& img) {
    return vec2_t {
        static_cast<double>(img.width / 2),
        static_cast<double>(img.height / 2),
    };
}

latlng_t reverse_equirect(const vec2_t point, const vec2_t geo_centre, const double equator_length) {
    const vec2_t correct_point = vec2_t { point.x - geo_centre.x, point.y - geo_centre.y };

    const double radius = equator_length / ( 2 * PI );
    const double centre_lat = 0;
    const double centre_lng = 0;
    const double standard_lat = 0;
    const double standard_lng = 0;

    return latlng_t {
        (correct_point.y / radius + centre_lat),
        (correct_point.x / (radius * cos(standard_lat)) + centre_lng)
    };
}

vec2_t forward_gall(const latlng_t point, const vec2_t geo_centre, const double equator_length) {
    const double radius = equator_length / ( 2 * PI );

    return vec2_t {
        (radius * point.lng / sqrt(2)) + geo_centre.x,
        (radius * (1 + sqrt(2) / 2) * tan(point.lat / 2)) + geo_centre.y
    };
}

vec2_t equirect_to_gall(const vec2_t point, const vec2_t geo_centre, const double equator_length) {
    const double radius = equator_length / ( 2 * PI );
    const double centre_lat = 0;
    const double centre_lng = 0;
    const double standard_lat = 0;//PI/4;
    const double standard_lng = 0;//PI/4;

    double x = ( (point.x + geo_centre.x) / (sqrt(2) * cos(standard_lat)));
    double y = ( radius * ( 1 + sqrt(2) / 2) * tan( (point.y + geo_centre.y) / (2 * radius) ) );

    return vec2_t {x, y};
}

std::pair<image_t, double> map_equirect_to_gall(image_t& img) {

    vec2_t eq_centre = calc_centre(img);
    vec2_t gs_centre = forward_gall(reverse_equirect(eq_centre, eq_centre, img.width), eq_centre, img.width);
    image_t result = img;

    std::vector<std::vector<vec2_t>> projection_res;
    vec2_t min_vals = vec2_t {
        100000000000000000000000.0,
        100000000000000000000000.0,
    };

    for (size_t y = 0; y < img.height; ++y) {
        projection_res.push_back(std::vector<vec2_t>());
        for (size_t x = 0; x < img.width; ++x) {
            result.pixels_2d[y][x] = pixel_t { 255, 255, 255, 255 };
            vec2_t fg_xy = equirect_to_gall(vec2_t {(double)x,(double)y}, eq_centre, img.width);
            // std::cout << "{ " << x << " , " << y << " } -> { " << fg_xy.x << " , " << fg_xy.y << " }\n";
            projection_res[y].push_back(fg_xy);
        }
    }

    return std::make_pair(result, 1);
}