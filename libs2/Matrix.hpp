#pragma once

//Compile test: g++ -std=c++11 -I. Matrix.hpp -O3 -o efe.o

#include <iostream>
#include <cassert>
#include <cmath>

#include "GeOS.hpp"

const int LEN = 4;
enum matrix_type { traslate, scale, rotate };
enum rotate_axis { x_axis, y_axis, z_axis };

#define PI 3.1415926535
#define RAD_DEG270 PI*3/2
#define RAD_DEG180 PI/1
#define RAD_DEG90 PI/2
#define RAD_DEG45 PI/4
#define RAD_DEG30 PI/6

class Matrix {
private:
  float m[LEN][LEN];
  bool valid;

public:
  Matrix(){
    for (int i = 0; i < LEN; i++) for (int j = 0; j < LEN; j++) m[i][j] = 0;
    valid = true;
  }
  Matrix(float matrix[LEN][LEN]){
    for (int i = 0; i < LEN; i++) for (int j = 0; j < LEN; j++) m[i][j] = matrix[i][j];
    valid = true;
  }
  // Matrix(translate,1,2,3)
  // Matrix(rotate,1,2,3,axis_y)
  Matrix(matrix_type type, float tx, float ty, float tz){
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

  Matrix(matrix_type type, rotate_axis rt, float angle, bool inDegree = true){
    float r_angle = angle;
    if (inDegree) r_angle = r_angle/180*PI;
    assert(matrix_type::rotate == type);
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
        m[1][1] = cos(r_angle);
        m[1][2] = -sin(r_angle);
        m[2][1] = sin(r_angle);
        m[2][2] = cos(r_angle);
        break;
      case y_axis:
        m[0][0] = cos(r_angle);
        m[0][2] = -sin(r_angle);
        m[2][0]= sin(r_angle);
        m[2][2] = cos(r_angle);
        break;
      case z_axis:
        m[0][0] = cos(r_angle);
        m[0][1] = -sin(r_angle);
        m[1][0] = sin(r_angle);
        m[1][1] = cos(r_angle);
        break;
      default:
        break;
    }
  }
  Matrix(const Direction& u, const Direction& v, const Direction& w, const Point& o) {
    // Matrix ( u, v, w, o)
    //        (u1,v1,w1,o1)
    //        (u2,v2,w2,o2)
    //        (u3,v3,w3,o3)
    //        ( 0, 0, 0, 1)
    m[0][0] = u[xi]; m[1][0] = u[yj]; m[2][0] = u[zk];
    m[0][1] = v[xi]; m[1][1] = v[yj]; m[2][1] = v[zk];
    m[0][2] = w[xi]; m[1][2] = w[yj]; m[2][2] = w[zk];
    m[0][3] = o[xi]; m[1][3] = o[yj]; m[2][3] = o[zk];
    m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
  }

  bool isValid() const { return valid; }

  float& operator()(unsigned row, unsigned col){
    assert(!(row >= LEN || col >= LEN));
    return m[row][col];
  }
  float operator()(unsigned row, unsigned col) const {
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

  Direction operator*(const Direction& d) const {
    Direction res;
    for (int i = 0; i < LEN; i++) {
      res.g[i] = 0;
      for (int j = 0; j < LEN; j++){
        res.g[i] += m[i][j]*d.g[j];
      }
    }
    assert(res.isDirection());
    return res;
  }
  Point operator*(const Point& p) const {
    Point res;
    for (int i = 0; i < LEN; i++) {
      res.g[i] = 0;
      for (int j = 0; j < LEN; j++){
        res.g[i] += m[i][j]*p.g[j];
      }
    }
    assert(res.isPoint());
    return res;
  }

  Matrix inv(){
    float A2323 = m[2][2] * m[3][3] - m[2][3] * m[3][2] ;
    float A1323 = m[2][1] * m[3][3] - m[2][3] * m[3][1] ;
    float A1223 = m[2][1] * m[3][2] - m[2][2] * m[3][1] ;
    float A0323 = m[2][0] * m[3][3] - m[2][3] * m[3][0] ;
    float A0223 = m[2][0] * m[3][2] - m[2][2] * m[3][0] ;
    float A0123 = m[2][0] * m[3][1] - m[2][1] * m[3][0] ;
    float A2313 = m[1][2] * m[3][3] - m[1][3] * m[3][2] ;
    float A1313 = m[1][1] * m[3][3] - m[1][3] * m[3][1] ;
    float A1213 = m[1][1] * m[3][2] - m[1][2] * m[3][1] ;
    float A2312 = m[1][2] * m[2][3] - m[1][3] * m[2][2] ;
    float A1312 = m[1][1] * m[2][3] - m[1][3] * m[2][1] ;
    float A1212 = m[1][1] * m[2][2] - m[1][2] * m[2][1] ;
    float A0313 = m[1][0] * m[3][3] - m[1][3] * m[3][0] ;
    float A0213 = m[1][0] * m[3][2] - m[1][2] * m[3][0] ;
    float A0312 = m[1][0] * m[2][3] - m[1][3] * m[2][0] ;
    float A0212 = m[1][0] * m[2][2] - m[1][2] * m[2][0] ;
    float A0113 = m[1][0] * m[3][1] - m[1][1] * m[3][0] ;
    float A0112 = m[1][0] * m[2][1] - m[1][1] * m[2][0] ;

    float det = m[0][0] * (m[1][1] * A2323 - m[1][2] * A1323 + m[1][3] * A1223)
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


void paint(Matrix m){
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) printf("%.3f  ", m(i,j));
    cout << endl;
  }
}
