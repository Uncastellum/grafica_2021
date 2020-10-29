#pragma once

#include <vector>
#include <string>
#include <memory>
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
    origen = c_;
    u = u_.normalize(); l = l_.normalize();
    f = f_;
  }
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

  void addObj(shared_ptr<Object> obj) {
    objs.push_back(obj);
  }
  void exportImg(string file){
    out_img.exportLDR(file);
  }

  void RayTracing1rppx(int x, int y){
    out_img = Image(x,y);
    //#pragma omp parallel for schedule(dynamic,1)
    for (int i = 0; i < x; i++) {
      //#pragma omp parallel for schedule(dynamic,1)
      for (int j = 0; j < y; j++) {
        Direction ray = (c.origen + c.f + (c.l*(x-(i + 0.5)/x)) + (c.u*(y-(j + 0.5)/y))) - c.origen;
        float min_choque_dist; double d;
        int choques=0;
        RGB color;
        for (int k = 0; k < objs.size(); k++) {
          float choque_dist;
          if(objs[k]->intersection(ray, c.origen, d)){ //comprobamos interseccion
            choques++;
            if(choques==1||d<min_choque_dist){ //comprobamos distancia
              min_choque_dist = choque_dist;
              color = objs[k]->getSolid();
            }
          }
        }
        if (choques == 0) color = RGB(189,189,189);
        out_img(i,j) = color;
      }
    }
  }
};
