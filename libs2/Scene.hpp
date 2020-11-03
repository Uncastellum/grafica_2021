#pragma once

#include <vector>
#include <string>
#include <memory>

#include <cassert>
#include <ctime>
#include <cstdlib>


#include "GeOS.hpp"
#include "Image.hpp"
//#include "Matrix.hpp"

//Compile test: g++ -std=c++11 -I. Scene.hpp -O3 -o efe.o
using namespace std;

class Scene;

class Camera {
private:
  Point origen;
  Direction u,l,f;
  friend class Scene;

public:
  Camera(Point c_, Direction u_, Direction l_, Direction f_) {
    srand(time(NULL));
    origen = c_;
    u = u_.normalize(); l = l_.normalize();
    f = f_;
  }

  Direction getRaypp(const int x, const int y, const int p_i, const int p_j) const {
    assert(!(p_i >= x || p_j >= y));
    float c_x = 1.0 - 2.0/x*(p_i + 0.5),
           c_y = 1.0 - 2.0/y*(p_j + 0.5);
    //printf("%.5f  ", c_x); printf("%.5f --- ", c_y);
    return (origen + f + l*c_x + u*c_y) - origen;
  };
  Direction getRandomRaypp(const int x, const int y, const int p_i, const int p_j) const {
    assert(!(p_i >= x || p_j >= y));
    float c_x = 1 - 2/x*(p_i + ((float) rand() / (RAND_MAX)) ),
          c_y = 1 - 2/y*(p_j + ((float) rand() / (RAND_MAX)) );
    return origen - (origen + u*c_y + l*c_x + f);
  };
};


class Scene {
private:
  Camera c;
  vector<shared_ptr<Object>> objs;
  Image out_img;

public:
  Scene(const Camera& cam) : c(cam) {}

  void setCamera(Camera c_) {
    c = c_;
  }
  Camera getCamera() {
    return c;
  }

  void addObj(const shared_ptr<Object>& obj) {
    objs.push_back(obj);
  }
  void exportImg(string file){
    out_img.exportRay(file);
  }

  void RayTracing1rppx(int x, int y){
   out_img = Image(x,y);
   //#pragma omp parallel for schedule(dynamic,1)
   for (int i = 0; i < x; i++) {
     //#pragma omp parallel for schedule(dynamic,1)
     for (int j = 0; j < y; j++) {
       Direction ray = (c.origen + c.f + (c.l*((x - i + 0.5)/x)) + (c.u*((y - j + 0.5)/y))) - c.origen;
       //cout<<i<<" "<<j<<endl;
       //paint(c.u*0.5);
       float min_choque_dist=50;
       int choques=0;
       RGB color;
       for (int k = 0; k < objs.size(); k++) {
         float choque_dist;
         //min_choque_dist=50;
         if(objs[k]->intersection(ray, c.origen, choque_dist)){ //comprobamos interseccion
           choques++;
           //cout<<choque_dist<<endl;
           if(choques==1||choque_dist<min_choque_dist){ //comprobamos distancia
             min_choque_dist = choque_dist;
             color = objs[k]->getSolid();
           }
         }
       }
       if (choques == 0) color = RGB(0,0,0);
       out_img(i,j) = color;
     }
   }
 }

  void RayTracing1rppx2(const int x, const int y){
    out_img = Image(x,y);
    Direction ray;
    //#pragma omp parallel for schedule(dynamic,1)
    for (int i = 0; i < x; i++) {
      //#pragma omp parallel for schedule(dynamic,1)
      for (int j = 0; j < y; j++) {
        ray = c.getRaypp(x,y,i,j);
        //printf("( %.3f  ", ray.getxi()); cout << ", ";
        //printf("%.3f  ", ray.getyj()); cout << ", ";
        //printf("%.3f  ", ray.getzk()); cout << " ) - ";
        float dist_obj = -1, d = 0;
        RGB color(189,189,189);
        shared_ptr<Object> p;

        for (int k = 0; k < objs.size(); k++) {
          if( objs[k]->intersection(ray, c.origen, d) ){ //comprobamos interseccion
            if (dist_obj == -1 || dist_obj > d) {
              dist_obj = d;
              p = objs[k];
            }
          }
        }
        cout << dist_obj << ", " << d << ", "; print(p->getSolid()); cout << endl;

        if (!dist_obj == -1) color = p->getSolid();
        out_img(i,j) = color;
      }
    }
  }
};
