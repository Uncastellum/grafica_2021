#pragma once

//Compile test: g++ -std=c++11 -I. Sphere.hpp -O3 -o efe.o

#include <iostream>
#include <cmath>
#include "GeOS.hpp"
#include "Matrix.hpp"

using namespace std;

#define PI 3.1415926535
#define RAD_DEG270 PI*3/2
#define RAD_DEG180 PI/1
#define RAD_DEG90 PI/2
#define RAD_DEG45 PI/4
#define RAD_DEG30 PI/6

// MINI PARCHE ANTI COMPILADOR
class Sphere;
class SpherePoint;

class Sphere {
private:
  struct sref {
  public:
    Direction u,v,w;
  } sys_ref;
  Point center, city;
  Direction axis;
  double radius;
  friend class SpherePoint;

public:
  Sphere(){}
  Sphere(const Point& center_, const Direction& axis_, const Point& city_){
    radius = axis_.modulus() / 2;
    bool correct = fabs((city_ - center_).modulus() - radius) <= 1e-6;
    if (correct) {
        center = center_;
        axis = axis_;
        city = city_;
        sys_ref.v = axis.normalize();   // == sys_ref.v = (axis*0.5).normalize();
        sys_ref.u = crossProduct(city-center, sys_ref.v).normalize();
        sys_ref.w = crossProduct(sys_ref.u, sys_ref.v).normalize();
    } else {
        cout << "Planeta no coherente" << endl;
    }
  }
  Point getCenter() const {
    return center;
  }
  Direction getAxis() const {
    return axis;
  }
  Point getCity() const {
    return city;
  }
  double getRadius() const {
    return radius;
  }
};

class SpherePoint {
private:
  Point l_coor; // LOCAL COOR
  Sphere sph; // coor ϵ sph surface / sph surface contains coor
  friend class Sphere;

public:
  SpherePoint(){}
  SpherePoint(const Sphere& sh, const Point coor_){
    l_coor = coor_; // coor ϵ sph surface / sph surface contains coor
    sph = sh;
  }
  Point getLocalCoor() const {
    return l_coor;
  }
  Point getCoor() const {
    Matrix to_ucs = Matrix(sph.sys_ref.u, sph.sys_ref.v, sph.sys_ref.w, sph.center);
    return to_ucs*l_coor;
  }
  Direction getNormal() const {
    return (getCoor()-sph.center).normalize();
  }
  Direction getConexion(const SpherePoint station) const {
    Point a = getCoor();
    Point b = station.getCoor();
    //VecPun c = VecPun( b[xi]-a[xi], b[yj]-a[yj], b[zk]-a[zk], false);
    Direction c = b-a;
    double choca_a = dotProduct(c,getNormal());
    double choca_b = dotProduct(c,station.getNormal());
    if(choca_a<0) cout<<"La conexion atraviesa el primer planeta"<<endl;
    if(choca_b>0) cout<<"La conexion atraviesa el segundo planeta"<<endl;
    return c;
  }
  void getTangs(Direction &lat, Direction &lon) const {
    Direction normal = getNormal();
    lon = crossProduct(normal, sph.axis).normalize();
    lat = crossProduct(normal, lon).normalize();
  }
};

SpherePoint getPoint(const Sphere& sh, const double azim, const double incl) {
  //VecPun vref = sh.getAxis()*0.5;
  //vref = vref * Matrix(rotate, z_axis, incl) * Matrix(rotate, y_axis, azim);
  //return SpherePoint(sh, sh.getCenter() + vref);
  double radius = sh.getRadius();
  double x = radius * sin(incl) * sin(azim),
         y = radius * sin(incl) * cos(azim),
         z = radius * cos(incl);
  return SpherePoint(sh, Point(x, y, z));
}

SpherePoint getPoint_degree(const Sphere& sh, const double azim, const double incl) {
  double azim_r = azim/180*PI,
         incl_r = incl/180*PI,
         radius = sh.getRadius();
  double x = radius * sin(incl_r) * sin(azim_r),
         y = radius * sin(incl_r) * cos(azim_r),
         z = radius * cos(incl_r);
  return SpherePoint(sh, Point(x, y, z));
}
