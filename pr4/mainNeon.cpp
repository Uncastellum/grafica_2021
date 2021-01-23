
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

  shared_ptr<Object> p26 ( new Pelota( Point(-2, 1, 0), 1) );  p26->mt().is_dielectric = true;
  shared_ptr<Object> p27 ( new Pelota( Point(-2, -1, 0), 1) );  p27->mt().is_dielectric = true;

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


  // ESCENA 2
  Camera c2(Point(-14, -0.4, 0), 140, true);
  Scene scn2(c2);
  shared_ptr<Object> pared_iz (new FinitePlane( Point(0,0,4) , Direction(6,0,0), Direction(0,6,0) ));  pared_iz->mt().kd = RGB255(116, 116, 207); pared_iz->mt().ks = RGB(0.5);
  scn2.addObj(pared_iz);
  shared_ptr<Object> r1 ( new Pelota( Point(1, -4, 3), 1.5) );            r1->mt().kd = RGB255(89, 171, 244);
  shared_ptr<Object> r2 ( new Pelota( Point(0.8, 4, 3.2), 1.3) );         r2->mt().kd = RGB255(104, 224, 117);
  shared_ptr<Object> r3 ( new Pelota( Point(0, -4, -4), 1.2) );           r3->mt().kd = RGB255(109, 48, 214);
  shared_ptr<Object> r4 ( new Pelota( Point(0.6, 4, -3.5), 1.8) );        r4->mt().kd = RGB255(85, 106, 207);

  shared_ptr<Object> r5 ( new Pelota( Point(2.2, 2, -2.2), 1.5) );        r5->mt().kd = RGB255(12, 66, 33);
  shared_ptr<Object> r6 ( new Pelota( Point(2, -4.3, -2.4), 1.5) );       r6->mt().kd = RGB255(50, 228, 214);
  shared_ptr<Object> r7 ( new Pelota( Point(1, -2.4, -3.2), 1.3) );       r7->mt().kd = RGB255(93, 239, 100);
  shared_ptr<Object> r8 ( new Pelota( Point(5, -2, -13.5), 10) );         r8->mt().kd = RGB255(31, 244, 199);
  shared_ptr<Object> r9 ( new Pelota( Point(2.3, -0.2, -3), 1.3) );       r9->mt().kd = RGB255(49, 109, 209);
  shared_ptr<Object> r10 ( new Pelota( Point(1.4, 1.6, -4.4), 1.3) );    r10->mt().kd = RGB255(90, 191, 122);
  shared_ptr<Object> r11 ( new Pelota( Point(1, -0.5, -4.4), 1) );       r11->mt().kd = RGB255(124, 17, 104);

  shared_ptr<Object> r12 ( new Pelota( Point(2.3, 1.9, 3), 1.8) );       r12->mt().kd = RGB255(241, 25, 66);
  shared_ptr<Object> r13 ( new Pelota( Point(1.8, -2, 3.4), 1.3) );      r13->mt().kd = RGB255(84, 10, 134);
  shared_ptr<Object> r14 ( new Pelota( Point(1.3, 0, 3.3), 1) );         r14->mt().kd = RGB255(206, 110, 34);

  shared_ptr<Object> r15 ( new Pelota( Point(2, 2, 0.2), 1.2) );         r15->mt().kd = RGB255(107, 5, 2);
  shared_ptr<Object> r16 ( new Pelota( Point(1.1, 3.8, -0.8), 1.2) );    r16->mt().kd = RGB255(201, 194, 167);
  shared_ptr<Object> r17 ( new Pelota( Point(1.6, 4, 1.2), 1.5) );       r17->mt().kd = RGB255(45, 153, 211);

  shared_ptr<Object> r18 ( new Pelota( Point(2.4, -2, -1.4), 1.2) );     r18->mt().kd = RGB255(27, 150, 42);
  shared_ptr<Object> r19 ( new Pelota( Point(3.1, 0, -0.8), 1.5) );      r19->mt().kd = RGB255(97, 135, 134);
  shared_ptr<Object> r20 ( new Pelota( Point(2.8, -0.4, 1.8), 1.5) );    r20->mt().kd = RGB255(180, 27, 5)*2;
  shared_ptr<Object> r21 ( new Pelota( Point(1.8, 0.5, 1), 0.8) );       r21->mt().kd = RGB255(45, 161, 203);

  shared_ptr<Object> r22 ( new Pelota( Point(3.2, -2.2, 1.2), 1.6) );    r22->mt().kd = RGB255(124, 9, 60);
  shared_ptr<Object> r23 ( new Pelota( Point(2.5, -3.6, -0.3), 1.2) );   r23->mt().kd = RGB255(45, 196, 33);
  shared_ptr<Object> r24 ( new Pelota( Point(2, -4.4, 1.7), 1.3) );      r24->mt().kd = RGB255(45, 71, 36);
  shared_ptr<Object> r25 ( new Pelota( Point(1.3, -5, -0.2), 1) );       r25->mt().kd = RGB255(59, 36, 42);

  scn2.addObj(r1); scn2.addObj(r2); scn2.addObj(r3); scn2.addObj(r4);
  scn2.addObj(r5); scn2.addObj(r6); scn2.addObj(r7); scn2.addObj(r8);
  scn2.addObj(r9); scn2.addObj(r10); scn2.addObj(r11);
  scn2.addObj(r12); scn2.addObj(r13); scn2.addObj(r14);
  scn2.addObj(r15); scn2.addObj(r16); scn2.addObj(r17);
  scn2.addObj(r18); scn2.addObj(r19); scn2.addObj(r20); scn2.addObj(r21);
  scn2.addObj(r22); scn2.addObj(r23); scn2.addObj(r24); scn2.addObj(r25);

  shared_ptr<Object> r_up (new FinitePlane( Point(-2,6,0) , Direction(6,0,0), Direction(0,0,6) ));  r_up->mt().kd = RGB255(204, 74, 74);
  shared_ptr<Object> r_fr (new FinitePlane( Point(-5,6,0) , Direction(3,2,0), Direction(0,0,3) ));
  r_up->mt().kd = RGB(0.5); r_up->emit = true; r_fr->mt().kd = RGB(1); r_fr->emit = true;

  scn2.addObj(r_up);
  scn2.addObj(r_fr);

  t0 = clock();
  scn.PathTracing(800, 800, 80);
  scn2.PathTracing(800, 800, 1);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "PT_1: " << read_time << " segundos" << endl;

  t0 = clock();

  Tone_Mapper tm1 = scn.getTonerImg();
  Tone_Mapper tm2 = scn2.getTonerImg();
  tm1.apply_tone_mapper(clamp_gamma, 255, 0.6);
  tm2.apply_tone_mapper(clamp_gamma, 255, 0.6);
  tm1.exportBMP("n1");
  tm2.exportBMP("n2");
  fusion(tm1.getImage(),tm2.getImage()).exportBitmap("neon");

  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "Tone mapping && Export: " << read_time << " segundos" << endl;
}
