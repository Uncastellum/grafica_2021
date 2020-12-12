#pragma once

#include "BasicsRender.hpp"
#include <cmath>

#include <iostream>
using namespace std;

// g++ -std=c++11 -I. 2DObjects.hpp -O3 -o efe.o

class Plane : public Object {
protected:
  Point p;
  Direction normal;
  Plane(){}

public:
  Plane(Point dist, Direction norm) : p(dist), normal(norm.normalize()) {}

  // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
  bool intersection(const Ray& r, float &t, float &dist, Direction& n) override {
    float denom = dotProduct(normal, r.dir);
    if (fabs(denom) > 1e-6 ) {
      Direction p_orig = p - r.orig;
      t = dotProduct(p_orig, normal) / denom;
      dist = t*r.dir.modulus();
      //PATCH  n = normal;
      if(denom < 0) n = normal;
      else n = neg(normal);
      //n = n.normalize(); si normal esta normalizada, la negada tmbn
      return (t >= 0);
    }
    return false;
  }
  void transform(const Matrix &m) override {
    // translate
    p[xi]+=m(0,3); p[yj]+=m(1,3); p[zk]+=m(2,3);
    // rotate
    normal = (m*normal).normalize();
  }
};

class FinitePlane : public Plane {
private:
  Direction v1,v2;
  float v1mod, v2mod;

public:
  FinitePlane(Point dist, Direction v1_, Direction v2_) :
    v1(v1_), v2(v2_) {
      p = dist;
      normal = crossProduct(v1,v2).normalize();
      v1mod = v1.modulus(); v2mod = v2.modulus();
    }

  bool intersection(const Ray& r, float &t, float &dist, Direction& n) override {
    if(Plane::intersection(r,t,dist,n)){
      Point inter = r.orig + r.dir*t;
      Direction auxv1 = inter - p;
      Direction auxv2 = auxv1;

      // DENTRO DEL RECTANGULO??? CODIGO SIMPLIFICADO
      float cosv1 = dotProduct(v1, auxv1) / v1mod;
      float cosv2 = dotProduct(v2, auxv2) / v2mod;
      if (-v1mod < cosv1 && cosv1 < v1mod) {
        if (-v2mod < cosv2 && cosv2 < v2mod) {
          return true;
        }
      }
      return false;

      /* DEMOSTRACION MATEMATICA, NO BORRAR
      double cosv1 = dotProduct(v1, auxv1) / (v1mod*auxv1.modulus());
      auxv1 = auxv1*cosv1;
      double cosv2 = dotProduct(v2, auxv2) / (v2mod*auxv2.modulus());
      auxv2 = auxv2*cosv2;

      float v1test = fabs(auxv1.modulus() / v1mod);
      float v2test = fabs(auxv2.modulus() / v2mod);
      if (v1test > 1.0  || v2test > 1.0) return false;
      else return true;*/
    }
    return false;
  }
  void transform(const Matrix &m) override {
    // translate
    p[xi]+=m(0,3); p[yj]+=m(1,3); p[zk]+=m(2,3);
    // rotate + scale
    v1 = m*v1; v1mod = v1.modulus();
    v2 = m*v2; v2mod = v2.modulus();
    normal = crossProduct(v2,v1).normalize();
  }
};

class Triangle : public Object {
private:
  Point a, b, c;
  Direction normal;
public:
  Triangle(Point _a, Point _b, Point _c) : a(_a), b(_b), c(_c) {
    Direction aux1 = b-a;
    Direction aux2 = c-a;
    normal = crossProduct(aux1, aux2).normalize();
  }
  // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
  bool intersection(const Ray& r, float &t, float &dist, Direction& n) override {
    n = normal;
    float denom = dotProduct(normal, r.dir);


    if(denom < 0){ n = normal;}
    else{ n = neg(normal);}
    //n = n.normalize(); si normal esta normalizada, la negada tmbn

    if (fabs(denom) < 1e-6 ) {// triangle paralelo a rayo
      return false;
    }

    float d = dotProduct(normal, (a-r.orig));
    t = (dotProduct(normal, Direction(0,0,0)) + d) / denom;
    if (t < 0) return false; // triangle detras de camara
    dist = t*r.dir.modulus();

    // Punto de interseccion
    Point P = r.orig + (r.dir * t);

    // Test dentro-fuera triangle
    Direction C;

    // edge 0
    Direction edge0 = b - a;
    Direction vp0 = P - a;
    C = crossProduct(edge0,vp0);
    if (dotProduct(normal,C) < 0) return false; // P is on the right side

    // edge 1
    Direction edge1 = c - b;
    Direction vp1 = P - b;
    C = crossProduct(edge1,vp1);
    if (dotProduct(normal,C) < 0)  return false; // P is on the right side

    // edge 2
    Direction edge2 = a - c;
    Direction vp2 = P - c;
    C = crossProduct(edge2,vp2);
    if (dotProduct(normal,C) < 0) return false; // P is on the right side;

    return true; // this ray hits the triangle
  }
  void transform(const Matrix &m) override {
    // NOT PROTECTED!
    a = m*a;
    b = m*b;
    c = m*c;
    Direction aux1 = b-a;
    Direction aux2 = c-a;
    normal = crossProduct(aux1, aux2).normalize();
  }
  void prtr(){
    cout << "Tri{";
    paint(a);paint(b);paint(c);
    cout << "} -> d(a-b) = " << (b-a).modulus() << ", d(b-c) = "
         << (c-b).modulus() << ", d(a-c)" << (c-a).modulus();
  }
};
