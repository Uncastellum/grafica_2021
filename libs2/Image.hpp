#pragma once

//Compile test: g++ -std=c++11 -I. Color.hpp -O3 -o efe.o

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "GeOS.hpp" // RGB tuple
//#include "Sphere.hpp" //PI

using namespace std;

enum t_mapper { clamp, equalization, clamp_equaliz, gamma_curve, clamp_gamma };
class Tone_Mapper;

class Image {
private:
  string pSix, max_comm, name;
  int width, height, res_color;
  float real_max;
  vector<RGB> cached;
  bool empty = false;
  friend class Tone_Mapper;

public:
  Image() : empty(true) {}
  Image(int width_, int height_) : width(width_), height(height_) {
    pSix = "P3";
    name = "# ppm file";
    for (size_t i = 0; i < width*height; i++) {
      RGB black; black.red = 0; black.green = 0; black.blue = 0;
      cached.push_back(black);
    }
  }
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

  RGB& operator()(unsigned row, unsigned col){ // matriz[][]
    assert(!empty);
    assert(!(row >= height || col >= width));
    return cached[row*width + col];
  }
  RGB operator()(unsigned row, unsigned col) const {
    assert(!empty);
    assert(!(row >= height || col >= width));
    return cached[row*width + col];
  }

#pragma GCC optimize("O0")
  void exportLDR(string file){
    assert(!empty);
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

  void exportRay(string file){
    assert(!empty);
    ofstream f(file);
    assert(f.is_open());
    f << pSix << endl;
    f << "#MAX=1" << endl;
    f << name << endl;
    f << width << " " << height << endl;
    f << 255 << endl;
    for (int i = 0; i < width*height; i++) {
      f << cached[i].red << " " << cached[i].green << " " << cached[i].blue << "     ";
      if ((i%width)==width-1) f << endl;
    }
    f.close();
  }

};

class Tone_Mapper {
private:
  Image i;
public:
  Tone_Mapper(Image i_) : i(i_){}
  void exportLDR(string file){
    i.exportLDR(file);
  }

  // img.apply_tone_mapper(clamp);
  // img.apply_tone_mapper(clamp_equaliz, 0.456);
  // img.apply_tone_mapper(gamma_curve, X, 0.456);  X = dont care
  // img.apply_tone_mapper(clamp_gamma, 0.456, 0,5);
  void apply_tone_mapper(t_mapper map, float clamp_param = 1, float gamma_param = 1) {
    float clmp = map == clamp ? 1 : clamp_param;
    if (map == clamp || map == clamp_equaliz || map == clamp_gamma) {
      for (size_t it = 0; it < i.width*i.height; it++) {
        i.cached[it].red = i.cached[it].red > clmp ? 1 : i.cached[it].red;
        i.cached[it].green = i.cached[it].green > clmp ? 1 : i.cached[it].green;
        i.cached[it].blue = i.cached[it].blue > clmp ? 1 : i.cached[it].blue;
      }
    }

    if (map == equalization || map == clamp_equaliz || map == gamma_curve || map == clamp_gamma) {
      float max = 0;
      for (size_t it = 0; it < i.width*i.height; it++) {
        if (i.cached[it].red > max) max = i.cached[it].red;
        if (i.cached[it].green > max) max = i.cached[it].green;
        if (i.cached[it].blue > max) max = i.cached[it].blue;
      }
      for (size_t it = 0; it < i.width*i.height; it++) {
        i.cached[it].red = i.cached[it].red/max;
        i.cached[it].green = i.cached[it].green/max;
        i.cached[it].blue = i.cached[it].blue/max;
      }
    }

    if (map == gamma_curve || map == clamp_gamma){
      for (size_t it = 0; it < i.width*i.height; it++) {
        i.cached[it].red = pow(i.cached[it].red, gamma_param);
        i.cached[it].green = pow(i.cached[it].green, gamma_param);
        i.cached[it].blue = pow(i.cached[it].blue, gamma_param);
      }
    }
  }
  void apply_reinhard_tmapper(float k_param = 1){
    for (size_t it = 0; it < i.width*i.height; it++) {
      i.cached[it].red    =  i.cached[it].red*k_param / (1 + i.cached[it].red*k_param);
      i.cached[it].green  =  i.cached[it].green*k_param / (1 + i.cached[it].green*k_param);
      i.cached[it].blue   =  i.cached[it].blue*k_param / (1 + i.cached[it].blue*k_param);
    }
  }
};
