
#include <iostream>
#include <memory>
#include <ctime>

#include "Scene.hpp"
#include "2DObjects.hpp"
#include "GeOS.hpp"

//g++ -std=c++11 -I ../libs2 mainTest.cpp -O3 -fopenmp -o main

using namespace std;

int main(){
  unsigned t0, t1;
  double read_time;

  Camera c(Point(-3, 0, 0), Direction(0,0,1), Direction(0,1,0), Direction(0.85,0,0));

  Scene scn(c);
  shared_ptr<Object> pared_fr ( new Plane(Point(5,0,0), Direction(1,0,0)) );    pared_fr ->setRGB(RGB(255, 20, 20));
  shared_ptr<Object> pared_iz ( new Plane(Point(5,3,0), Direction(0,1,0)) );    pared_iz ->setRGB(RGB(20, 255, 20));
  shared_ptr<Object> pared_de ( new Plane(Point(5,-3,0), Direction(0,-1,0)) );  pared_de ->setRGB(RGB(90, 255, 90));
  shared_ptr<Object> pared_up ( new Plane(Point(5,0,3), Direction(0,0,1)) );    pared_up ->setRGB(RGB(20, 20, 255));
  shared_ptr<Object> pared_do ( new Plane(Point(5,0,-3), Direction(0,0,-1)) );  pared_do ->setRGB(RGB(90, 90, 255));

  shared_ptr<Object> t ( new Triangle(Point(1,0,-3), Point(2,1,-3), Point(2,0,0)) );  t ->setRGB(RGB(66, 245, 227));
  shared_ptr<Object> ti ( new Triangle(Point(1,0,-3), Point(2,-1,-3), Point(2,0,0)) );  ti ->setRGB(RGB(250, 137, 220));

  shared_ptr<Object> pelota ( new Pelota(Point(2.2,0.3,0), 1) );  pelota ->setRGB(RGB(157, 250, 165));
  shared_ptr<Object> pelotaa ( new Pelota(Point(2.2,1.3,0), 1) );  pelotaa ->setRGB(RGB(94, 133, 106));
  shared_ptr<Object> pelota2 ( new Pelota(Point(4.2,0,3), 1) );  pelota2 ->setRGB(RGB(250, 182, 157));
  shared_ptr<Object> pelota22 ( new Pelota(Point(4.2,1,3), 1) );  pelota22 ->setRGB(RGB(94, 133, 106));
  shared_ptr<Object> pelota3 ( new Pelota(Point(2.2,-3,0), 1) );  pelota3 ->setRGB(RGB(173, 69, 31));

  scn.addObj(pared_fr);
  scn.addObj(pared_iz);
  scn.addObj(pared_de);
  scn.addObj(pared_up);
  scn.addObj(pared_do);
  scn.addObj(t);
  scn.addObj(ti);
  scn.addObj(pelota);
  scn.addObj(pelotaa);
  scn.addObj(pelota2);
  scn.addObj(pelota22);
  scn.addObj(pelota3);

  t0 = clock();
  scn.RayTracing1rppx(800, 800);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "RT_v1: " << read_time << " segundos" << endl;

  /*
  t0 = clock();
  scn.RayTracing1rppx2(800, 800);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "RT_v2: " << read_time << " segundos" << endl;
  */

  t0 = clock();
  scn.exportImg("loqsea.bpm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "Export: " << read_time << " segundos" << endl;
}
