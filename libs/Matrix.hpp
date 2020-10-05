#pragma once
//g++ -std=c++11 matrix.h -I geos.h -O3 -o efe.o

#include <iostream>
#include <cassert>
#include <cmath>

const int LEN = 4;
enum matrix_type { traslate, scale, rotate };
enum rotate_axis { x_axis, y_axis, z_axis };

class Matrix {
private:
  double m[LEN][LEN];
  bool valid;

public:
  Matrix(){
    for (int i = 0; i < LEN; i++) for (int j = 0; j < LEN; j++) m[i][j] = 0;
    valid = true;
  }
  Matrix(double matrix[LEN][LEN]){
    for (int i = 0; i < LEN; i++) for (int j = 0; j < LEN; j++) m[i][j] = matrix[i][j];
    valid = true;
  }
  // Matrix(translate,1,2,3)
  // Matrix(rotate,1,2,3,axis_y)
  Matrix(matrix_type type, double tx, double ty, double tz){
    assert(matrix_type::traslate == type || matrix_type::scale == type);
    valid = true;
    //static_assert( (matrix_type::traslate == type || matrix_type::scale == type),
    //    "This constructor can be used only to use `traslate` or `scale` transformaion");
    for (int i = 0; i < LEN; i++) for (int j = 0; j < LEN; j++) m[i][j] = 0;
    m[0][0] = 1;
    m[1][1] = 1;
    m[2][2] = 1;
    m[3][3] = 1;
    switch (type) {
      case traslate:
        m[0][3] = tx;
        m[1][3] = ty;
        m[2][3] = tz;
        break;
      case scale:
        m[0][0] = tx;
        m[1][1] = ty;
        m[2][2] = tz;
        break;
      default:
        break;
    }
  }

  Matrix(matrix_type type, rotate_axis rt, double angle){
    assert(matrix_type::rotate != type);
    valid = true;
    //static_assert( (matrix_type::rotate != type),
    //    "This constructor can be only used to use `rotate` transformaion");
    for (int i = 0; i < LEN; i++) for (int j = 0; j < LEN; j++) m[i][j] = 0;
    m[0][0] = 1;
    m[1][1] = 1;
    m[2][2] = 1;
    m[3][3] = 1;
    switch (rt) {
      case x_axis:
        m[1][1] = cos(angle);
        m[1][2] = -sin(angle);
        m[2][1] = sin(angle);
        m[2][2] = -cos(angle);
        break;
      case y_axis:
        m[0][0] = cos(angle);
        m[0][2] = -sin(angle);
        //m[2][0]= sin(angle); //MIRAR <--------------- MIRAR <---------------------- MIRAR
        m[2][2] = cos(angle);
        break;
      case z_axis:
        m[0][0] = cos(angle);
        m[0][1] = -sin(angle);
        m[1][0] = sin(angle);
        m[1][1] = cos(angle);
        break;
      default:
        break;
    }
  }

  bool isValid() const { return valid; }

  double& operator()(unsigned row, unsigned col){
    assert(!(row >= LEN || col >= LEN));
    return m[row][col];
  }
  double operator()(unsigned row, unsigned col) const {
    assert(!(row >= LEN || col >= LEN));
    return m[row][col];
  }

  Matrix operator*(const Matrix& b) const {
    Matrix res;
    for(int i=0; i<LEN; i++){
      for(int j=0; j<LEN; j++){
        res.m[i][j]=0;
        for(int k=0; k<LEN; k++){
          res.m[i][j] += m[i][k] * b.m[k][j];
        }
      }
    }

    return res;
  }

  Matrix operator+(const Matrix& b) const {
    Matrix res;
    for(int i=0; i<LEN; i++){
      for(int j=0; j<LEN; j++){
        res.m[i][j] = m[i][j] + b.m[i][j];
      }
    }
    return res;
  }

  Matrix operator-(const Matrix& b) const {
    Matrix res;
    for(int i=0; i<LEN; i++){
      for(int j=0; j<LEN; j++){
        res.m[i][j] = m[i][j] - b.m[i][j];
      }
    }
    return res;
  }

