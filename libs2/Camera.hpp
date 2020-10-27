

#include <vector>
#include "Matrix.hpp"

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
  vector<Objects> objs;
  Image salida;

public:
  Scene(Camera c_) {
    c = c_;
  }

  void setCamera(Camera c_) {
    c = c_;
  }
  Camera getCamera() {
    return c;
  }

  void addObj(Objects obj) {
    objs.push_back(obj);
  }

  void RayTracing1rppx(int x, int y){
    salida = Image(x,y);
    //#pragma omp parallel for schedule(dynamic,1)
    for (int i = 0; i < x; i++) {
      //#pragma omp parallel for schedule(dynamic,1)
      for (int j = 0; j < y; j++) {
        Direction ray = (c.origen + c.f + (c.l*(x-(i + 0.5)/x)) + (c.u*(y-(j + 0.5)/y))) - c.origen;
        float min_choque_dist;
        RGB color; color.red = 0; color.green = 0; color.blue = 0;
        for (int k = 0; k < objs.lenght(); k++) {
          float choque_dist;
          int choques=0;
          if(objs[k].intersection(ray,c.origen,d)){ //comprobamos interseccion
            choques++;
            if(choques==1||d<min_choque_dist){ //comprobamos distancia
              min_choque_dist = choque_dist;
              color = objs[k].solid_color;
            }
          }
        }
        salida(i,j) = color;
      }
    }
  }
};
