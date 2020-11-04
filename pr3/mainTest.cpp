
#include <iostream>
#include <memory>
#include <ctime>

#include "Scene.hpp"
#include "2DObjects.hpp"
#include "GeOS.hpp"

//g++ -std=c++11 -I ../libs2 mainTest.cpp -O3 -o main

using namespace std;

int main(){

  Camera c(Point(0, 0, 0), Direction(0,0,1), Direction(0,1,0), Direction(0.7,0,0));
  Direction neg = Direction(1,0,-1);

  Scene scn(c);
  shared_ptr<Object> pared_fr ( new Plane(Point(5,0,0), Direction(1,0,0)) );         pared_fr->setRGB(RGB(22, 219, 61));
  shared_ptr<Object> pared_iz ( new Plane(Point(5,3,0), Direction(0,1,0)) );         pared_iz ->setRGB(RGB(111, 61, 191));
  shared_ptr<Object> pared_de ( new Plane(Point(5,-3,0), Direction(0,-1,0)) );         pared_de ->setRGB(RGB(111, 61, 191));
  shared_ptr<Object> pared_up ( new Plane(Point(5,0,3), Direction(0,0,1)) );         pared_up ->setRGB(RGB(219, 196, 22));
  shared_ptr<Object> pared_do ( new Plane(Point(5,0,-3), Direction(0,0,-1)) );         pared_do ->setRGB(RGB(219, 196, 22));

  scn.addObj(pared_fr);
  scn.addObj(pared_iz);
  scn.addObj(pared_de);
  scn.addObj(pared_up);
  scn.addObj(pared_do);

  scn.RayTracing1rppx(400, 400);
  scn.exportImg("loqsea.ppm");

}
