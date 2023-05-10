#include "auxi.hpp"

#define PI 3.14159265358979323846

vec2_t equirect_to_gall(const vec2_t point, const vec2_t geo_centre, const double equator_length);
latlng_t reverse_equirect(const vec2_t point, const vec2_t geo_centre, const double equator_length);
vec2_t forward_gall(const latlng_t point, const vec2_t geo_centre, const double equator_length);
std::pair<image_t, double> map_equirect_to_gall(image_t& img);