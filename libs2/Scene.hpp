#pragma once

#include <vector>
#include <string>
#include <limits>
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
  Camera(Point c_) : Camera(c_, 70, 110, true) {}
  Camera(Point c_, float maxFOV, bool inDegree){
    float r_angle = maxFOV/2;
    if (inDegree) r_angle = r_angle/180*PI;
    origen = c_;
    f = Direction(1,0,0);
    u = Direction(0,cos(r_angle),0);
    l = Direction(0,0,cos(r_angle));
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
    u = Direction(0,cos(u_angle),0);
    l = Direction(0,0,cos(l_angle));
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
  vector<LightPoint> lps;
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
  void addLight(const LightPoint lp){
    lps.push_back(lp);
  }

  void exportImg(string file){
    //out_img.exportLDR(file);
    out_img.exportBitmap(file);
  }
  void exportHDR(string file){
    out_img.exportHDR(file);
  }
  void exportHDR(string file, int csp){
    out_img.exportHDR(file, csp);
  }
  Tone_Mapper getTImg(){
    return Tone_Mapper(out_img);
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
       ray = ray.normalize();
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

  RGB getDirect(const Ray& r){
    Ray r_aux;
    r_aux.orig = r.orig;
    RGB direct(0);
    for (int k = 0; k < lps.size(); k++) {
      r_aux.dir = lps[k].point - r.orig;
      bool contribute = true;
      for (int j = 0; j < objs.size(); j++) {
        float z,zz; Direction zzz;
        if( objs[j]->intersection(r_aux, z,zz,zzz) ){
          contribute = false;
          break;
        }
      }
      if (contribute) {
        Direction d = lps[k].point - r.orig;
        direct = direct + RGB(lps[k].force)/(d[xi]*d[xi] + d[yj]*d[yj] + d[zk]*d[zk]);
      }
    }
    return direct;
  }

  RGB find_path(const Ray& ray){
    RGB throughput(1);
    RGB direct(0);
    Ray luz_refl = ray;

    while(true){

      float dist_obj = std::numeric_limits<float>::infinity(), d, t;
      Direction n, n0;
      shared_ptr<Object> intersects(nullptr);

      // 1. Rayo incide en objs?
      for (int k = 0; k < objs.size(); k++) {
        // 2. interseccion debe devolver punto y normal
        if( objs[k]->intersection(luz_refl, t, d, n0) ){
          if (dist_obj > d) {
            n = n0;
            dist_obj = d;
            intersects = objs[k]; //cambiar por obtener funcion BRDF
          }
        }
      }
      if (isnan(n[xi])){cerr<< "err nan" << endl; break;}

      // Si no interseccion, fin. Si es emisor, devolvemos ya su emision
      if (intersects==nullptr) return RGB(0); //throughput*0
      if (intersects -> emit) return throughput*(intersects->mt()).kd + direct;

      // 3. Creamos sys_ref (hemiesfera) y mtx cambio coor
      Point o = luz_refl.orig + luz_refl.dir*dist_obj;
      Direction localsys[3];
      localsys[1] = n;
      revisedONB(localsys[1], localsys[0], localsys[2]);
      Matrix to_global(localsys[0], localsys[1], localsys[2], o);

      // 4. Ruleta rusa
      Ray luz_inc;
      luz_inc.orig = o;
      material* mt = &(intersects -> mt());
      if (!mt->is_dielectric) { // 4.0 (Lamb_diff, perf_specular or both)
        // 4.1 Calculo de prob
        float pd = max(mt->kd);
        float ps = max(mt->ks);
        float sum = pd + ps;
        if (sum > 0.9) {
          pd = 0.9*pd/sum;
          ps = 0.9*ps/sum;
          sum = pd + ps;
        }

        //Correccion en punto
        luz_inc.orig = luz_inc.orig + n*0.02;

        // 4.2 RR -> event¿?
        float ev = rand0_1();
        if (ev <= pd) { // diff
          // rand0_1() = (float) rand() / (RAND_MAX) ∈ [0,1]
          float phi = 2*PI*rand0_1(),
                theta = acos(sqrt(1-rand0_1()));
          Direction d0(
            sin(theta)*cos(phi),
            cos(theta),
            sin(theta)*sin(phi)
          );

          luz_inc.dir = to_global*d0;
          throughput = throughput * (mt->kd/pd);
          direct = direct + throughput*getDirect(luz_inc);
        } else if(/*pd < ev  &&*/ ev < (sum)) { // specular
          Direction wo = luz_refl.dir;
          Direction wr = wo - n*2*(dotProduct(wo, n));

          luz_inc.setdirnorm(wr);
          throughput = throughput * (mt->ks/ps);
        } else { // ev_ignored
          // Matar rayo
          return throughput*0;
        }

        // Rayo nuevo => Rayo siguiente
        luz_refl = luz_inc;
      } else { // 4.0 (dielectric)
        // 4.1.1 Calculo de fresnel
        float ks, kt;
        float cosi = dotProduct(luz_refl.dir, n);
        if (cosi<-1) cosi = -1;
        if (cosi> 1) cosi =  1;
        float etai = 1, etat = 1.5;
        if (cosi > 0) { std::swap(etai, etat); }
        // Compute sini using Snell's law
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        // Total internal reflection
        if (sint >= 1) {
            ks = 1;
        }
        else {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            ks = (Rs * Rs + Rp * Rp) / 2;

        }
        kt = 1 - ks;

        // 4.1.2 Calculo de prob
        float ps = ks;
        float pt = kt;
        float sum = ps + pt;
        if (sum > 0.9) {
          ps = 0.9*ps/sum;
          pt = 0.9*pt/sum;
          sum = ps + pt;
        }

        // 4.2 RR -> event¿?
        float ev = rand0_1();
        if (ev <= ps) { // reflection     ev <= ps
          Direction wo = luz_refl.dir;
          Direction wr = wo - n*2*(dotProduct(wo, n));

          luz_inc.dir = wr;
          throughput = throughput * (ks/ps);
          //Correccion en punto
          luz_inc.orig = luz_inc.orig + n*0.02;
          luz_refl = luz_inc;
        } else if(ps < ev  && ev < sum) { // refrac ps < ev  && ev < (sum)
          Direction wo = luz_refl.dir;
          float cosi = dotProduct(wo, n);
          if (cosi<-1) cosi = -1;
          if (cosi> 1) cosi =  1;
          float etai = 1, etat = 1.5;
          Direction n2 = n;
          if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n2= neg(n); }
          float eta = etai / etat;
          float k = 1 - eta * eta * (1 - cosi * cosi);
          Direction wt;
          if (k < 0){wt = Direction(0,0,0);}
          else{wt = (wo*eta) + (n2*(eta * cosi - sqrtf(k)));}

          luz_inc.dir = wt;
          throughput = throughput * (kt/pt);
          //Correccion en punto
          luz_inc.orig = luz_inc.orig + neg(n2)*0.02;
          luz_refl = luz_inc;
        } else { // ev_ignored
          // Matar rayo
          return throughput*0;
        }
      }
    }

    // No deberia llegar aqui nunca
    cout << "- - SOCORRO - -" << endl;
    // 5. return acumm
    return throughput;
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
