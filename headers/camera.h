#pragma once
#include "color.h"
#include "hittable.h"
#include "rtweekend.h"
#include "vec3.h"

class camera {

public:
  double aspect_ratio = 1.0;
  int image_width = 100;
  int samples_per_pixel = 10;
  int max_depth = 10; // introduced to prevent max depth recursion with the
                      // ray_color function

  void render(const hittable &world) {
    // Render

    init();

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++) {

      std::clog << "\rScanlines remaining: " << (image_height - j) << ' '
                << std::flush;
      for (int i = 0; i < image_width; i++) {
        color pixel_color(0, 0, 0);

        for (int sample = 0; sample < samples_per_pixel; sample++) {
          ray r = get_ray(i, j);
          pixel_color += ray_color(r, max_depth, world);
        }
        write_color(std::cout, pixel_samples_scale * pixel_color);

        // // a pixel center is essential to be calculated because it defines
        // the
        // // location of the pixel relative to the viewport
        // auto pixel_center =
        //     pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        // auto ray_direction = pixel_center - center;
        // ray r(center, ray_direction);
        //
        // pixel_color = ray_color(r, world);
        // write_color(std::cout, pixel_color);
      }
    }
    std::clog << "\rDone.             \n";
  };

private:
  int image_height;
  point3 center;
  double pixel_samples_scale;
  point3 pixel00_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;

  void init() {
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = point3(0, 0, 0);
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width =
        viewport_height * (double(image_width) / image_height);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left =
        center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
  }

  // get the ray from a randomised pixel location to use as a sample to prevent
  // aliasing, the ray origin is the center where the camera is located
  ray get_ray(int i, int j) const {

    auto offset = sample_square();
    auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) +
                        ((j + offset.y()) * pixel_delta_v);
    auto ray_direction = pixel_sample - center;
    return ray(center, ray_direction);
  }

  vec3 sample_square() const {
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
  }
  // used for returning the color for a given
  // ray scene
  color ray_color(const ray &r, int depth, const hittable &world) {
    if (depth <= 0) {
      return color(0, 0, 0);
    }
    hit_record rec;
    // making the min interval 0.001 instead of 0 fixes shadow acne by
    // preventing the floating point error
    if (world.hit(r, interval(0.001, infinity), rec)) {
      // vec3 direction = rec.normal + random_unit_vector();
      ray scattered;
      color attenuation;
      // the coefficient represents the reflectance, higher the coefficient the
      // more reflective the surface of the sphere is
      // return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);
      if (rec.mat->scatter(r, rec, attenuation, scattered)) {
        return attenuation * ray_color(scattered, depth - 1, world);
      }
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
  }
};
