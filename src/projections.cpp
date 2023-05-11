#include "auxi.hpp"
#include "projections.hpp"
#include <cmath>

// Converting a set of pixel indices to their corresponding latitude and longitude on a sphere
// Please see the compasition for more details but this is effectively a straight riff of the wikipedia formula
// The centre point is just used to translate the pixel space to the real space -> should probably be a seperate function
// Equator length is also calculated help map scale correctly -> should also be refactored to rather generate a scale for each pixel (at the equator)
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

// Converting a set of latitude and longitudes to a gall steoregraphic projection
vec2_t forward_gall(const latlng_t point, const vec2_t geo_centre, const double equator_length) {
    // Get the radius of the globe latlng is mapped to by using the length of the equator
    const double radius = equator_length / ( 2 * PI );

    return vec2_t {
        (radius * point.lng / sqrt(2)) + geo_centre.x,
        (radius * (1 + sqrt(2) / 2) * tan(point.lat / 2)) + geo_centre.y
    };
}

// Composed Formulae of the reverese_equirect and forward_gall projections 
vec2_t equirect_to_gall(const vec2_t point, const vec2_t geo_centre, const double equator_length) {
    // Constants required by the formula
    // Equator length is the only funny calculation and probably needs to be revisited to be more correct
    const double radius = equator_length / ( 2 * PI ); // Determine the globe radius by using the equator length (in pixels)
    const double centre_lat = 0;
    const double centre_lng = 0;
    const double standard_lat = 0;//PI/4; // At what parallel do we start deforming the projection. Not sure what qaz's map uses but I'm just assuming its at the equator
    const double standard_lng = 0;//PI/4;

    double x = ( (point.x + geo_centre.x) / (sqrt(2) * cos(standard_lat)));
    double y = ( radius * ( 1 + sqrt(2) / 2) * tan( (point.y + geo_centre.y) / (2 * radius) ) );

    return vec2_t {x, y};
}

// Returning the newly projected image - does not work
// Returns a pair, first the new image and secondly the cumulative pixel loss of the projection
std::pair<image_t, double> map_equirect_to_gall(image_t& img) {

    vec2_t eq_centre = calc_centre(img);
    vec2_t gs_centre = forward_gall(reverse_equirect(eq_centre, eq_centre, img.width), eq_centre, img.width);
    image_t result = img;

    // Not finished, meant to keep track of the minimum value created after the projection, and then shift it towards zero
    // A hack to correctly shift things in place and not implemented. A better solution would be to construct a function that can
    // map the gall_stereographic space to the pixel space -> that was a recommendation by a lecturer of mine
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

    // Not actually calculating the loss yet
    return std::make_pair(result, 1);
}