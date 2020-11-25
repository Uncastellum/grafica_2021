

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
  Direction dir;
  /*class {
    public:
      Direction cast;
      Direction not_normalized;
      Direction & operator = (const Direction &i) {
        not_normalized = i;
        cast = i; cast.normalize();
        return cast;
      }
      operator Direction () const { return cast; }
  } dir;*/
  Ray(){}
  Ray(Point o, Direction d) /*: orig(o), dir(d)*/ { orig = o; dir = d;}
};

void paint(Ray r){
  printf("Ray(");
  paint(r.orig);
  printf(",");
  paint(r.dir);
  printf(")");
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
  RGB operator/(float i){
    return RGB(red/i, green/i, blue/i);
  }
};
RGB RGB255(float r, float g, float b) {
  return RGB(r/255, g/255, b/255);
}

float max(const RGB& c) {
  if (c.red > c.green && c.red > c.blue) return c.red;
  if (c.green > c.blue) return c.green;
  return c.blue;
}

void paint(RGB t){
  cout << "(" << t.red << ", " << t.green << ", " << t.blue << ")";
}

struct material {
  RGB kd = RGB(0.5);
  RGB ks = RGB(0);
  bool is_dielectric = false;
};

class Object{
protected:
  RGB solid_color;
  Object(){};
public:
  bool emit = false;
  material mt;
  virtual bool intersection(const Ray& r, float &t, float &dist, Direction& n) = 0;
  virtual void transform(const Matrix &m) = 0;

  void setRGB(RGB sc){
    solid_color = sc;
  }
  RGB getSolid(){
    return solid_color;
  }


  virtual void doItSpecial() {}
};
