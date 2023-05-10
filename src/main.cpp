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

int main() {

    // Use c++ 20 filesystem because I hate iterating through directories
    const std::filesystem::path in_dir("C:\\Users\\Orwellian\\OpenVic2\\map-convert\\resource\\input");
    const std::filesystem::path out_dir("C:\\Users\\Orwellian\\OpenVic2\\map-convert\\resource\\output");
    std::vector<image_t> images;

    for (const auto& entry: std::filesystem::directory_iterator(in_dir)) {
        images.push_back(load_image(entry.path().string(), entry.path().filename().string()));
    }

    // std::cout << "DEBUG\n";
    // std::cout << "COMPOSITION WITH CENTRE {500, 250}\n";
    // vec2_t new_centre = equirect_to_gall(vec2_t{500,250}, vec2_t{500, 250}, 1000);
    // vec2_t comp1 = equirect_to_gall(vec2_t{0,0}, vec2_t{500, 250}, 1000);
    // vec2_t comp2 = equirect_to_gall(vec2_t{1000,0}, vec2_t{500, 250}, 1000);
    // vec2_t comp4 = equirect_to_gall(vec2_t{1000,15}, vec2_t{500, 250}, 1000);
    // vec2_t comp3 = equirect_to_gall(vec2_t{800,200}, vec2_t{500, 250}, 1000);
    // std::cout << "\tNew Centre {500,250} = {" << new_centre.x << "," << new_centre.y << "}\n";
    // std::cout << "\t{0,0} = {" << comp1.x << "," << comp1.y << "}\n";
    // std::cout << "\t{1000,0} = {" << comp2.x << "," << comp2.y << "}\n";
    // std::cout << "\t{1000,15} = {" << comp4.x << "," << comp4.y << "}\n";
    // std::cout << "\t{800,200} = {" << comp3.x << "," << comp3.y << "}\n";

    for(auto img: images) {
        vec2_t centre = calc_centre(img);
        std::pair<image_t, double> result = map_equirect_to_gall(img);
        save_image(result.first, out_dir.string() + "\\" + img.name);
    }

}