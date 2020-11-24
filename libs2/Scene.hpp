#pragma once

#include <vector>
#include <string>
#include <memory>

#include <cassert>
#include <ctime>
#include <cstdlib>


#include "BasicsRender.hpp"
#include "Image.hpp"

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

  Ray getRaypp(const int x, const int y, const int p_i, const int p_j) const {
    //assert(!(p_i >= x || p_j >= y));
    int max = (x > y) ? x : y;
    float c_x = x - 2.0*(p_i + 0.5),
           c_y = y - 2.0*(p_j + 0.5);
    return Ray(origen, f + u*(c_y/max) + l*(c_x/max));
  };
  Ray getRandomRaypp(const int x, const int y, const int p_i, const int p_j) const {
    //assert(!(p_i >= x || p_j >= y));
    int max = (x > y) ? x : y;
    float c_x = x - 2.0*(p_i + ((float) rand() / (RAND_MAX))),
          c_y = y - 2.0*(p_j + ((float) rand() / (RAND_MAX)));
    return Ray(origen, f + u*(c_y/max) + l*(c_x/max));
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
    //out_img.exportLDR(file);
    out_img.exportBitmap(file);
  }

  void RayTracing1rppx(int x, int y){
   out_img = Image(x,y);
   Direction aux_l = c.l * (1.0/(x/2));
   Direction aux_u = c.u * (1.0/(x/2));
   #pragma omp parallel for schedule(guided,1)
   for (int i = 0; i < y; i++) {
     #pragma omp parallel for schedule(guided,1)
     for (int j = 0; j < x; j++) {
       Direction ray = (c.origen + c.f + (aux_l * ((x/2)-(j+1))) + aux_l*0.5 + (aux_u * ((y/2)-(i+1))) + aux_u*0.5 ) - c.origen;
       //paint(ray);
       float min_choque_dist, choque_dist, t; Direction n;
       int choques=0;
       RGB color;
       for (int k = 0; k < objs.size(); k++) {
         if(objs[k]->intersection(Ray(c.origen, ray), t, choque_dist, n)){ //comprobamos interseccion
           choques++;
           if(choques==1||choque_dist<min_choque_dist){ //comprobamos distancia
             min_choque_dist = choque_dist;
             color = objs[k]->getSolid();
           }
         }
       }
       if (choques == 0) color = RGB(0,0,0);
       out_img(j,i) = color;
     }
   }
  }

  void RayTracing(const int x, const int y, const int rppx){
    out_img = Image(x,y);

    #pragma omp parallel for schedule(guided,1)
    for (int i = 0; i < x; i++) {

      #pragma omp parallel for schedule(guided,1)
      for (int j = 0; j < y; j++) {

        RGB media(0,0,0);
        for(int nray = 0; nray < rppx; nray++){

          Ray ray = c.getRandomRaypp(x,y,i,j);
          float dist_obj = -1, d = 0, t;
          Direction n;
          RGB color = RGB255(64,64,64);

          for (int k = 0; k < objs.size(); k++) {
            if( objs[k]->intersection(ray, t, d, n) ){ //comprobamos interseccion
              if (dist_obj == -1 || dist_obj > d) {
                dist_obj = d;
                color = objs[k]->getSolid();
              }
            }
          }

          media.red += color.red/rppx;
          media.green += color.green/rppx;
          media.blue += color.blue/rppx;
        }

        out_img(i,j) = media;
      }
    }
  }


  RGB find_path(const Ray& ray, int r){ //COMPLETE
    float dist_obj = -1, d = 0, t = -1;
    Direction n, n0;
    RGB color;

    // 1. Rayo incide en objs?
    for (int k = 0; k < objs.size(); k++) {
      // 2. interseccion debe devolver punto y normal
      if( objs[k]->intersection(ray, t, d, n0) ){
        if (dist_obj == -1 || dist_obj > d) {
          n0 = n;
          dist_obj = d;
          color = objs[k]->getSolid(); //cambiar por obtener funcion BRDF
        }
      }
    }

    if (dist_obj == -1) return RGB(0,0,0);

    // 3. Cresamos sys_ref (hemiesfera)
    Point o = ray.orig + ray.dir*t;
    Direction localsys[3];
    localsys[0] = n;
    localsys[1] = Matrix(rotate,z_axis,90)*n;
    localsys[2] = crossProduct(n, localsys[1]);

    // 4. Phi y tita de forma aleatoria
    // (float) rand() / (RAND_MAX) ∈ [0,1]
    float phi = ((float) rand() / (RAND_MAX)),
          theta = ((float) rand() / (RAND_MAX));
    theta = acos(sqrt(1-theta));
    phi = 2*PI*phi;

    // 5. nuevo vector aleatorio + cambio coor
    Matrix to_global(localsys[0], localsys[1], localsys[2], o);
    Direction d0(
        sin(theta)*sin(phi),
        sin(theta)*cos(phi),
        cos(theta)
      );
    Ray wo(o, to_global*d0);

    // 6. calculo de BRDF(---) ¿+ Ruleta Rusa?


    // 7. return (6.)*funcion_recursiva_pt(--)
    return RGB(0,0,0);
  }

  void PathTracing(const int x, const int y, const int rppx){
    out_img = Image(x,y);

    #pragma omp parallel for schedule(guided,1)
    for (int i = 0; i < x; i++) {

      #pragma omp parallel for schedule(guided,1)
      for (int j = 0; j < y; j++) {
        RGB media(0,0,0);

        for(int nray = 0; nray < rppx; nray++){
          Ray ray = c.getRandomRaypp(x,y,i,j);

          RGB color = find_path(ray, 2); //COMPLETE

          media.red += color.red/rppx;
          media.green += color.green/rppx;
          media.blue += color.blue/rppx;
        }

        out_img(i,j) = media;
      }
    }

  }
};
