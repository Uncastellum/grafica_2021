
#include <iostream>
#include <memory>
#include <ctime>

#include "Scene.hpp"
#include "2DObjects.hpp"
#include "GeOS.hpp"

//g++ -std=c++11 -I ../libs2 mainTest.cpp -O3 -o main

using namespace std;

int main(){
  unsigned t0, t1;
  double read_time;

  Camera c(Point(0, 0, 0), Direction(0,0,1), Direction(0,1,0), Direction(0.7,0,0));

  Scene scn(c);
  shared_ptr<Object> pared_fr ( new Plane(Point(5,0,0), Direction(1,0,0)) );    pared_fr ->setRGB(RGB(255, 20, 20));
  shared_ptr<Object> pared_iz ( new Plane(Point(5,3,0), Direction(0,1,0)) );    pared_iz ->setRGB(RGB(20, 255, 20));
  shared_ptr<Object> pared_de ( new Plane(Point(5,-3,0), Direction(0,-1,0)) );  pared_de ->setRGB(RGB(90, 255, 90));
  shared_ptr<Object> pared_up ( new Plane(Point(5,0,3), Direction(0,0,1)) );    pared_up ->setRGB(RGB(20, 20, 255));
  shared_ptr<Object> pared_do ( new Plane(Point(5,0,-3), Direction(0,0,-1)) );  pared_do ->setRGB(RGB(90, 90, 255));

  scn.addObj(pared_fr);
  scn.addObj(pared_iz);
  scn.addObj(pared_de);
  scn.addObj(pared_up);
  scn.addObj(pared_do);

  t0 = clock();
  scn.RayTracing1rppx(400, 400);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "RT_v1: " << read_time << " segundos" << endl;

  /*t0 = clock();
  scn.RayTracing1rppx2(400, 400);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "RT_v2: " << read_time << " segundos" << endl;*/


  t0 = clock();
  scn.exportImg("loqsea.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "Export: " << read_time << " segundos" << endl;
}
