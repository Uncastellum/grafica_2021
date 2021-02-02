#include <iostream>
#include "Matrix.hpp"
#include "Sphere.hpp"
#include "GeOS.hpp"

//g++ -std=c++11 -I ../libs2 mainTest2.cpp -O3 -o main2

using namespace std;

/*void paint(Matrix m){
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) printf("%.3f  ", m(i,j));
    cout << endl;
  }
}
void paint(GeomObj v){
  printf("%.3f  ", v.getxi());
  printf("%.3f  ", v.getyj());
  printf("%.3f  ", v.getzk());
  cout << endl;
}*/
void paint(SpherePoint sp){
  Point v = sp.getCoor();
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
  Point center(0,0,0);
  Direction axis(2,0,0);
  Point zgz(1,0,0), murcia(8,0,0);
  cout <<"1.El siguiente planeta deberia ser correcto"<< endl;
  Sphere mars = Sphere(center,axis,zgz);
  cout <<"2.El siguiente planeta deberia ser incorrecto"<< endl;
  Sphere marsista = Sphere(center,axis,murcia);
  cout<<endl;

  cout <<"++|++|++|++ Prueba conexion ++|++|++|++"<< endl;
  cout <<"(Comprobamos conexion entre 2 estaciones)"<< endl;
  cout <<"1.Conexion sin colision (1,0,0)"<< endl;
  Sphere sa = Sphere(Point(0,0,0), Direction(0,2,0), Point(0,0,1));
  Sphere sb = Sphere(Point(3,0,0), Direction(0,2,0), Point(3,0,1));
  SpherePoint pa = getPoint_degree(sa, -90, 90);
  SpherePoint pb = getPoint(sb, PI/2, PI/2);
  //SpherePoint pb = getPoint_degree(sb, 180, 90);
  Direction con = pa.getConexion(pb);
  //paint(pa); paint(pb);
  cout<<"La conexion es: ";
  paint(con);
  cout<<endl;
  cout <<"2.Conexion con colision (3,0,0)"<< endl;
  sa = Sphere(Point(0,0,0), Direction(0,2,0), Point(1,0,0));
  sb = Sphere(Point(3,0,0), Direction(0,2,0), Point(2,0,0));
  pa = getPoint(sa, 0, PI/2);
  pb = getPoint(sb, PI, PI/2);
  //SpherePoint pb = getPoint_degree(sb, 180, 90);
  con = pa.getConexion(pb);
  //paint(pa); paint(pb);
  cout<<"La conexion es: ";
  paint(con);
  cout<<endl;
}
