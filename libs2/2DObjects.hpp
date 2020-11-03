#pragma once

#include "GeOS.hpp"

// g++ -std=c++11 -I. 2DObjects.hpp -O3 -o efe.o

class Plane : public Object {
private:
  Point p;
  Direction normal;

public:
  Plane(Point dist, Direction norm) : p(dist), normal(norm) {}

  // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
  bool intersection(const Direction& ray, const Point& origen, double &dist) override {
    float denom = dotProduct(normal, ray);
    if (denom > 1e-6) {
        Direction p_orig = p - origen;
        float t = dotProduct(p_orig, normal) / denom;
        dist = t*ray.modulus();
        return (t >= 0);
    }
    return false;
  }
};

class Triangle : public Object {
private:

public:

  // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
  bool intersection(const Direction& ray, const Point& origen, double &dist) override {
    return true;
  }
};
