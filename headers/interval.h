#ifndef INTERVAL_H
#define INTERVAL_H
#include <limits>

class interval {
public:
  double min, max;

  const double infinity = std::numeric_limits<double>::infinity();

  interval() : min(+infinity), max(-infinity) {} // Default interval is empty

  interval(double min, double max) : min(min), max(max) {}

  double size() const { return max - min; }

  bool contains(double x) const { return min <= x && x <= max; }

  bool surrounds(double x) const { return min < x && x < max; }

  // when fetching for color samples the bounds need to be between [0,1]
  double clamp(double x) const {
    if (x < min)
      return min;
    if (x > max)
      return max;

    return x;
  }

  static const interval empty, universe;
};

const interval interval::empty;
const interval interval::universe;

#endif
