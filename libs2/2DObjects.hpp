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
  bool intersection(const Direction& ray, const Point& origen, float &dist) override {
    float denom = dotProduct(normal, ray);
    if (fabs(denom) > 1e-6 ) {
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
  Point a, b, c;
public:
  Triangle(Point _a, Point _b, Point _c) : a(_a), b(_b), c(_c) {}
  // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
  bool intersection(const Direction& ray, const Point& origen, float &dist) override {
    Direction aux1 = b-a;
    Direction aux2 = c-a;
    Direction normal = crossProduct(aux1, aux2);
    float denom = dotProduct(normal, ray);
    if (fabs(denom) < 1e-6 ) {// triangle paralelo a rayo
      return false;
    }
    float d = dotProduct(normal, (a-origen));
    float t = (dotProduct(normal, Direction(0,0,0)) + d) / denom;
    if (t < 0) return false; // triangle detras de camara
    dist = t*ray.modulus();

    // Punto de interseccion
    Point P = origen + (ray * t);

    // Test dentro-fuera triangle
    Direction C;

    // edge 0
    Direction edge0 = b - a;
    Direction vp0 = P - a;
    C = crossProduct(edge0,vp0);
    if (dotProduct(normal,C) < 0) return false; // P is on the right side

    // edge 1
    Direction edge1 = c - b;
    Direction vp1 = P - b;
    C = crossProduct(edge1,vp1);
    if (dotProduct(normal,C) < 0)  return false; // P is on the right side

    // edge 2
    Direction edge2 = a - c;
    Direction vp2 = P - c;
    C = crossProduct(edge2,vp2);
    if (dotProduct(normal,C) < 0) return false; // P is on the right side;

    return true; // this ray hits the triangle
  }
};
  class Pelota : public Object {
  private:
    Point center;
    float radius;
  public:
    Pelota(Point cen, float rad) : center(cen), radius(rad) {}
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
    bool intersection(const Direction& ray, const Point& origen, float &dist) override {
              float t0, t1; // solutions for t if the ray intersects
              // analytic solution
              Direction L = origen - center;
              float a = dotProduct(ray,ray);
              float b = 2 * dotProduct(ray,L);
              float c = dotProduct(L,L) - (radius*radius);
              if (!solveQuadratic(a, b, c, t0, t1)) return false;

              if (t0 > t1) std::swap(t0, t1);

              if (t0 < 0) {
                  t0 = t1; // if t0 is negative, let's use t1 instead
                  if (t0 < 0) return false; // both t0 and t1 are negative
              }

              float t = t0;
              dist = t*ray.modulus();

              return true;
      }
      bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
      {
          float discr = b * b - 4 * a * c;
          if (discr < 0) return false;
          else if (discr == 0) x0 = x1 = - 0.5 * b / a;
          else {
              float q = (b > 0) ?
                  -0.5 * (b + sqrt(discr)) :
                  -0.5 * (b - sqrt(discr));
              x0 = q / a;
              x1 = c / q;
          }
          if (x0 > x1) std::swap(x0, x1);

          return true;
      }
};
