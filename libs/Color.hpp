#pragma once

//Compile test: g++ -std=c++11 -I. Color.hpp -O3 -o efe.o

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

enum t_mapper { clamp, equalization, clamp_equaliz, gamma_curve, clamp_gamma };


class Image {
private:
  struct RGB {
    float red, green, blue;
  };
  string pSix, max_comm, name;
  int width, height, res_color;
  float real_max;
  vector<RGB> cached;
  const float PI = 3.1415926535;

public:
  Image(string file){
    ifstream f(file);
    assert(f.is_open());

    getline(f, pSix);
    getline(f, max_comm);
    getline(f, name);
    f >> width;
    f >> height;
    f >> res_color;

    // string::#MAX=xxxxxx => float::xxxxxx
    real_max = stof(max_comm.erase(0, 5));

    while (!f.eof()) {
      RGB aux;
      f >> aux.red;
      f >> aux.green;
      f >> aux.blue;
      aux.red *= real_max/res_color;
      aux.green *= real_max/res_color;
      aux.blue *= real_max/res_color;
      cached.push_back(aux);
    }
    f.close();
  }

#pragma GCC optimize("O0")
  void exportLDR(string file){
    ofstream f(file);
    assert(f.is_open());
    f << pSix << endl;
    f << "#MAX=1" << endl;
    f << name << endl;
    f << width << " " << height << endl;
    f << 255 << endl;
    for (int i = 0; i < width*height; i++) {
      int r = cached[i].red * 255;
      int g = cached[i].green * 255;
      int b = cached[i].blue * 255;
      if(r > 255) r=255;
      if(g > 255) g=255;
      if(b > 255) b=255;
      f << r << " " << g << " " << b << "     ";
      if ((i%width)==width-1) f << endl;
    }
    f.close();
  }

  // img.apply_tone_mapper(clamp);
  // img.apply_tone_mapper(clamp_equaliz, 0.456);
  // img.apply_tone_mapper(gamma_curve, X, 0.456);  X = dont care
  // img.apply_tone_mapper(clamp_gamma, 0.456, 0,5);
  void apply_tone_mapper(t_mapper map, float clamp_param = 1, float gamma_param = 1) {
    float clmp = map == clamp ? 1 : clamp_param;
    if (map == clamp || map == clamp_equaliz || map == clamp_gamma) {
      for (size_t i = 0; i < width*height; i++) {
        cached[i].red = cached[i].red > clmp ? 1 : cached[i].red;
        cached[i].green = cached[i].green > clmp ? 1 : cached[i].green;
        cached[i].blue = cached[i].blue > clmp ? 1 : cached[i].blue;
      }
    }

    if (map == equalization || map == clamp_equaliz || map == gamma_curve || map == clamp_gamma) {
      float max = 0;
      for (size_t i = 0; i < width*height; i++) {
        if (cached[i].red > max) max = cached[i].red;
        if (cached[i].green > max) max = cached[i].green;
        if (cached[i].blue > max) max = cached[i].blue;
      }
      for (size_t i = 0; i < width*height; i++) {
        cached[i].red = cached[i].red/max;
        cached[i].green = cached[i].green/max;
        cached[i].blue = cached[i].blue/max;
      }
    }

    if (map == gamma_curve || map == clamp_gamma){
      for (size_t i = 0; i < width*height; i++) {
        cached[i].red = pow(cached[i].red, gamma_param);
        cached[i].green = pow(cached[i].green, gamma_param);
        cached[i].blue = pow(cached[i].blue, gamma_param);
      }
    }
  }
  /*void apply_reinhard_tmapper(float k_param = 1){
    // apply_tone_mapper(equalization)
    float max = 0;
    for (size_t i = 0; i < width*height; i++) {
      if (cached[i].red > max) max = cached[i].red;
      if (cached[i].green > max) max = cached[i].green;
      if (cached[i].blue > max) max = cached[i].blue;
    }
    

    // y = Asin(wt+phi) + y_desp
    for (size_t i = 0; i < width*height; i++) {
      cached[i].red    =  0.5*sin( PI/max * cached[i].red - PI/2) + 0.5;
      cached[i].green  =  0.5*sin( PI/max * cached[i].green - PI/2) + 0.5;
      cached[i].blue   =  0.5*sin( PI/max * cached[i].blue - PI/2) + 0.5;
    }
  }*/
  void apply_reinhard_tmapper(float k_param = 1){
    for (size_t i = 0; i < width*height; i++) {
      cached[i].red    =  cached[i].red*k_param / (1 + cached[i].red*k_param);
      cached[i].green  =  cached[i].green*k_param / (1 + cached[i].green*k_param);
      cached[i].blue   =  cached[i].blue*k_param / (1 + cached[i].blue*k_param);
    }
  }

};
