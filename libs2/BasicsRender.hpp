

#pragma once

//Compile test: g++ -std=c++11 -I. BasicsRender.hpp -O3 -o efe.o

#include <iostream>
#include <cmath>
#include "GeOS.hpp"
#include "Matrix.hpp"

using namespace std;

struct Ray {
  Point orig;
  Direction dir;
  Ray(){}
  Ray(Point o, Direction d) : orig(o), dir(d) {}
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
  RGB(){}
  RGB(float r, float g, float b) : red(r), green(g), blue(b) {}
};
RGB RGB255(float r, float g, float b) {
  return RGB(r/255, g/255, b/255);
}

void paint(RGB t){
  cout << "(" << t.red << ", " << t.green << ", " << t.blue << ")";
}

class Object{
protected:
  RGB solid_color;
  Object(){};
public:
  virtual bool intersection(const Ray& r, float &t, float &dist) = 0;
  virtual void transform(const Matrix &m) = 0;

  void setRGB(RGB sc){
    solid_color = sc;
  }
  RGB getSolid(){
    return solid_color;
  }

  virtual void doItSpecial() {}
};
