
#include <iostream>
#include <memory>
#include <ctime>

#include "BasicsRender.hpp"
#include "Matrix.hpp"
#include "2DObjects.hpp"
#include "3DObjects.hpp"
#include "PLYObject.hpp"
#include "Scene.hpp"

//g++ -std=c++11 -I ../libs2 mainNeon.cpp -O3 -fopenmp -o main

using namespace std;

int main(){
  unsigned t0, t1;
  double read_time;

  Camera c(Point(-14, 0, 0), 140, true);

  Scene scn(c);
  shared_ptr<Object> pared_de (new FinitePlane( Point(0,0,-4), Direction(6,0,0), Direction(0,6,0) ));  pared_de->mt().kd = RGB255(116, 116, 207); pared_de->mt().ks = RGB(0.5);
  shared_ptr<Object> p1 ( new Pelota( Point(1, 4, -3), 1.5) );            p1->mt().kd = RGB255(89, 171, 244);
  shared_ptr<Object> p2 ( new Pelota( Point(0.8, -4, -3.2), 1.3) );       p2->mt().kd = RGB255(104, 224, 117)*2; p2->emit = true;
  shared_ptr<Object> p3 ( new Pelota( Point(0, 4, 4), 1.2) );             p3->mt().kd = RGB255(109, 48, 214)*2; p3->emit = true;
  shared_ptr<Object> p4 ( new Pelota( Point(0.6, -4, 3.5), 1.8) );        p4->mt().kd = RGB255(85, 106, 207);

  shared_ptr<Object> p5 ( new Pelota( Point(2.2, -2.0, 2.2), 1.5) );      p5->mt().kd = RGB255(12, 66, 33);
  shared_ptr<Object> p6 ( new Pelota( Point(2, 4.3, 2.4), 1.5) );         p6->mt().kd = RGB255(50, 228, 214);
  shared_ptr<Object> p7 ( new Pelota( Point(1, 2.4, 3.2), 1.3) );         p7->mt().kd = RGB255(93, 239, 100);
  shared_ptr<Object> p8 ( new Pelota( Point(5, 2, 13.5), 10) );           p8->mt().kd = RGB255(31, 244, 199);
  shared_ptr<Object> p9 ( new Pelota( Point(2.3, 0.2, 3), 1.3) );         p9->mt().kd = RGB255(49, 109, 209)*2; p9->emit = true;
  shared_ptr<Object> p10 ( new Pelota( Point(1.4, -1.6, 4.4), 1.3) );     p10->mt().kd = RGB255(90, 191, 122);
  shared_ptr<Object> p11 ( new Pelota( Point(1, 0.5, 4.4), 1) );          p11->mt().kd = RGB255(124, 17, 104);

  shared_ptr<Object> p12 ( new Pelota( Point(2.3, -1.9, -3), 1.8) );     p12->mt().kd = RGB255(241, 25, 66);
  shared_ptr<Object> p13 ( new Pelota( Point(1.8, 2, -3.4), 1.3) );      p13->mt().kd = RGB255(84, 10, 134);
  shared_ptr<Object> p14 ( new Pelota( Point(1.3, 0, -3.3), 1) );        p14->mt().kd = RGB255(206, 110, 34);

  shared_ptr<Object> p15 ( new Pelota( Point(2, -2, -0.2), 1.2) );       p15->mt().kd = RGB255(107, 5, 2);
  shared_ptr<Object> p16 ( new Pelota( Point(1.1, -3.8, 0.8), 1.2) );    p16->mt().kd = RGB255(201, 194, 167)*2; p16->emit = true;
  shared_ptr<Object> p17 ( new Pelota( Point(1.6, -4, -1.2), 1.5) );     p17->mt().kd = RGB255(45, 153, 211);

  shared_ptr<Object> p18 ( new Pelota( Point(2.4, 2, 1.4), 1.2) );       p18->mt().kd = RGB255(27, 150, 42);
  shared_ptr<Object> p19 ( new Pelota( Point(3.1, 0, 0.8), 1.5) );       p19->mt().kd = RGB255(97, 135, 134); p19->mt().ks = RGB(0.5);
  shared_ptr<Object> p20 ( new Pelota( Point(2.8, 0.4, -1.8), 1.5) );    p20->mt().kd = RGB255(180, 27, 5)*2; p20->emit = true;
  shared_ptr<Object> p21 ( new Pelota( Point(1.8, -0.5, -1), 0.8) );     p21->mt().kd = RGB255(45, 161, 203);

  shared_ptr<Object> p22 ( new Pelota( Point(3.2, 2.2, -1.2), 1.6) );    p22->mt().kd = RGB255(124, 9, 60);
  shared_ptr<Object> p23 ( new Pelota( Point(2.5, 3.6, 0.3), 1.2) );     p23->mt().kd = RGB255(45, 196, 33)*2; p23->emit = true;
  shared_ptr<Object> p24 ( new Pelota( Point(2, 4.4, -1.7), 1.3) );      p24->mt().kd = RGB255(45, 71, 36);
  shared_ptr<Object> p25 ( new Pelota( Point(1.3, 5, 0.2), 1) );         p25->mt().kd = RGB255(59, 36, 42);

  shared_ptr<Object> p26 ( new Pelota( Point(0, 1, 0), 1) );  p26->mt().is_dielectric = true;
  shared_ptr<Object> p27 ( new Pelota( Point(0, -1, 0), 1) );  p27->mt().is_dielectric = true;

  shared_ptr<Object> pared_up (new FinitePlane( Point(-2,6,0) , Direction(6,0,0), Direction(0,0,6) ));  pared_up->mt().kd = RGB255(204, 74, 74);
  shared_ptr<Object> pared_fr (new FinitePlane( Point(-5,6,0) , Direction(3,2,0), Direction(0,0,3) ));
  pared_up->mt().kd = RGB(0.5); pared_up->emit = true; pared_fr->mt().kd = RGB(1); pared_fr->emit = true;

  scn.addObj(p1); scn.addObj(p2); scn.addObj(p3); scn.addObj(p4);
  scn.addObj(p5); scn.addObj(p6); scn.addObj(p7); scn.addObj(p8);
  scn.addObj(p9); scn.addObj(p10); scn.addObj(p11);
  scn.addObj(p12); scn.addObj(p13); scn.addObj(p14);
  scn.addObj(p15); scn.addObj(p16); scn.addObj(p17);
  scn.addObj(p18); scn.addObj(p19); scn.addObj(p20); scn.addObj(p21);
  scn.addObj(p22); scn.addObj(p23); scn.addObj(p24); scn.addObj(p25);
  scn.addObj(p26); scn.addObj(p27);

  scn.addObj(pared_de);
  scn.addObj(pared_up);
  scn.addObj(pared_fr);


  Scene scn2(c);
  shared_ptr<Object> pared_iz (new FinitePlane( Point(0,0,4) , Direction(6,0,0), Direction(0,6,0) ));  pared_iz->mt().kd = RGB255(116, 116, 207); pared_iz->mt().ks = RGB(0.5);
  scn2.addObj(pared_iz);
  scn2.addObj(pared_up);
  scn2.addObj(pared_fr);


  t0 = clock();
  scn.PathTracing(800, 800, 80);
  scn2.PathTracing(800, 800, 80);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "PT_1: " << read_time << " segundos" << endl;

  t0 = clock();

  Tone_Mapper tm(fusion(scn.getImg(),scn2.getImg()));
  tm.apply_tone_mapper(clamp_gamma, 255, 0.6);
  tm.exportBMP("neon");
  Tone_Mapper tm1 = scn.getTonerImg();
  tm1.apply_tone_mapper(clamp_gamma, 255, 0.6);
  tm1.exportBMP("n1");
  Tone_Mapper tm2 = scn2.getTonerImg();
  tm2.apply_tone_mapper(clamp_gamma, 255, 0.6);
  tm2.exportBMP("n2");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "Tone mapping && Export: " << read_time << " segundos" << endl;
}
