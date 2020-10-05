#pragma once

#include <iostream>
#include <exception>
#include <cmath>
#include "GeOS.hpp"

//#include "Matrix.hpp"

using namespace std;

class Sphere {
private:
  VecPun center;
  VecPun axis;
  VecPun city;
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
  double getRadius(){
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
