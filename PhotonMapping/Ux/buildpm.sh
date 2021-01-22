#!/bin/bash

g++ -no-pie -std=c++11 ../SmallPM/src/main.cpp ../SmallPM/src/PhotonMapping.cpp ../SmallPM/src/RenderEngine.cpp ../SmallPM/smallrt/lib/smallrt.a -I../SmallPM/smallrt/include -I../SmallPM/include/ -o smallpm