  Matrix inv(){
    double A2323 = m[2][2] * m[3][3] - m[2][3] * m[3][2] ;
    double A1323 = m[2][1] * m[3][3] - m[2][3] * m[3][1] ;
    double A1223 = m[2][1] * m[3][2] - m[2][2] * m[3][1] ;
    double A0323 = m[2][0] * m[3][3] - m[2][3] * m[3][0] ;
    double A0223 = m[2][0] * m[3][2] - m[2][2] * m[3][0] ;
    double A0123 = m[2][0] * m[3][1] - m[2][1] * m[3][0] ;
    double A2313 = m[1][2] * m[3][3] - m[1][3] * m[3][2] ;
    double A1313 = m[1][1] * m[3][3] - m[1][3] * m[3][1] ;
    double A1213 = m[1][1] * m[3][2] - m[1][2] * m[3][1] ;
    double A2312 = m[1][2] * m[2][3] - m[1][3] * m[2][2] ;
    double A1312 = m[1][1] * m[2][3] - m[1][3] * m[2][1] ;
    double A1212 = m[1][1] * m[2][2] - m[1][2] * m[2][1] ;
    double A0313 = m[1][0] * m[3][3] - m[1][3] * m[3][0] ;
    double A0213 = m[1][0] * m[3][2] - m[1][2] * m[3][0] ;
    double A0312 = m[1][0] * m[2][3] - m[1][3] * m[2][0] ;
    double A0212 = m[1][0] * m[2][2] - m[1][2] * m[2][0] ;
    double A0113 = m[1][0] * m[3][1] - m[1][1] * m[3][0] ;
    double A0112 = m[1][0] * m[2][1] - m[1][1] * m[2][0] ;

    double det = m[0][0] * (m[1][1] * A2323 - m[1][2] * A1323 + m[1][3] * A1223)
                - m[0][1] * (m[1][0] * A2323 - m[1][2] * A0323 + m[1][3] * A0223)
                + m[0][2] * (m[1][0] * A1323 - m[1][1] * A0323 + m[1][3] * A0123)
                - m[0][3] * (m[1][0] * A1223 - m[1][1] * A0223 + m[1][2] * A0123);

    Matrix inv;

    if (det == 0) {
      inv.valid = false;
      return inv;
    }
    det = 1 / det;

    inv.m[0][0] = det *   (m[1][1] * A2323 - m[1][2] * A1323 + m[1][3] * A1223);
    inv.m[0][1] = det * - (m[0][1] * A2323 - m[0][2] * A1323 + m[0][3] * A1223);
    inv.m[0][2] = det *   (m[0][1] * A2313 - m[0][2] * A1313 + m[0][3] * A1213);
    inv.m[0][3] = det * - (m[0][1] * A2312 - m[0][2] * A1312 + m[0][3] * A1212);
    inv.m[1][0] = det * - (m[1][0] * A2323 - m[1][2] * A0323 + m[1][3] * A0223);
    inv.m[1][1] = det *   (m[0][0] * A2323 - m[0][2] * A0323 + m[0][3] * A0223);
    inv.m[1][2] = det * - (m[0][0] * A2313 - m[0][2] * A0313 + m[0][3] * A0213);
    inv.m[1][3] = det *   (m[0][0] * A2312 - m[0][2] * A0312 + m[0][3] * A0212);
    inv.m[2][0] = det *   (m[1][0] * A1323 - m[1][1] * A0323 + m[1][3] * A0123);
    inv.m[2][1] = det * - (m[0][0] * A1323 - m[0][1] * A0323 + m[0][3] * A0123);
    inv.m[2][2] = det *   (m[0][0] * A1313 - m[0][1] * A0313 + m[0][3] * A0113);
    inv.m[2][3] = det * - (m[0][0] * A1312 - m[0][1] * A0312 + m[0][3] * A0112);
    inv.m[3][0] = det * - (m[1][0] * A1223 - m[1][1] * A0223 + m[1][2] * A0123);
    inv.m[3][1] = det *   (m[0][0] * A1223 - m[0][1] * A0223 + m[0][2] * A0123);
    inv.m[3][2] = det * - (m[0][0] * A1213 - m[0][1] * A0213 + m[0][2] * A0113);
    inv.m[3][3] = det *   (m[0][0] * A1212 - m[0][1] * A0212 + m[0][2] * A0112);

    return inv;
  }
};
