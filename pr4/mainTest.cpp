
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

  //Camera c(Point(-3, 0, 0), Direction(0,0,1), Direction(0,1,0), Direction(1,0,0));
  Camera c(Point(-14, 0, 0), 140, true);
  //Camera c(Point(-14, 0, 0));

  Scene scn(c);
  shared_ptr<Object> pared_fr (new FinitePlane( Point(4,0,0) , Direction(0,6,0), Direction(0,0,6) ));  pared_fr->mt().kd = RGB255(55, 156, 156);
  shared_ptr<Object> pared_iz (new FinitePlane( Point(0,0,4) , Direction(6,0,0), Direction(0,6,0) ));  pared_iz->mt().kd = RGB255(173, 207, 116);
  shared_ptr<Object> pared_de (new FinitePlane( Point(0,0,-4), Direction(6,0,0), Direction(0,6,0) ));  pared_de->mt().kd = RGB255(116, 116, 207);
  shared_ptr<Object> pared_up (new FinitePlane( Point(0,4,0) , Direction(6,0,0), Direction(0,0,6) ));  pared_up->mt().kd = RGB255(204, 74, 74);
  shared_ptr<Object> pared_do (new FinitePlane( Point(0,-4,0), Direction(6,0,0), Direction(0,0,6) ));  pared_do->mt().kd = RGB255(156, 156, 156);
  pared_do->mt().kd = RGB(0); pared_do->mt().ks = RGB(0.8);
  shared_ptr<Object> luz_up (new FinitePlane( Point(0,3.9,0) , Direction(2,0,0), Direction(0,0,2) ));
  pared_up->mt().kd = RGB(5); pared_up->emit = true;

  shared_ptr<Object> pelota1 ( new Pelota( Point(2, -3, 3), 1) );   pelota1->mt().kd = RGB255(204, 55, 55);
  shared_ptr<Object> pelota2 ( new Pelota( Point(3, -2, -2), 1) );  pelota2->mt().kd = RGB255(55, 204, 55); //pelota2->mt().ks = RGB(0.2);
  shared_ptr<Object> pelota3 ( new Pelota( Point(3, 2, 1), 1) );    pelota3->mt().kd = RGB255(55, 55, 204);

  scn.addObj(pared_fr);
  scn.addObj(pared_iz);
  scn.addObj(pared_de);
  scn.addObj(pared_up);
  scn.addObj(pared_do);
  //scn.addObj(luz_up);

  scn.addObj(pelota1);
  scn.addObj(pelota2);
  scn.addObj(pelota3);


  t0 = clock();
  scn.PathTracing(800, 800, 80);
  //scn.PathTracing(600, 600, 40);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "RT: " << read_time << " segundos" << endl;

  t0 = clock();
  Tone_Mapper tm = scn.getTImg();
  tm.apply_tone_mapper(clamp_gamma, tm.getMax()*7/8, 0.6);
  tm.exportBMP("render");
  //scn.exportImg("loqsea.bpm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "Tone mapping && Export: " << read_time << " segundos" << endl;
}
