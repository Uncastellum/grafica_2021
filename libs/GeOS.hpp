#pragma once

#include <iostream>
#include <exception>
#include <cmath>

#include "Matrix.hpp"

using namespace std;

enum VecPunComp { xi = 0, yj = 1, zk = 2 };

class VecPun {
private:
  double g[4];
public:
  VecPun(){
    for (int i = 0; i < 4; i++) g[i] = 0;
  }
  VecPun(int xi, int yj, int zk, bool isPoint){
    g[0] = xi; g[1] = yj; g[2] = zk;
    g[3] = isPoint ? 1 : 0;
  }
  bool isPoint() const {
    return g[3] == 1;
  }
  bool isVector() const {
    return g[3] == 0;
  }
  double& operator[](VecPunComp c){
    switch (c) {
      case xi:
        return g[0];
      case yj:
        return g[1];
      case zk:
        return g[2];
    }
  }
  double operator[](VecPunComp c) const {
    switch (c) {
      case xi:
        return g[0];
      case yj:
        return g[1];
      case zk:
        return g[2];
    }
  }
  double getxi() const {
    return g[0];
  }
  double getyj() const {
    return g[1];
  }
  double getzk() const {
    return g[2];
  }
  void setxi(int xi){
    g[0] = xi;
  }
  void setyj(int yj){
    g[1] = yj;
  }
  void setzk(int zk){
    g[2] = zk;
  }
  VecPun operator-(const VecPun& other) const {
    if(isPoint() && other.isPoint()) {
      VecPun res = VecPun(g[0] - other.g[0], g[1] - other.g[1], g[2] - other.g[2], false);
      return res;
    } else if(isVector() && other.isVector()) {
      VecPun res = VecPun(g[0] - other.g[0], g[1] - other.g[1], g[2] - other.g[2], false);
      return res;
    } else {
      throw invalid_argument("Operator - is not defined for this operation: "
            + isPoint() ? "Point - ":"Vector - " + other.isPoint() ? "Point":"Vector");
    }
  }
  VecPun operator+(const VecPun& other) const {
    if(isVector() && other.isVector()){
      VecPun res = VecPun(g[0] + other.g[0], g[1] + other.g[1], g[2] + other.g[2], false);
      return res;
    }else if(isPoint() && other.isVector()){
      VecPun res = VecPun(g[0] + other.g[0], g[1] + other.g[1], g[2] + other.g[2], true);
      return res;
    }else{
      throw invalid_argument("Operator + is not defined for this operation: "
            + isPoint() ? "Point + ":"Vector + " + other.isPoint() ? "Point":"Vector");
    }
  }
  VecPun operator*(double escalar) const {
    if(isVector()){
      VecPun res = VecPun(g[0]*escalar, g[1]*escalar, g[2]  *escalar, false);
      return res;
    }else{
      throw invalid_argument("Operator * is not defined for this operation: Point * Escalar");
    }
  }
  VecPun operator*(const Matrix& m) const {
    VecPun res;
    for (int i = 0; i < LEN; i++) {
      res.g[i] = 0;
      for (int j = 0; j < LEN; j++){
        res.g[i] += g[j]*m(j,i);
      }
    }
    return res;
  }
  double modulus() const {
    if (isVector()) {
      double res = sqrt( pow(g[0],2) + pow(g[1],2) + pow(g[2],2) );
      return res;
    } else {
      throw invalid_argument("Operation `modulus` is not defined for Points");
    }
  }
};




double dotProduct(const VecPun& a, const VecPun& b) {
  if (a.isVector() && b.isVector()) {
    double res = a[xi]*b[xi] + a[yj]*b[yj] + a[zk]*b[zk];
    return res;
  } else {
    throw invalid_argument("Operation `dotProduct` is not defined for Points");
  }
}
VecPun crossProduct(const VecPun& a, const VecPun& b) {
  if (a.isVector() && b.isVector()) {
    VecPun res = VecPun(a[xi]*b[xi], a[yj]*b[yj], a[zk]*b[zk], false);
    return res;
  } else {
    throw invalid_argument("Operation `crossProduct` is not defined for Points");
  }
}
