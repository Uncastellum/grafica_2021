
#include <iostream>
#include <memory>
#include <ctime>

#include "BasicsRender.hpp"
#include "Matrix.hpp"
#include "2DObjects.hpp"
#include "3DObjects.hpp"
#include "PLYObject.hpp"
#include "Scene.hpp"

//g++ -std=c++11 -I ../libs2 mainTest.cpp -O3 -fopenmp -o main

using namespace std;

int main(){
  unsigned t0, t1;
  double read_time;

  Camera c(Point(-3, 0, 0), Direction(0,0,1), Direction(0,1,0), Direction(0.85,0,0));

  Scene scn(c);
  shared_ptr<Object> pared_fr ( new Plane(Point(2,0,0), Direction(1,0,0)) );    pared_fr ->setRGB(RGB255(156, 156, 156));
  shared_ptr<Object> pared_iz ( new Plane(Point(5,3,0), Direction(0,1,0)) );    pared_iz ->setRGB(RGB255(173, 207, 116));
  shared_ptr<Object> pared_de ( new Plane(Point(5,-3,0), Direction(0,-1,0)) );  pared_de ->setRGB(RGB255(116, 169, 207));
  shared_ptr<Object> pared_up ( new Plane(Point(5,0,3), Direction(0,0,1)) );    pared_up ->setRGB(RGB255(255, 74, 74));
  shared_ptr<Object> pared_do ( new Plane(Point(5,0,-3), Direction(0,0,-1)) );  pared_do ->setRGB(RGB255(156, 156, 156));

  shared_ptr<Object> pelota ( new Pelota(Point(2.2,0.3,0), 0.5) );  pelota ->setRGB(RGB255(157, 250, 165));
  shared_ptr<Object> pelotaa ( new Pelota(Point(2.2,1.3,0), 1) );  pelotaa ->setRGB(RGB255(94, 133, 106));
  shared_ptr<Object> pelota2 ( new Pelota(Point(4.2,0,3), 1) );  pelota2 ->setRGB(RGB255(250, 182, 157));
  shared_ptr<Object> pelota22 ( new Pelota(Point(4.2,1,3), 1) );  pelota22 ->setRGB(RGB255(94, 133, 106));
  shared_ptr<Object> pelota3 ( new Pelota(Point(2.2,-3,0), 1) );  pelota3 ->setRGB(RGB255(173, 69, 31));

  shared_ptr<Object> fp ( new FinitePlane(Point(0,1,1), Direction(0,0.5,0), Direction(0,0,0.5)) );    fp ->setRGB(RGB255(64, 64, 64));

  scn.addObj(pared_fr);
  scn.addObj(pared_iz);
  scn.addObj(pared_de);
  scn.addObj(pared_up);
  scn.addObj(pared_do);

  scn.addObj(pelota);
  /*
  scn.addObj(pelotaa);
  scn.addObj(pelota2);
  scn.addObj(pelota22);
  scn.addObj(pelota3);
  scn.addObj(fp);
  */

  /*
  t0 = clock();
  scn.RayTracing1rppx(800, 800);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "RT1rppx_v1: " << read_time << " segundos" << endl;

  */
  t0 = clock();
  scn.RayTracing(800, 800, 1);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "RT: " << read_time << " segundos" << endl;

  t0 = clock();
  scn.exportImg("loqsea.bpm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "Export: " << read_time << " segundos" << endl;
}
