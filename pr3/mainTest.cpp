
#include <iostream>
#include <memory>
#include <ctime>

#include "Scene.hpp"
#include "2DObjects.hpp"
#include "GeOS.hpp"

//g++ -std=c++11 -I ../libs2 mainTest.cpp -O3 -o main

using namespace std;

int main(){

  Camera c(Point(-10, 10, 0), Direction(0,1,0), Direction(0,0,1), Direction(10,0,0));

  Scene scn(c);
  shared_ptr<Object> x ( new Plane(Point(20,0,0), Direction(-1,0,0)) );         x->setRGB(RGB(255,0,0));
  shared_ptr<Object> y ( new Plane(Point(0,20,0), Direction(0,-1,0)) );         y->setRGB(RGB(0,255,0));
  shared_ptr<Object> yneg ( new Plane(Point(0,-20,0), Direction(0,1,0)) );   yneg->setRGB(RGB(64,255,64));
  shared_ptr<Object> z ( new Plane(Point(0,0,20), Direction(0,0,-1)) );         z->setRGB(RGB(0,0,255));
  shared_ptr<Object> zneg ( new Plane(Point(0,0,-20), Direction(0,0,1)) );   zneg->setRGB(RGB(64,64,255));

  scn.addObj(x);
  scn.addObj(y);
  scn.addObj(yneg);
  scn.addObj(z);
  scn.addObj(zneg);

  scn.RayTracing1rppx(1024, 768);
  scn.exportImg("loqsea.ppm");

}
