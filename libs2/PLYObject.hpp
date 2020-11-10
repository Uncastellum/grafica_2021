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
  int vertex, faces;
  vector<Point> points;
  Point center;
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
    faces = stoi(input.erase(0, 13));
    getline(f, input);   header += "\n" + input; assert(input.substr(0,13) == "property list");
    getline(f, input);   header += "\n" + input; assert(input == "end_header");

    /*int to_c[6];
    for (size_t i = 0; i < vertex; i++) {
      float x,y,z;
      f >> x >> y >> z;
      if(i==0){
        to_c[0]=x;to_c[1]=x;to_c[2]=y;to_c[3]=y;to_c[4]=z;to_c[5]=z;
      } else {
        if (to_c[0] > x) to_c[0]=x;
        if (to_c[1] < x) to_c[1]=x;
        if (to_c[2] > y) to_c[1]=y;
        if (to_c[3] < y) to_c[1]=y;
        if (to_c[4] > z) to_c[1]=z;
        if (to_c[5] < z) to_c[1]=z;
      }
      points.push_back(Point(x,y,z));
    }
    center = Point(
      (to_c[1]-to_c[0])/2+to_c[0],
      (to_c[3]-to_c[2])/2+to_c[2],
      (to_c[5]-to_c[4])/2+to_c[4],
    );
    for (size_t aa = 0; aa < points.size(); aa++) points[aa] -= center;*/

    for (size_t i = 0; i < faces; i++) {
      int len;
      f >> len;
      if (len >= 3){
        int a,b,c;
        f >> a >> c;
        RGB test(rand()%256,rand()%256,rand()%256);
        for (size_t j = 2; j < len; j++) {
          b = c; f >> c;
          if (a > vertex || b > vertex || c > vertex) continue;
          Triangle t(points[a], points[b], points[c]); t.setRGB(test);
          obj.push_back(t);
        }
      } else {
        for (size_t j = 0; j < len; j++) {}
      }
      /*
      if (len == 5) {
        Triangle t1(points[ver[0]], points[ver[1]], points[ver[2]]); t1.setRGB(test);
        Triangle t2(points[ver[1]], points[ver[2]], points[ver[3]]); t2.setRGB(test);
        Triangle t3(points[ver[2]], points[ver[3]], points[ver[4]]); t3.setRGB(test);
        Triangle t4(points[ver[3]], points[ver[4]], points[ver[0]]); t4.setRGB(test);
        Triangle t5(points[ver[4]], points[ver[0]], points[ver[1]]); t5.setRGB(test);
        obj.push_back(t1); obj.push_back(t2); obj.push_back(t3); obj.push_back(t4); obj.push_back(t5);
      }
      */
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
          solid_color = obj[k].getSolid();
        }
      }
    }

    if(dist > 0) {
      return true;
    } else {
      return false;
    }
  }

};
