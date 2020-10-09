#pragma once

//Compile test: g++ -std=c++11 -I. Color.hpp -O3 -o efe.o

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


class Image {
private:
  struct RGB {
    unsigned int red, green, blue;
  };
  string pSix, max_comm, name;
  int width, height, max;
  vector<RGB> cached;


public:
  Image(string file){
    ifstream f(file);
    assert(f.is_open());

    getline(f, pSix);
    getline(f, max_comm);
    getline(f, name);
    f >> width;
    f >> height;
    f >> max;

    //printf("Name: %s\n", name);               //WTF
    cout << "Name: " << name << endl;
    //printf("PSix: %s\n", pSix);               //WTF
    cout << "PSix: " << pSix << endl;
    printf("Width: %d\n", width);
    printf("Height: %d\n", height);
    printf("maximum: %d\n", max);

    while (!f.eof() && width*height > cached.size()) {
      RGB aux;
      f >> aux.red;
      f >> aux.green;
      f >> aux.blue;
      cached.push_back(aux);
    }
    //cout << count << "/" << width*height << ", " << f.eof() << endl;
    //string rest; f >> rest; cout << rest << f.eof() << endl;

    f.close();
  }

  transform(){
    for (int i = 0; i < width*height; i++) {
      //cached[i].red *= max_comm/max;
      //cached[i].green *= max_comm/max;
      //cached[i].blue *= max_comm/max;
    }
  }
  tonemapping(){

  }

};












// .
