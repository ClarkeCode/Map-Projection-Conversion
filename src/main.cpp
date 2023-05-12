// Std
#include <cstdint>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <tuple>

#include "auxi.hpp"
#include "projections.hpp"

// Please note if I manage to figure out how cmake works, I plan on adding the fmt library to this project instead of iostream

image_t add_green_px(image_t& img) {
    image_t more = img;
    return more;
}

int main() {
    // Use c++ 20 filesystem because I hate iterating through directories
    const std::filesystem::path resource_dir("...");
    const std::filesystem::path in_dir(resource_dir/"input");
    const std::filesystem::path out_dir(resource_dir/"output");
    std::vector<image_t> images;

    for (const auto& entry: std::filesystem::directory_iterator(in_dir)) {
        std::cout << "Reading: " << entry.path().string() << std::endl;
        images.push_back(load_image(entry.path().string(), entry.path().filename().string()));
    }

    for(auto img: images) {
        std::cout << "Processing: " << img.name << std::endl;

        std::cout << "Pixel To Real Mapping Points of Interest" << std::endl;
        vec2_t eq_centre = calc_centre(img);
        {
            vec2i_t min_px = vec2i_t { 0 , 0 };
            vec2_t min_px_to_rl = map_pixel_to_real(min_px, eq_centre);
            vec2i_t min_px_from_rl = map_real_to_pixel(min_px_to_rl, eq_centre);
            pixel_t& actual_min_px = img.px_at(min_px);

            vec2i_t max_px = vec2i_t { (int64_t)(img.width - 1) , (int64_t)(img.height - 1) };
            vec2_t max_px_to_rl = map_pixel_to_real(max_px, eq_centre);
            vec2i_t max_px_from_rl = map_real_to_pixel(max_px_to_rl, eq_centre);
            pixel_t& actual_max_px = img.px_at(max_px);

            vec2i_t mid_px = vec2i_t { (int64_t)(eq_centre.x) , (int64_t)(eq_centre.y) };
            vec2_t mid_px_to_rl = map_pixel_to_real(mid_px, eq_centre);
            vec2i_t mid_px_from_rl = map_real_to_pixel(mid_px_to_rl, eq_centre);
            pixel_t& actual_mid_px = img.px_at(mid_px);

            std::cout << "\tPixels (" << min_px.i << "," << min_px.j << ") => Real (" << min_px_to_rl.x << "," << min_px_to_rl.y << ") => Pixel (" << min_px_from_rl.i << "," << min_px_from_rl.j << ")" << std::endl;
            std::cout << "\t\tPixel RGB (" << (int)actual_min_px.red << "," << (int)actual_min_px.green << "," << (int)actual_min_px.blue << ")" << std::endl;
            std::cout << "\tPixels (" << mid_px.i << "," << mid_px.j << ") => Real (" << mid_px_to_rl.x << "," << mid_px_to_rl.y << ") => Pixel (" << mid_px_from_rl.i << "," << mid_px_from_rl.j << ")" << std::endl;
            std::cout << "\t\t Pixel RGB (" << (int)actual_mid_px.red << "," << (int)actual_mid_px.green << "," << (int)actual_mid_px.blue << ")" << std::endl;
            std::cout << "\tPixels (" << max_px.i << "," << max_px.j << ") => Real (" << max_px_to_rl.x << "," << max_px_to_rl.y << ") => Pixel (" << max_px_from_rl.i << "," << max_px_from_rl.j << ")" << std::endl;
            std::cout << "\t\t Pixel RGB (" << (int)actual_max_px.red << "," << (int)actual_max_px.green << "," << (int)actual_max_px.blue << ")" << std::endl;
        };
        // save_image(conversion_result, (out_dir/img.name).string());
    }

    std::cout << "Done" << std::endl;
}