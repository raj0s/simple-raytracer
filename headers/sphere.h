#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <memory>

// the line below means that the sphere class inherits from hittable
class sphere : public hittable {
public:
  // making a reference to the point3 class in order to avoid copying and
  // wasting time copying
  sphere(const point3 &center, double radius, std::shared_ptr<material> mat)
      : center(center), radius(std::fmax(0, radius)), mat(mat) {

    // outline the material that the sphere object has through the user of a mat
    // pointer
  }

  bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - (radius * radius);

    auto discriminant = h * h - a * c;

    if (discriminant < 0) {
      return false;
    }

    auto sqrtd = std::sqrt(discriminant);

    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {

      root = (h + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        return false;
      }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;
    return true;
  }

private:
  point3 center;
  double radius;
  std::shared_ptr<material> mat;
};

#endif
