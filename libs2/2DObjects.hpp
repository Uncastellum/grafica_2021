

#include "GeOS.hpp"


class Plane : public Object {
private:
  Point p;
  Direction normal;
  RBG solid_color;
public:
  Plane(Point dist, Direction norm) : d(dist), normal(norm) {}

  void setRBG(RBG sc){
    solid_color = sc;
  }


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

  bool intersection(const Direction& ray, const Point& origen, double &dist) override {

  }
};
