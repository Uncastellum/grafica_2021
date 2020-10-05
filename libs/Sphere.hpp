#pragma once

#include <iostream>
#include <exception>
#include <cmath>
#include "GeOS.hpp"

//#include "Matrix.hpp"

using namespace std;

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
  Sphere(VecPun center_, VecPun axis_, VecPun city_){
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
  double getInclinationRef() const {
    VecPun ref = city-center;
    double factor = dotProduct(axis, ref)/(ref.modulus()*axis.modulus());
    return acos(factor);
  }
  double getAzimuthRef() const {
    VecPun ref = city-center;
    double factor = crossProduct(axis, ref).modulus()/(ref.modulus()*axis.modulus());
    return asin(factor);
  }

};

class SpherePoint {
private:
  VecPun local_coor;
  Sphere sph;
  friend class Sphere;

public:
  SpherePoint(){
  }
  SpherePoint(const Sphere& sh, const VecPun lcoor){
    local_coor = lcoor; //lcoor must be local coordinates based in sh coor system.
    sph = sh;
  }

  VecPun getLocalCoor() const {
    return local_coor;
  }
  VecPun getCoor() const {
    VecPun aux = local_coor;
    aux[xi] += sph.center[xi];
    aux[yj] += sph.center[yj];
    aux[zk] += sph.center[zk];
    return aux;
  }
  VecPun getNormal(const VecPun& coor) const {
    return local_coor-sph.center;
  }
  VecPun getConexion(const SpherePoint station) const {
    VecPun a = getCoor();
    VecPun b = station.getCoor();
    return VecPun(b.getxi()-a.getxi(),b.getyj()-a.getyj(),b.getzk()-a.getzk(),0);
  }
};

SpherePoint getPoint(const Sphere& sh, const double azim, const double incl) {
  double azim_r = sh.getAzimuthRef() - azim,
         radius = sh.getRadius();
  double x = radius * sin(azim_r) * cos(incl),
         y = radius * sin(incl) * cos(azim_r),
         z = radius * cos(azim_r);
  return SpherePoint(sh, VecPun(x, y, z, true));
}
