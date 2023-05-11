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
    const std::filesystem::path resource_dir("/mnt/c/_github/Map-Projection-Conversion/resource");
    const std::filesystem::path in_dir(resource_dir/"input");
    const std::filesystem::path out_dir(resource_dir/"output");
    std::vector<image_t> images;

    for (const auto& entry: std::filesystem::directory_iterator(in_dir)) {
        std::cout << "Reading: " << entry.path().string() << std::endl;
        images.push_back(load_image(entry.path().string(), entry.path().filename().string()));
    }

    for(auto img: images) {
        std::cout << "Processing: " << img.name << std::endl;
        image_t altered = add_green_px(img);
        save_image(altered, out_dir/std::filesystem::path(img.name));
    }

    std::cout << "Done" << std::endl;
}