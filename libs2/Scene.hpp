#pragma once

#include <vector>
#include <string>
#include <memory>

#include <cassert>


#include "BasicsRender.hpp"
#include "Image.hpp"

//Compile test: g++ -std=c++11 -I. Scene.hpp -O3 -o efe.o
using namespace std;

class Scene;

class Camera {
private:
  Point origen;
  Direction u,l,f;
  Matrix to_global;
  friend class Scene;

public:
  Camera(Point c_) : Camera(c_, 60, 120, true) {}
  Camera(Point c_, float maxFOV, bool inDegree){
    float r_angle = maxFOV/2;
    if (inDegree) r_angle = r_angle/180*PI;
    origen = c_;
    f = Direction(1,0,0);
    l = Direction(0,cos(r_angle),0);
    u = Direction(0,0,cos(r_angle));
    to_global = Matrix(traslate, origen[xi], origen[yj], origen[zk]);
    cout << "Default camera created in "; paint(c_); cout << ": looking x edge" << endl;
    cout << "          '---> FOV max = " << maxFOV << (inDegree? " degrees.":" rads.") << endl;
  }
  Camera(Point c_, float FOVu, float FOVl, bool inDegree){
    float u_angle = FOVu/2;
    float l_angle = FOVl/2;
    if (inDegree) u_angle = u_angle/180*PI;
    if (inDegree) l_angle = l_angle/180*PI;
    origen = c_;
    f = Direction(1,0,0);
    l = Direction(0,cos(l_angle),0);
    u = Direction(0,0,cos(u_angle));
    to_global = Matrix(traslate, origen[xi], origen[yj], origen[zk]);
    cout << "Default camera created in "; paint(c_); cout << ": looking x edge" << endl;
    cout << "         '---> vertical FOV = " << FOVu << ", horiz FOV = " << FOVl
     << (inDegree? " (degrees).":" (rads).") << endl;
  }

  Camera(Point c_, Direction u_, Direction l_, Direction f_) {
    cout << "Custom cam created. -> NOT CHECKED" << endl;
    cout << "DEPRECATED CAMERA!!!" << endl;
    origen = c_;
    u = u_.normalize(); l = l_.normalize();
    //u = u_; l = l_;
    f = f_;
  }

  Ray getRaypp(const int x, const int y, const int p_i, const int p_j) const {
    //assert(!(p_i >= x || p_j >= y));
    int max = (x > y) ? x : y;
    float c_x = x - 2.0*(p_i + 0.5),
           c_y = y - 2.0*(p_j + 0.5);
    Direction dir = f + u*(c_y/max) + l*(c_x/max);
    return Ray(origen, to_global*dir);
  };
  Ray getRandomRaypp(const int x, const int y, const int p_i, const int p_j) const {
    //assert(!(p_i >= x || p_j >= y));
    int max = (x > y) ? x : y;
    float c_x = x - 2.0*(p_i + rand0_1()),
          c_y = y - 2.0*(p_j + rand0_1());
    Direction dir = f + u*(c_y/max) + l*(c_x/max);
    return Ray(origen, to_global*dir);
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
             color = objs[k]->mt().kd;
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
                color = objs[k]->mt().kd;
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


  RGB find_path(const Ray& ray){ //COMPLETE
    RGB resul;
    Ray luz_refl = ray;

    while(true){

      float dist_obj = -1, d = 0, t = -1;
      Direction n, n0;
      shared_ptr<Object> intersects;

      // 1. Rayo incide en objs?
      for (int k = 0; k < objs.size(); k++) {
        // 2. interseccion debe devolver punto y normal
        if( objs[k]->intersection(luz_refl, t, d, n) ){
          if (dist_obj == -1 || dist_obj > d) {
            n0 = n;
            dist_obj = d;
            intersects = objs[k]; //cambiar por obtener funcion BRDF
          }
        }
      }

      if (dist_obj == -1) return resul;
      // Si es emisor, devolvemos ya su emision
      if (intersects -> emit) return resul;

      // 3. Creamos sys_ref (hemiesfera) y mtx cambio coor
      Point o = luz_refl.orig + luz_refl.dir*t;
      Direction localsys[3];
      localsys[0] = n;
      localsys[1] = Matrix(rotate,z_axis,90)*n;
      localsys[2] = crossProduct(n, localsys[1]);
      Matrix to_global(localsys[0], localsys[1], localsys[2], o);

      // 4. Ruleta rusa
      Ray luz_inc;
      luz_inc.orig = o;
      material* mt = &(intersects -> mt());
      if (!mt->is_dielectric) { // 4.0 (Lamb_diff, perf_specular or both)
        // 4.1 Calculo de prob
        float pd = max(mt->kd); pd /= (pd + max(mt->ks));
        float ps = 1 - pd;
        float sum = pd + ps;
        if (sum > 0.9) {
          pd = 0.9*pd/sum;
          ps = 0.9*ps/sum;
        }

        // 4.2 RR -> event¿?
        float ev = rand0_1();
        if (ev <= pd) { // diff
          // rand0_1() = (float) rand() / (RAND_MAX) ∈ [0,1]
          float phi = 2*PI*rand0_1(),
          theta = acos(sqrt(1-rand0_1()));
          Direction d0(
            sin(theta)*sin(phi),
            sin(theta)*cos(phi),
            cos(theta)
          );
          luz_inc.dir = to_global*d0;
          resul = resul * mt->kd;

        } else if(pd < ev  && ev < (ps+pd)) { // specular
          Direction wo = luz_refl.dir; wo.normalize();
          Direction wi = n*2*(dotProduct(n, wo)) + neg(wo);
          luz_inc.dir = wi;
          resul = resul * mt->ks;

        } else { // ev_ignored
          // Matar rayo
          return resul;
        }

        luz_refl = luz_inc;
      } else { // 4.0 (dielectric)


      }

    }

    // 5. return acumm
    return resul;
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

          RGB color = find_path(ray);

          media.red += color.red/rppx;
          media.green += color.green/rppx;
          media.blue += color.blue/rppx;
        }

        out_img(i,j) = media;
      }
    }

  }
};
