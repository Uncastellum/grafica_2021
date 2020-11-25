#pragma once

//#include "GeOS.hpp"
#include "BasicsRender.hpp"

// g++ -std=c++11 -I. 3DObjects.hpp -O3 -o efe.o


class Pelota : public Object {
private:
  Point center;
  float radius;
public:
  Pelota(Point cen, float rad) : center(cen), radius(rad) {}
  // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
  bool intersection(const Ray& r, float &t, float &dist, Direction& n) override {
    float t0, t1; // solutions for t if the ray intersects
    // analytic solution
    Direction aux = r.orig - center;
    float a = r.dir.modulus(); a *= a; // r.dir.modulus()*r.dir.modulus()
    float b = 2 * dotProduct(r.dir,aux);
    float c = aux.modulus(); c *= c; c -= (radius*radius);

    if (!solveQuadratic(a, b, c, t0, t1)) return false;
    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
      t0 = t1; // if t0 is negative, let's use t1 instead
      if (t0 < 0) return false; // both t0 and t1 are negative
    }

    t = t0;
    dist = t*r.dir.modulus();
    n = ((r.orig + r.dir*t) - center).normalize();

    return true;
  }

  void transform(const Matrix &m) override {
    // translate
    center[xi]+=m(0,3); center[yj]+=m(1,3); center[zk]+=m(2,3);
    // scalate
    Direction mod(m(0,3), m(1,3), m(2,3));
    radius *= mod.modulus();
  }

  bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) {
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
