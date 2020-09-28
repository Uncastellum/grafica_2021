
#include <iostream>
#include <stdexception>
#include <cmath>
using namespace std;

const int LEN = 4;

class Point {
  private:
    double c[LEN];

  public:
    Point(int x, int y, int z){
        c[0] = x; c[1] = y; c[2] = z;
    }
    double getx(){
      return c[0];
    }
    double gety(){
      return c[1];
    }
    double getz(){
      return c[2];
    }
    void setx(int x){
      c[0] = x;
    }
    void sety(int y){
      c[1] = y;
    }
    void setz(int z){
      c[2] = z;
    }
};


class Vector {
  private:
     double v[3];

  public:
    Vector(int x, int y, int z){
        v[0] = x; v[1] = y; v[2] = z;
    }
    double getx(){
      return v[0];
    }
    double gety(){
      return v[1];
    }
    double getz(){
      return v[2];
    }
    void setx(int x){
      v[0] = x;
    }
    void sety(int y){
      v[1] = y;
    }
    void setz(int z){
      v[2] = z;
    }
}
/*
  |
  |
  |      ___ .a
  |    |/_
  |   .b
  |
  |
  ---------------------
  vector2d = a-b

  .........
  Point a = Point(0,0,0);
  Point b = Point(1,1,1);
  Vector c = ? //COMPLETA DANI  (INVOCANDO A LA FUNCION QUE HAS HECHO XD) //SOBRECARGA DE OPERADORES
  //DEFINICION DEL OPERADOR - (menos) PARA DOS PUNTOS CON RESULTADO VECTOR
  c = {
    c = a-b //= (-1,-1,-1)
    c = b-a //= (1,1,1)
  }
*/

Vector operator-(const Point& a, const Point& b){
  Vector res = Vector(a.getx()-b.getx(), a.gety()-b.gety(), a.getz()-b.getz());
  return res;
}
Vector operator+(const Vector& a, const Vector& b){
  Vector res = Vector(a.getx()+b.getx(), a.gety()+b.gety(), a.getz()+b.getz());
  return res;
}
Vector operator-(const Vector& a, const Vector& b){
  Vector res = Vector(a.getx()-b.getx(), a.gety()-b.gety(), a.getz()-b.getz());
  return res;
}
Vector operator*(const Vector& a, double b){
  Vector res = Vector(a.getx()*b, a.gety()*b, a.getz()*b);
  return res;
}

double modulus(const Vector& a){
  double res = sqrt( pow(a.getx(),2) + pow(a.gety(),2) + pow(a.getz(),2) );
  return res;
}

double dot(const Vector& a, const Vector& b);
Vector cross(const Vector& a, const Vector& b);

// Ejercicio 1: #include <tuple> std::tuple<Point, Vector>
template<class T, class U>
class Tuple {
  T first;
  U second;
public:
  Tuple(T x, U y){
    first = x; second = y;
  }
  T first(){
    return first;
  }
  T second(){
    return second;
  }
};


class Sphere {
  private:
    Point center;
    Vector axis;
    Point ref;

  public:
    Sphere(Vector x, Vector y, Vector z){
      center = x;
      axis = y;
      ref = z;
      Vector cenref = ref-center;
      if (modulus(cenref)-(modulus(axis)/2) > 0.000001) throw invalid_argument("Sphere error (+ 10^-6 radius)");
    }
    Point getcenter(){
      return center;
    }
    Vector getaxis(){
      return axis;
    }
    Point getref(){
      return ref;
    }
    void setcenter(Point x){
      center = x;
    }
    void setaxis(Vector y){
      axis = y;
    }
    void setref(Point z){
      ref = z;
    }
};

















.
