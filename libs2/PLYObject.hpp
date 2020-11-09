#pragma once

//Compile test: g++ -std=c++11 -I. PLYObject.hpp -O3 -o efe.o

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#include "GeOS.hpp" // RGB tuple
#include "2DObjects.hpp"

// Test
#include <cstdlib>
#include <ctime>


class PLYObject: public Object {
private:
  string input, header;
  int vertex, triangles;
  vector<Point> points;
  vector<Triangle> obj;

public:
  PLYObject(string file){
    // Test
    srand(time(NULL));
    ifstream f(file);
    assert(f.is_open());

    getline(f, input);   header = input;    assert(input == "ply");
    getline(f, input);   header += "\n" + input; assert(input == "format ascii 1.0");
    do{
      getline(f, input);    header += "\n" + input;
    }while(input.substr(0,7) == "comment");
    assert(input.substr(0,14) == "element vertex");
    vertex = stoi(input.erase(0, 15));

    getline(f, input);   header += "\n" + input; assert(input.substr(0,8) == "property");
    getline(f, input);   header += "\n" + input; assert(input.substr(0,8) == "property");
    getline(f, input);   header += "\n" + input; assert(input.substr(0,8) == "property");

    getline(f, input);   header += "\n" + input; assert(input.substr(0,12) == "element face");
    triangles = stoi(input.erase(0, 13));
    getline(f, input);   header += "\n" + input; assert(input.substr(0,13) == "property list");
    getline(f, input);   header += "\n" + input; assert(input == "end_header");


    for (size_t i = 0; i < vertex; i++) {
      float x,y,z;
      f >> x >> y >> z;
      points.push_back(Point(x,y,z));
    }

    for (size_t i = 0; i < triangles; i++) {
      int len;
      f >> len;
      if (len == 3){
        int a,b,c;
        f >> a >> b >> c;
        if (a > vertex || b > vertex || c > vertex) continue;
        Triangle t(points[a], points[b], points[c]); t.setRGB(RGB(rand()%256,rand()%256,rand()%256));
        obj.push_back(t);
      } else {
        for (size_t j = 0; j < len; j++) {}
      }
    }
    points.clear();
    cout << "(" + file + ")" << " created: " << obj.size() << " polygons added." << endl;

    f.close();
  }

  bool intersection(const Ray& r, float &t, float &dist) override {
    float d = 0, t0 = 0; dist = -1, t = -1;
    solid_color = RGB(64,64,64);

    for (int k = 0; k < obj.size(); k++) {
      if( obj[k].intersection(r, t0, d) ){ //comprobamos interseccion
        if (dist == -1 || dist > d) {
          dist = d; t = t0;
          //solid_color = obj[k].getSolid();
          setRGB(obj[k].getSolid());
        }
      }
    }

    paint(solid_color); cout << ".   " << t << ", "<< dist<< endl;
  }

};
