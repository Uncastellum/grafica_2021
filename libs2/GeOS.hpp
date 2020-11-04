#pragma once

//Compile test: g++ -std=c++11 -I. GeOS.hpp -O3 -o efe.o

#include <iostream>
#include <cmath>

using namespace std;

enum GeomComponent { xi = 0, yj = 1, zk = 2 };
class Matrix;

class GeomObj {
protected:
  float g[4];
  GeomObj(){
    for (int i = 0; i < 4; i++) g[i] = 0;
  }
  GeomObj(float x, float y, float z){
    g[0] = x; g[1] = y; g[2] = z;
  }
  friend class Matrix;
public:
  bool isPoint() const {
    return g[3] == 1;
  }
  bool isDirection() const {
    return g[3] == 0;
  }
  float& operator[](GeomComponent c){
    switch (c) {
      case xi:
        return g[0];
      case yj:
        return g[1];
      case zk:
        return g[2];
    }
  }
  float operator[](GeomComponent c) const {
    switch (c) {
      case xi:
        return g[0];
      case yj:
        return g[1];
      case zk:
        return g[2];
      default:
        return -888;
    }
  }
  float getxi() const {
    return g[0];
  }
  float getyj() const {
    return g[1];
  }
  float getzk() const {
    return g[2];
  }
  float getType() const {
    return g[3];
  }
  void setxi(int in){
    g[0] = in;
  }
  void setyj(int in){
    g[1] = in;
  }
  void setzk(int in){
    g[2] = in;
  }
};

class Direction : public GeomObj {
public:
  Direction() : GeomObj() {
    g[3] = 0;
  }
  Direction(float x, float y, float z) : GeomObj(x, y, z) {
    g[3] = 0;
  }

  Direction operator-(const Direction& other) const {
    return Direction(g[0] - other.g[0], g[1] - other.g[1], g[2] - other.g[2]);
  }
  Direction operator+(const Direction& other) const {
    return Direction(g[0] + other.g[0], g[1] + other.g[1], g[2] + other.g[2]);
  }
  Direction operator*(float escalar) const {
    return Direction(g[0]*escalar, g[1]*escalar, g[2]*escalar);
  }
  Direction operator/(float escalar) const {
    return Direction(g[0]/escalar, g[1]/escalar, g[2]/escalar);
  }
  float modulus() const {
    return sqrt( g[0]*g[0] + g[1]*g[1] + g[2]*g[2]);
  }
  Direction normalize() {
    float mod = modulus();
    if (mod != 0){
      g[0] /= mod;
      g[1] /= mod;
      g[2] /= mod;
    }
    return *this;
  }
};

class Point : public GeomObj {
public:
  Point() : GeomObj() {
    g[3] = 1;
  }
  Point(float x, float y, float z) : GeomObj(x, y, z) {
    g[3] = 1;
  }

  Direction operator-(const Point& other) const {
    return Direction(g[0] - other.g[0], g[1] - other.g[1], g[2] - other.g[2]);
  }
  Point operator+(const Direction& other) const {
    return Point(g[0] + other[xi], g[1] + other[yj], g[2] + other[zk]);
  }
  Direction to_dir() const {
    return Direction(g[0],g[1],g[2]);
  }
};



float dotProduct(const Direction& a, const Direction& b) {
  return a[xi]*b[xi] + a[yj]*b[yj] + a[zk]*b[zk];
}
Direction crossProduct(const Direction& a, const Direction& b) {
  float i = a[yj]*b[zk] - a[zk]*b[yj],
         j = a[zk]*b[xi] - a[xi]*b[zk],
         k = a[xi]*b[yj] - a[yj]*b[xi];
  return Direction(i,j,k);
}

void paint(GeomObj v){
  switch ((int) v.getType()) {
    case 0:
      printf("d(");
      break;
    case 1:
      printf("p(");
      break;
    default:
      printf("Unknown(");
      break;
  }
  printf("%.3f  ", v.getxi());
  printf("%.3f  ", v.getyj());
  printf("%.3f)", v.getzk());
}


struct RGB {
  float red, green, blue;
  RGB(){}
  RGB(float r, float g, float b) : red(r), green(g), blue(b) {};
};

void paint(RGB t){
  cout << "(" << t.red << ", " << t.green << ", " << t.blue << ")";
}

class Object{
protected:
  RGB solid_color;
  Object(){};
public:
  virtual bool intersection(const Direction& ray, const Point& origen, float &dist) = 0;

  void setRGB(RGB sc){
    solid_color = sc;
  }
  RGB getSolid(){
    return solid_color;
  }
};
