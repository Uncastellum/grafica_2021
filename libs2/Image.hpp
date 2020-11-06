#pragma once

//Compile test: g++ -std=c++11 -I. Color.hpp -O3 -o efe.o

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>

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

    FILE *file = fopen("render.bmp", "wb");
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
            unsigned char r = cached[(y*width)+x].red;
            unsigned char g = cached[(y*width)+x].green;
            unsigned char b = cached[(y*width)+x].blue;
            fwrite(&b, 1, 1, file);
            fwrite(&g, 1, 1, file);
            fwrite(&r, 1, 1, file);
            }
        }
    fclose(file);
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
