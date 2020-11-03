#pragma once

//Compile test: g++ -std=c++11 -I. GeOS.hpp -O3 -o efe.o

#include <iostream>
#include <cmath>

using namespace std;

enum GeomComponent { xi = 0, yj = 1, zk = 2 };
class Matrix;

class GeomObj {
protected:
  double g[4];
  GeomObj(){
    for (int i = 0; i < 4; i++) g[i] = 0;
  }
  GeomObj(int xi, int yj, int zk){
    g[0] = xi; g[1] = yj; g[2] = zk;
  }
  friend class Matrix;
public:
  bool isPoint() const {
    return g[3] == 1;
  }
  bool isDirection() const {
    return g[3] == 0;
  }
  double& operator[](GeomComponent c){
    switch (c) {
      case xi:
        return g[0];
      case yj:
        return g[1];
      case zk:
        return g[2];
    }
  }
  double operator[](GeomComponent c) const {
    switch (c) {
      case xi:
        return g[0];
      case yj:
        return g[1];
      case zk:
        return g[2];
    }
  }
  double getxi() const {
    return g[0];
  }
  double getyj() const {
    return g[1];
  }
  double getzk() const {
    return g[2];
  }
  double getType() const {
    return g[3];
  }
  void setxi(int xi){
    g[0] = xi;
  }
  void setyj(int yj){
    g[1] = yj;
  }
  void setzk(int zk){
    g[2] = zk;
  }
};

class Direction : public GeomObj {
public:
  Direction() : GeomObj() {
    g[3] = 0;
  }
  Direction(int x, int y, int z) : GeomObj(x, y, z) {
    g[3] = 0;
  }

  Direction operator-(const Direction& other) const {
    return Direction(g[0] - other.g[0], g[1] - other.g[1], g[2] - other.g[2]);
  }
  Direction operator+(const Direction& other) const {
    return Direction(g[0] + other.g[0], g[1] + other.g[1], g[2] + other.g[2]);
  }
  Direction operator*(double escalar) const {
    return Direction(g[0]*escalar, g[1]*escalar, g[2]*escalar);
  }
  double modulus() const {
    return sqrt( g[0]*g[0] + g[1]*g[1] + g[2]*g[2]);
  }
  Direction normalize() {
    double mod = modulus();
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
  Point(int x, int y, int z) : GeomObj(x, y, z) {
    g[3] = 1;
  }

  Direction operator-(const Point& other) const {
    return Direction(g[0] - other.g[0], g[1] - other.g[1], g[2] - other.g[2]);
  }
  Point operator+(const Direction& other) const {
    return Point(g[0] + other[xi], g[1] + other[yj], g[2] + other[zk]);
  }
};



double dotProduct(const Direction& a, const Direction& b) {
  return a[xi]*b[xi] + a[yj]*b[yj] + a[zk]*b[zk];
}
Direction crossProduct(const Direction& a, const Direction& b) {
  double i = a[yj]*b[zk] - a[zk]*b[yj],
         j = a[zk]*b[xi] - a[xi]*b[zk],
         k = a[xi]*b[yj] - a[yj]*b[xi];
  return Direction(i,j,k);
}

void paint(GeomObj v){
  printf("%.3f  ", v.getxi());
  printf("%.3f  ", v.getyj());
  printf("%.3f  ", v.getzk());
  printf("%.3f  ", v.getType());
  cout << endl;
}


struct RGB {
  float red, green, blue;
  RGB(){}
  RGB(float r, float g, float b) : red(r), green(g), blue(b) {};
};

void print(RGB t){
  cout << "(" << t.red << ", " << t.green << ", " << t.blue << ")";
}

class Object{
protected:
  RGB solid_color;
  Object(){};
public:
  virtual bool intersection(const Direction& ray, const Point& origen, double &dist) = 0;

  void setRGB(RGB sc){
    solid_color = sc;
  }
  RGB getSolid(){
    return solid_color;
  }
};
