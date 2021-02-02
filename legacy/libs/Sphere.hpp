#pragma once

//Compile test: g++ -std=c++11 -I. Sphere.hpp -O3 -o efe.o

#include <iostream>
#include <exception>
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
  VecPun center;
  VecPun axis;
  VecPun city;
  friend class SpherePoint;

public:
  Sphere(){
    center = VecPun(0,0,0,1);
    axis = VecPun(2,0,0,0);
    city = VecPun(1,0,0,1);
  }
  Sphere(const VecPun center_, const VecPun axis_, const VecPun city_){
    double radius = axis_.modulus() / 2;
    bool correct = fabs((city_ - center_).modulus() - radius) <= 1e-6;
    if (correct) {
        center = center_;
        axis = axis_;
        city = city_;
    } else {
        cout << "Planeta no coherente" << endl;
    }
  }
  VecPun getCenter() const {
    return center;
  }
  VecPun getAxis() const {
    return axis;
  }
  VecPun getCity() const {
    return city;
  }
  double getRadius() const {
    return axis.modulus() / 2;
  }
  double getInclinationCRef() const {
    VecPun ref = city-center;
    double factor = dotProduct(axis, ref)/(ref.modulus()*axis.modulus());
    return acos(factor);
  }
  /*
  double getAzimuthCRef() const {
    VecPun ref = city-center;
    double factor = crossProduct(axis, ref).modulus()/(ref.modulus()*axis.modulus());
    return asin(factor);
    //return atanf(city[xi]/ -city[zk]);
  }
  */
};

class SpherePoint {
private:
  VecPun coor; // GLOBAL COOR
  Sphere sph; // coor ϵ sph surface / sph surface contains coor
  friend class Sphere;

public:
  SpherePoint(){
  }
  SpherePoint(const Sphere& sh, const VecPun coor_){
    coor = coor_; // coor ϵ sph surface / sph surface contains coor
    sph = sh;
  }

  VecPun getLocalCoor() const {
    // Point (0,0,0) is the center of sph
    VecPun aux = coor;
    aux[xi] -= sph.center[xi];
    aux[yj] -= sph.center[yj];
    aux[zk] -= sph.center[zk];
    return aux;
  }
  VecPun getCoor() const {
    return coor;
  }
  VecPun getNormal() const {
    return coor-sph.center;
  }
  VecPun getConexion(const SpherePoint station) const {
    VecPun a = getCoor();
    VecPun b = station.getCoor();
    VecPun c = VecPun( b[xi]-a[xi], b[yj]-a[yj], b[zk]-a[zk], false);
    double choca_a = dotProduct(c,getNormal());
    double choca_b = dotProduct(c,station.getNormal());
    if(choca_a<0) cout<<"La conexion atraviesa el primer planeta"<<endl;
    if(choca_b>0) cout<<"La conexion atraviesa el segundo planeta"<<endl;
    return c;
  }
  void getTangs(VecPun &lat, VecPun &lon) const {
    VecPun normal = getNormal();
    lon = crossProduct(normal, sph.axis);
    lat = crossProduct(normal, lon);
  }
};

SpherePoint getPoint(const Sphere& sh, const double azim, const double incl) {
  VecPun vref = sh.getAxis()*0.5;
  vref = vref * Matrix(rotate, z_axis, incl) * Matrix(rotate, y_axis, azim);
  return SpherePoint(sh, sh.getCenter() + vref);
  /*
  VecPun center = sh.getCenter();
  double azim_r = sh.getAzimuth-Ref() - azim,
         radius = sh.getRadius();
  double x = center[xi] + radius * sin(incl) * sin(azim_r),
         y = center[yj] + radius * sin(incl) * cos(azim_r),
         z = center[zk] + radius * cos(incl);
  return SpherePoint(sh, VecPun(x, y, z, true)); */
}

SpherePoint getPoint_degree(const Sphere& sh, const double azim, const double incl) {
  double azim_r = azim/180*PI,
         incl_r = incl/180*PI;
  VecPun vref = sh.getAxis()*0.5;
  vref = vref * Matrix(rotate, z_axis, incl) * Matrix(rotate, y_axis, azim);
  return SpherePoint(sh, sh.getCenter() + vref);
}
