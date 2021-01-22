#pragma once

//Compile test: g++ -std=c++11 -I. Image.hpp -O3 -o efe.o

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>

#include "BasicsRender.hpp"

using namespace std;

enum t_mapper { clamp, equalization, clamp_equaliz, gamma_curve, clamp_gamma };
class Tone_Mapper; class Image;
Image fusion(const Image& i1, const Image& i2);

class Image {
private:
  string pSix, max_comm, name;
  int width, height, res_color;
  float real_max;
  vector<RGB> cached;
  bool empty = false;
  friend class Tone_Mapper;
  friend Image fusion(const Image& i1, const Image& i2);

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
    if(!f.is_open()) cout << "\n [Image] ERROR: Archivo no encontrado." << endl;
    assert(f.is_open());

    getline(f, pSix);       assert(pSix == "P3");
    getline(f, max_comm);   assert(max_comm[0] == '#');
    getline(f, name);       assert(name[0] == '#');
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

  RGB& operator()(unsigned x, unsigned y){ // matriz[][]
    assert(!empty);
    assert(!(x >= width || y >= height));
    return cached[y*width + x];
  }
  RGB operator()(unsigned x, unsigned y) const {
    assert(!empty);
    assert(!(x >= width || y >= height));
    return cached[y*width + x];
  }

#pragma GCC optimize("O0")
  void exportLDR(string file){
    assert(!empty);
    ofstream f(file + ".ppm");
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

  void exportHDR(string file, int color_sp=1e7){
    assert(!empty);
    ofstream f(file + ".ppm");
    assert(f.is_open());

    real_max = 1;
    for (int i = 0; i < width*height; i++) {
      float r = cached[i].red / color_sp;
      float g = cached[i].green / color_sp;
      float b = cached[i].blue / color_sp;
      if (r > real_max) real_max = r;
      if (g > real_max) real_max = g;
      if (b > real_max) real_max = b;
    }
    // max 2 decimales
    real_max = (((int)(real_max*100+1))/100.0);

    f << pSix << endl;
    f << "#MAX=" << real_max << endl;
    f << name << endl;
    f << width << " " << height << endl;
    f << color_sp << endl;

    float pre_calc = color_sp / real_max;
    for (int i = 0; i < width*height; i++) {
      int r = cached[i].red * pre_calc;
      int g = cached[i].green * pre_calc;
      int b = cached[i].blue * pre_calc;
      f << r << " " << g << " " << b << "     ";
      if ((i%width)==width-1) f << endl;
    }
    f.close();
  }

  void exportBitmap(string filename){
    typedef struct                       /**** BMP file header structure ****/
        {
        unsigned int   bfSize;           /* Size of file */
        unsigned short bfReserved1;      /* Reserved */
        unsigned short bfReserved2;      /* ... */
        unsigned int   bfOffBits;        /* Offset to bitmap data */
        } BITMAPFILEHEADER;

    typedef struct                       /**** BMP file info structure ****/
        {
        unsigned int   biSize;           /* Size of info header */
        int            biWidth;          /* Width of image */
        int            biHeight;         /* Height of image */
        unsigned short biPlanes;         /* Number of color planes */
        unsigned short biBitCount;       /* Number of bits per pixel */
        unsigned int   biCompression;    /* Type of compression to use */
        unsigned int   biSizeImage;      /* Size of image data */
        int            biXPelsPerMeter;  /* X pixels per meter */
        int            biYPelsPerMeter;  /* Y pixels per meter */
        unsigned int   biClrUsed;        /* Number of colors used */
        unsigned int   biClrImportant;   /* Number of important colors */
        } BITMAPINFOHEADER;

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    /* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
    unsigned short bfType=0x4d42;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfSize = 2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+width*height*3;
    bfh.bfOffBits = 0x36;

    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = height;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = 0;
    bih.biSizeImage = 0;
    bih.biXPelsPerMeter = 5000;
    bih.biYPelsPerMeter = 5000;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;


    //char *cstr = new char[filename.length() + 1];
    //strcpy(cstr, filename.c_str());

    string aux = filename + ".bmp";
    FILE *file = fopen(aux.c_str(), "wb");
    if (!file)
        {
        printf("Could not write file\n");
        return;
        }

    /*Write headers*/
    fwrite(&bfType,1,sizeof(bfType),file);
    fwrite(&bfh, 1, sizeof(bfh), file);
    fwrite(&bih, 1, sizeof(bih), file);

    /*Write bitmap*/
    for (int y = bih.biHeight-1; y>=0; y--) /*Scanline loop backwards*/
        {
        for (int x = 0; x < bih.biWidth; x++) /*Column loop forwards*/
            {
            /*compute some pixel values*/
            RGB it = cached[(y*width)+x];
            it.red = it.red > 1.0 ? 1.0 : it.red ;
            it.green = it.green > 1.0 ? 1.0 : it.green ;
            it.blue = it.blue > 1.0 ? 1.0 : it.blue ;
            unsigned char r = it.red * 255;
            unsigned char g = it.green * 255;
            unsigned char b = it.blue * 255;
            fwrite(&b, 1, 1, file);
            fwrite(&g, 1, 1, file);
            fwrite(&r, 1, 1, file);
            }
        }
    fclose(file);
  }

};

Image fusion(const Image& i1, const Image& i2){
  int width = i1.width + i2.width;
  int max_height = i1.height > i2.height ? i1.height : i2.height;
  int diff = abs((i1.height - i2.height) / 2);
  Image out(width, max_height);
  float max = 0;
  for (size_t it = 0; it < i1.cached.size(); it++) {
    RGB tp = i1.cached[it];
    if (tp.red > max) max = tp.red;
    if (tp.green > max) max = tp.green;
    if (tp.blue > max) max = tp.blue;
  }
  for (size_t it = 0; it < i2.cached.size(); it++) {
    RGB tp = i2.cached[it];
    if (tp.red > max) max = tp.red;
    if (tp.green > max) max = tp.green;
    if (tp.blue > max) max = tp.blue;
  }
  max = max/16;
  for (int i = 0; i < width*max_height; i++) {
    out.cached[i] = RGB(max);
  }
  if(i1.height > i2.height) {
    for (size_t i = 0; i < i1.width; i++) {
      for (size_t j = 0; j < i1.height; j++)
        out.cached[j*width + i] = i1.cached[j*i1.width + i];
    }
    for (size_t i = 0; i < i2.width; i++) {
      for (size_t j = 0; j < i2.height; j++)
        out.cached[(j+diff)*width + i + i1.width] = i2.cached[j*i2.width + i];
    }
  } else {
    for (size_t i = 0; i < i1.width; i++) {
      for (size_t j = 0; j < i1.height; j++)
        out.cached[(j+diff)*width + i] = i1.cached[j*i1.width + i];
    }
    for (size_t i = 0; i < i2.width; i++) {
      for (size_t j = 0; j < i2.height; j++)
        out.cached[j*width + i + i1.width] = i2.cached[j*i2.width + i];
    }
  }
  return out;
}

class Tone_Mapper {
private:
  Image i;
public:
  Tone_Mapper(Image i_) : i(i_){}
  void exportLDR(string file){
    i.exportLDR(file);
  }
  void exportBMP(string file){
    i.exportBitmap(file);
  }
  float getMax(){
    float max = 0;
    for (size_t it = 0; it < i.cached.size(); it++) {
      RGB tp = i.cached[it];
      if (tp.red > max) max = tp.red;
      if (tp.green > max) max = tp.green;
      if (tp.blue > max) max = tp.blue;
    }
    return max;
  }

  // img.apply_tone_mapper(clamp);
  // img.apply_tone_mapper(equalization);
  // img.apply_tone_mapper(clamp_equaliz, 0.456);
  // img.apply_tone_mapper(gamma_curve, X, 0.456);  X = dont care
  // img.apply_tone_mapper(clamp_gamma, 0.456, 0,5);
  void apply_tone_mapper(t_mapper map, float clamp_param = 1, float gamma_param = 1) {
    float clmp = map == clamp ? 1 : clamp_param;
    if (map == clamp || map == clamp_equaliz || map == clamp_gamma) {
      for (size_t it = 0; it < i.width*i.height; it++) {
        i.cached[it].red = i.cached[it].red > clmp ? clmp : i.cached[it].red;
        i.cached[it].green = i.cached[it].green > clmp ? clmp : i.cached[it].green;
        i.cached[it].blue = i.cached[it].blue > clmp ? clmp : i.cached[it].blue;
      }
    }

    if (map == equalization || map == clamp_equaliz || map == gamma_curve || map == clamp_gamma) {
      float max = getMax();
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
