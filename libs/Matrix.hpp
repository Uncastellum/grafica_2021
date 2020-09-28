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

public:
  Matrix(){
    for (int i = 0; i < LEN; i++) for (int j = 0; j < LEN; j++) m[i][j] = 0;
  }
  Matrix(double matrix[LEN][LEN]){
    for (int i = 0; i < LEN; i++) for (int j = 0; j < LEN; j++) m[i][j] = matrix[i][j];
  }
  // Matrix(translate,1,2,3)
  // Matrix(rotate,1,2,3,axis_y)
  Matrix(matrix_type type, double tx, double ty, double tz){
    assert(matrix_type::traslate == type || matrix_type::scale == type);
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

  double& operator()(unsigned row, unsigned col){
    assert(!(row >= LEN || col >= LEN));
    return m[row][col];
  }
  double operator()(unsigned row, unsigned col) const {
    assert(!(row >= LEN || col >= LEN));
    return m[row][col];
  }

  Matrix operator*(const Matrix& b){
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

  Matrix operator+(const Matrix& b){
    Matrix res;
    for(int i=0; i<LEN; i++){
      for(int j=0; j<LEN; j++){
        res.m[i][j] = m[i][j] + b.m[i][j];
      }
    }
    return res;
  }

  Matrix operator-(const Matrix& b){
    Matrix res;
    for(int i=0; i<LEN; i++){
      for(int j=0; j<LEN; j++){
        res.m[i][j] = m[i][j] - b.m[i][j];
      }
    }
    return res;
  }

  // Check isnan(res(0,0)) to know if Matrix has inverse (GAUSS-JORDAN METHOD)
  Matrix inv() {
    double aug_m[LEN*2][LEN*2], temp;
    for (int i = 0; i < LEN; i++) for (int j = 0; j < LEN; j++) aug_m[i][j] = m[i][j];

    // Create the augmented matrix
    // Add the identity matrix of order at the end of original matrix.
    for (int i = 0; i < LEN; i++) {
      for (int j = 0; j < 2 * LEN; j++) {
        // Add '1' at the diagonal places of the matrix to create a identity matirx
        if (j == (i + LEN)) aug_m[i][j] = 1;
      }
    }

    // Interchange the row of matrix, interchanging of row will start from the last row
    for (int i = LEN - 1; i > 0; i--) {
      if (aug_m[i - 1][0] < aug_m[i][0]) {
        for (int j = 0; j < 2*LEN; j++) {
          temp = aug_m[i][j];
          aug_m[i][j] = aug_m[i - 1][j];
          aug_m[i - 1][j] = temp;
        }
      }
    }

    // Replace a row by sum of itself and a
    // constant multiple of another row of the matrix
    for (int i = 0; i < LEN; i++) {
      for (int j = 0; j < LEN; j++) {
        if (j != i) {
          temp = aug_m[j][i] / aug_m[i][i];
          for (int k = 0; k < 2*LEN; k++) aug_m[j][k] -= aug_m[i][k] * temp;
        }
      }
    }

    // Multiply each row by a nonzero integer. Divide row element by the diagonal element
    for (int i = 0; i < LEN; i++) {
      temp = aug_m[i][i];
      for (int j = 0; j < 2*LEN; j++) aug_m[i][j] = aug_m[i][j] / temp;
    }

    Matrix res;
    for (int i = 0; i < LEN; i++) for (int j = LEN; j < 2*LEN; j++) res.m[i][j-LEN] = aug_m[i][j];
    return res;
  }

};
