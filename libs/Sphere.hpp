#pragma once

#include <iostream>
#include <exception>
#include <cmath>

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
  Sphere(const VecPun &center_, const VecPun &axis_, const VecPun &city_){
    double radius = modulus(axis_) / 2;
    bool correct = fabs(modulus(city_ - center_) - radius) <= 1e-6;
    if (correct)
    {
        center = center_;
        axis = axis_;
        city = city_;
    }
    else
    {
        cout << "Planeta no coherente" << endl;
    }
  }
  VecPun getCenter(){
      return center;
  }
  VecPun getAxis(){
      return axis;
  }
  VecPun getCity(){
      return city;
  }
  double getRadius(){
      return axis.mod() / 2;
  }
  const double getAzimuth(){
      double x = city.getxi();
      double z = city.getzk();
      return atan(x / -z);
  }
};
