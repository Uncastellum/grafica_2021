#include <iostream>
#include <ctime>

#include "Color.hpp"

//g++ -std=c++11 -I ../libs mainTest.cpp -O3 -o main

using namespace std;

int main(){
  unsigned t0, t1;
  double read_time;

  t0 = clock();
  Image i1("ppm/mpi_atrium_3.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I1 Crear: " << read_time << " segundos" << endl;
  t0 = clock();
  i1.exportLDR("ppm/prueba_sin_tone.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I1 Exportar: " << read_time << " segundos" << endl;



  t0 = clock();
  Image i2("ppm/mpi_atrium_3.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I2 Crear: " << read_time << " segundos" << endl;
  t0 = clock();
  i2.apply_tone_mapper(clamp);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I2 Mapper: " << read_time << " segundos" << endl;
  t0 = clock();
  i2.exportLDR("ppm/prueba_clamp.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I2 Exportar: " << read_time << " segundos" << endl;



  t0 = clock();
  Image i3("ppm/mpi_atrium_3.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I3 Crear: " << read_time << " segundos" << endl;
  t0 = clock();
  i3.apply_tone_mapper(equalization);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I3 Mapper: " << read_time << " segundos" << endl;
  t0 = clock();
  i3.exportLDR("ppm/prueba_equalization.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I3 Exportar: " << read_time << " segundos" << endl;


  t0 = clock();
  Image i4("ppm/mpi_atrium_3.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I4 Crear: " << read_time << " segundos" << endl;
  t0 = clock();
  i4.apply_tone_mapper(clamp_equaliz, 14); // = i4.apply_tone_mapper(clamp_equaliz);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I4 Mapper: " << read_time << " segundos" << endl;
  t0 = clock();
  i4.exportLDR("ppm/prueba_clamp_equaliz.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I4 Exportar: " << read_time << " segundos" << endl;


  t0 = clock();
  Image i5("ppm/mpi_atrium_3.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I5 Crear: " << read_time << " segundos" << endl;
  t0 = clock();
  i5.apply_tone_mapper(gamma_curve, 1, 0.3);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I5 Mapper: " << read_time << " segundos" << endl;
  t0 = clock();
  i5.exportLDR("ppm/prueba_gamma_curve.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I5 Exportar: " << read_time << " segundos" << endl;


  t0 = clock();
  Image i6("ppm/mpi_atrium_3.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I6 Crear: " << read_time << " segundos" << endl;
  t0 = clock();
  i6.apply_tone_mapper(clamp_gamma, 14, 0.3);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I6 Mapper: " << read_time << " segundos" << endl;
  t0 = clock();
  i6.exportLDR("ppm/prueba_clamp_gamma.ppm");
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << "I6 Exportar: " << read_time << " segundos" << endl;
}
