#include <iostream>
#include "Matrix.hpp"
#include "Sphere.hpp"
#include "GeOS.hpp"

//g++ -std=c++11 -I ../libs mainTest.cpp -O3 -o main

using namespace std;

void paint(Matrix m){
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) printf("%.3f  ", m(i,j));
    cout << endl;
  }
}
void paint(VecPun v){
  printf("%.3f  ", v.getxi());
  printf("%.3f  ", v.getyj());
  printf("%.3f  ", v.getzk());
  cout << endl;
}
void paint(SpherePoint sp){
  VecPun v = sp.getCoor();
  printf("%.3f  ", v.getxi());
  printf("%.3f  ", v.getyj());
  printf("%.3f  ", v.getzk());
  cout << endl;
}


int main() {
  double m[4][4] = {
    {2, 3, 7, 9},
    {5, 4, -1, 4},
    {8, 7, 8, 5},
    {4, 3, 0, 9}
  };
  double no_inv[4][4] = {
    {1, 2, 3, 4},
    {1, 2, 3, 4},
    {2, 5, 4, 7},
    {-8, 5, -7, 5}
  };

  cout <<"++|++|++|++ Prueba radio ++|++|++|++"<< endl;
  cout <<"(Comprobamos que la ciudad esta a distancia radio)"<< endl;
  VecPun center = VecPun(0,0,0,1);
  VecPun axis = VecPun(2,0,0,0);
  VecPun zgz = VecPun(1,0,0,1);
  VecPun murcia = VecPun(8,0,0,1);
  cout <<"1.El siguiente planeta deberia ser correcto"<< endl;
  Sphere mars = Sphere(center,axis,zgz);
  cout <<"2.El siguiente planeta deberia ser incorrecto"<< endl;
  Sphere marsista = Sphere(center,axis,murcia);
  cout<<endl;

  cout <<"++|++|++|++ Prueba conexion ++|++|++|++"<< endl;
  cout <<"(Comprobamos conexion entre 2 estaciones)"<< endl;
  cout <<"1.Conexion sin colision (1,0,0)"<< endl;
  Sphere sa = Sphere(VecPun(0,0,0,1), VecPun(0,2,0,0), VecPun(1,0,0,1));
  Sphere sb = Sphere(VecPun(3,0,0,1), VecPun(0,2,0,0), VecPun(2,0,0,1));
  SpherePoint pa = getPoint(sa, 0, PI/2);
  SpherePoint pb = getPoint(sb, PI, PI/2);
  //SpherePoint pb = getPoint_degree(sb, 180, 90);
  VecPun con = pa.getConexion(pb);
  //paint(pa); paint(pb);
  cout<<"La conexion es: ";
  paint(con);
  cout<<endl;
  cout <<"2.Conexion con colision (3,0,0)"<< endl;
  sa = Sphere(VecPun(0,0,0,1), VecPun(0,2,0,0), VecPun(1,0,0,1));
  sb = Sphere(VecPun(3,0,0,1), VecPun(0,2,0,0), VecPun(2,0,0,1));
  pa = getPoint(sa, 0, PI/2);
  pb = getPoint(sb, 0, PI/2);
  //SpherePoint pb = getPoint_degree(sb, 180, 90);
  con = pa.getConexion(pb);
  //paint(pa); paint(pb);
  cout<<"La conexion es: ";
  paint(con);
  cout<<endl;

  /*cout <<"Prueba matrix"<< endl;
  Matrix a = Matrix(m);
  Matrix mni = Matrix(no_inv);

  paint(a);
  cout << endl;
  Matrix b = a.inv();
  cout << endl;
  paint(a*b);
  cout << endl;
  paint(b*b.inv());
  cout << endl;cout << endl;cout << endl;


  paint(mni);
  cout << endl;
  Matrix c = mni.inv();
  if (c.isValid()) paint(c);
  else cout << "NO INVERSIBLE" << endl;


  double mv[4][4] = {
    {0, 1, 2, 0},
    {1, 4, 0, 0},
    {1, 1, -1, 0},
    {0, 0, 1, 0}
  };
  Matrix mm = Matrix(mv);
  VecPun v = VecPun(4,2,7,false);
  paint(v);
  v = v*mm;
  paint(v);*/
}
