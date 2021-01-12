

#pragma once

//Compile test: g++ -std=c++11 -I. BasicsRender.hpp -O3 -o efe.o

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include "GeOS.hpp"
#include "Matrix.hpp"

using namespace std;

float rand0_1() {
  static bool const dummy = (srand(time(NULL)), true);
  return (float) rand() / (RAND_MAX);
}

struct Ray {
  Point orig;
  //Direction dir;
  class {
    public:
      Direction d_norm;
      float mod;
      Direction& operator= (const Direction &i) {
        mod = i.modulus();
        if (mod == 0) d_norm = i;
        else d_norm = i/mod;
        mod = 1; // tras normalizar, modulo = 1
        return d_norm;
      }
      operator Direction () const { return d_norm; }
      Direction operator*(float fl) const { return d_norm*fl; }
      Direction operator/(float fl) const { return d_norm/fl; }
      float modulus() const { return mod; }
  } dir;
  Ray(){}
  Ray(Point o, Direction d) { orig = o; dir = d;}
  void setdirnorm(Direction d) { // d DEBE ESTAR NORMALIZADO
    dir.d_norm = d;
    dir.mod = 1;
  }
};

void paint(Ray r){
  printf("Ray{");
  paint(r.orig);
  printf(",");
  paint(r.dir);
  printf("}");
}

struct RGB {
  float red, green, blue;
  RGB() : red(0), green(0), blue(0) {}
  RGB(float mono) : red(mono), green(mono), blue(mono) {}
  RGB(float r, float g, float b) : red(r), green(g), blue(b) {}
  RGB operator*(float i){
    return RGB(red*i, green*i, blue*i);
  }
  RGB operator*(RGB i){
    return RGB(red*i.red, green*i.green, blue*i.blue);
  }
  RGB operator+(RGB i){
    return RGB(red+i.red, green+i.green, blue+i.blue);
  }
  RGB operator/(float i){
    return RGB(red/i, green/i, blue/i);
  }
};
RGB RGB255(float mono) {
  return RGB(mono/255, mono/255, mono/255);
}
RGB RGB255(float r, float g, float b) {
  return RGB(r/255, g/255, b/255);
}

float max(const RGB& c) {
  if (c.red > c.green && c.red > c.blue) return c.red;
  if (c.green > c.blue) return c.green;
  return c.blue;
}

void paint(RGB t){
  printf("rgb(%.3f  ", t.red);
  printf("%.3f  ", t.green);
  printf("%.3f)", t.blue);
}

// if n.modulus == 1, (b1 & b2).modulus == 1
// http://jcgt.org/published/0006/01/01/
void revisedONB(const Direction &n, Direction &b1, Direction &b2){
  if(n[zk]<0.){
    const float a = 1.0f / (1.0f - n[zk]);
    const float b = n[xi]*n[yj]*a;
    b1 = Direction(1.0f - n[xi]*n[xi]*a, -b, n[xi]);
    b2 = Direction(b, n[yj]*n[yj]*a - 1.0f, -n[yj]);
  } else {
    const float a = 1.0f / (1.0f + n[zk]);
    const float b = -n[xi]*n[yj]*a;
    b1 = Direction(1.0f - n[xi]*n[xi]*a, b, -n[xi]);
    b2 = Direction(b, 1.0f - n[yj]*n[yj]*a, -n[yj]);
  }
}

struct material {
  RGB kd = RGB(0.5);
  RGB ks = RGB(0);
  RGB kt = RGB(0);
  bool is_dielectric = false;
  material(){}
};

class Object{
protected:
  Object(){};
  material mtl;
public:
  bool emit = false;
  virtual bool intersection(const Ray& r, float &t, float &dist, Direction& n) = 0;
  virtual void transform(const Matrix &m) = 0;

  virtual material& mt(){
    return mtl;
  }
  virtual material mt() const {
    return mtl;
  }

  virtual void doItSpecial() {}
};

class LightPoint{
public:
  Point point;
  float force;
  LightPoint(Point& p) : point(p){}
};
