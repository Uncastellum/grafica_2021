#pragma once

//Compile test: g++ -std=c++11 -I. PLYObject.hpp -O3 -o efe.o

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#include "BasicsRender.hpp" // RGB tuple
#include "2DObjects.hpp"

// Test
#include <cstdlib>
#include <ctime>


class PLYObject: public Object {
private:
  string filename;
  string input, header;
  int vertex, faces;
  vector<Point> points;
  Point center;
  vector<Triangle> obj;
  Triangle *marked;
  struct AABB_points {
    float minX, maxX, minY, maxY, minZ, maxZ;
  } aabb;
  void updateAABB(Point p) {
    if(p[xi] < aabb.minX) aabb.minX = p[xi];
    if(p[xi] > aabb.maxX) aabb.maxX = p[xi];
    if(p[yj] < aabb.minY) aabb.minY = p[yj];
    if(p[yj] > aabb.maxY) aabb.maxY = p[yj];
    if(p[zk] < aabb.minZ) aabb.minZ = p[zk];
    if(p[zk] > aabb.maxZ) aabb.maxZ = p[zk];
  }
  float _max(float a, float b) {if (a>b) return a; return b;}
  float _min(float a, float b) {if (a>b) return b; return a;}
  bool AABB_intersects(const Ray& ray) {
    Direction dir = ray.dir_();
    float t1 = (aabb.minX - ray.orig[xi]) / dir[xi];
    float t2 = (aabb.maxX - ray.orig[xi]) / dir[xi];
    float t3 = (aabb.minY - ray.orig[yj]) / dir[yj];
    float t4 = (aabb.maxY - ray.orig[yj]) / dir[yj];
    float t5 = (aabb.minZ - ray.orig[zk]) / dir[zk];
    float t6 = (aabb.maxZ - ray.orig[zk]) / dir[zk];

    float tmin = _max(_max(_min(t1, t2), _min(t3, t4)), _min(t5, t6));
    float tmax = _min(_min(_max(t1, t2), _max(t3, t4)), _max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    // if tmin > tmax, ray doesn't intersect AABB
    if (tmax < 0 || tmin > tmax) return false;
    //if (tmin < 0f) return tmax;
    //return tmin;
    return true;
  }

public:
  PLYObject(string file){
    // Test
    srand(time(NULL));
    filename = file;
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

    float to_c[6];
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
    Point p((to_c[1]-to_c[0])/2+to_c[0], (to_c[3]-to_c[2])/2+to_c[2], (to_c[5]-to_c[4])/2+to_c[4]);
    paint(p); cout << endl;
    center = p;

    for (size_t aa = 0; aa < points.size(); aa++) {points[aa] -= center; updateAABB(points[aa]);}

    for (size_t i = 0; i < faces; i++) {
      int len;
      f >> len;
      if (len >= 3){
        int a,b,c;
        f >> a >> c;
        RGB test = RGB255(rand()%256,rand()%256,rand()%256);
        for (size_t j = 2; j < len; j++) {
          b = c; f >> c;
          if (a > vertex || b > vertex || c > vertex) continue;
          Triangle t(points[a], points[b], points[c]); t.mt().kd = test;
          obj.push_back(t);
        }
      } else {
        for (size_t j = 0; j < len; j++) {}
      }
      /*
      if (len == 5) {
        Triangle t1(points[ver[0]], points[ver[1]], points[ver[2]]); t1.mt().kd = test;
        Triangle t2(points[ver[1]], points[ver[2]], points[ver[3]]); t2.mt().kd = test;
        Triangle t3(points[ver[2]], points[ver[3]], points[ver[4]]); t3.mt().kd = test;
        Triangle t4(points[ver[3]], points[ver[4]], points[ver[0]]); t4.mt().kd = test;
        Triangle t5(points[ver[4]], points[ver[0]], points[ver[1]]); t5.mt().kd = test;
        obj.push_back(t1); obj.push_back(t2); obj.push_back(t3); obj.push_back(t4); obj.push_back(t5);
      }
      */
    }
    points.clear();
    cout << "(" + file + ")" << " created: " << obj.size() << " polygons added." << endl;

    f.close();
  }

  bool intersects(const Ray& r, Intersection& it) override {
    Matrix localsys(Direction(1,0,0), Direction(0,1,0), Direction(0,0,1), center);
    Ray external;
    external.orig = localsys*r.orig;
    external.dir = localsys*r.dir;
    if(!AABB_intersects(external)) return false;
    it.dist = -1;

    for (int k = 0; k < obj.size(); k++) {
      Intersection it0;
      if( obj[k].intersects(external, it0) ){ //comprobamos interseccion
        if (it.dist == -1 || it.dist > it0.dist) {
          it = it0;
          //solid_color = obj[k].getSolid();
        }
      }
    }

    if(it.dist > 0) return true;
    return false;
  }
  void transform(const Matrix &m) override {
    Matrix fix = m;
    // traslate
    center[xi] += m(0,3); center[yj] += m(1,3); center[zk] += m(2,3);
    fix(0,3) = 0; fix(1,3) = 0; fix(2,3) = 0;
    // scale + rotate but no traslation
    for (size_t i = 0; i < obj.size(); i++) {
      Triangle t = obj[i];
      t.transform(fix);
      updateAABB(t.get_a()); updateAABB(t.get_b()); updateAABB(t.get_c());
      obj[i] = t;
    }
  }

  material& mt() override{
    if (marked == nullptr) return mtl;
    return marked->mt();
  }

  void doItSpecial() override {
    // ../objects/dodecahedron.ply
    if (filename.size() < 10) return;
    if(filename.substr(11) == "icosahedron.ply") {
      if(obj.size() != 20) return;
      RGB r=RGB255(181, 99, 67); RGB g=RGB255(50, 168, 82); RGB b=RGB255(59, 154, 173);
      RGB spec=RGB255(100,100,100); RGB neg(0,0,0);
      for (size_t i = 0; i < obj.size(); i++) {
        Triangle t = obj[i];
        t.mt().is_dielectric = true;
        obj[i] = t;
      }
      obj[0].mt().kd = r; obj[4].mt().kd = r; obj[8].mt().kd = r; obj[12].mt().kd = r;
      obj[1].mt().kd = b; obj[5].mt().kd = b; obj[9].mt().kd = b; obj[13].mt().kd = b;
      obj[2].mt().kd = g; obj[6].mt().kd = g; obj[10].mt().kd = g; obj[14].mt().kd = g;
      obj[3].mt().kd = neg; obj[7].mt().kd = neg; obj[11].mt().kd = neg; obj[15].mt().kd = neg;
      obj[16].mt().kd = spec; obj[17].mt().kd = spec; obj[18].mt().kd = spec; obj[19].mt().kd = spec;
    }
  }

};
