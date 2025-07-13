#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <cstdlib>
#include <iostream>

class vec3 {
public:
  double e[3];

  vec3() : e{0, 0, 0} {}
  vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  double operator[](int i) const { return e[i]; }
  double &operator[](int i) { return e[i]; }

  vec3 &operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  vec3 &operator*=(double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3 &operator/=(double t) { return *this *= 1 / t; }

  double length() const { return std::sqrt(length_squared()); }

  double length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

  double near_zero() const {
    // this is implemented to prevent normal to completely minimise the
    // reflected ray in material and cause an infinite result
    auto s = 1e-8;
    // this function returns true if the absolute value in any dimension is
    // close to near 0
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) &&
           (std::fabs(e[2]) < s);
  }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the
// code.
using point3 = vec3;

// Vector Utility Functions

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) { return t * v; }

inline vec3 operator/(const vec3 &v, double t) { return (1 / t) * v; }

inline double dot(const vec3 &u, const vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// returns a random value between 0 and 1
inline double random_double() { return std::rand() / (RAND_MAX + 1.0); }

// returns a random real number between min and max, this is called an
// overload the function runs based on what parameter you give it

inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}

static vec3 random(double min, double max) {
  return vec3(random_double(min, max), random_double(min, max),
              random_double(min, max));
}

inline vec3 unit_vector(const vec3 &v) { return v / v.length(); }

inline vec3 random_unit_vector() {

  while (true) {
    // chose a random point between 1 and -1 and create a vector
    auto p = random(-1, 1);
    auto lensq = p.length_squared();
    //
    // the reason why lensq has the minimum limit is to prevent a floating point
    // abstraction leak
    if (1e-160 <= lensq <= 1) {
      // if the length of the chosen point is within range
      return p / sqrt(lensq); // (vector p/ magnitude p)  nomalise the vector
    }
  }
}

// make dot product to compare whether the surface normal and random
// (reflection) vector face in the same direction if the dot prodouct is
// positive they do if the dot product doesnt then the random vector needs to be
// inverted
inline vec3 random_on_hemisphere(const vec3 &normal) {

  vec3 on_unit_sphere = random_unit_vector();
  if (dot(on_unit_sphere, normal) > 0.0) {
    return on_unit_sphere;
  } else
    return -on_unit_sphere;
}

// we find b (which is the second half the eqn) by finding the projection of v
// onto n and then multiplying that by n because b is a scaled version of n
// and then we negate the final value because v is pointing into the screen and
// we want b to oppose that and point out of the screen to represent a
// reflection
inline vec3 reflect(const vec3 &v, const vec3 &n) {
  return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat) {
  auto cos_theta = std::fmin(dot(-uv, n), 1);
  vec3 r_out_perp = etai_over_etat * (uv * cos_theta * n);
  vec3 r_out_para =
      -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_para + r_out_perp;
}

#endif
