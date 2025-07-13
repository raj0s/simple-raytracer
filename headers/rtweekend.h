#ifndef RTWEEKEND_H
#define RTWEEKEND_H

// file with all common headers and math values that are needed by the rt
#include "color.h"
#include "hittable_list.h"
#include "interval.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) { return degrees * pi / 180; }

#endif
