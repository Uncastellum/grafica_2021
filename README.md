# Informática Gráfica
---
Trabajo relacionado con la asignatura de 4º del grado de Ingeniería Informática de [@Unizar](https://github.com/universidad-zaragoza)

#### Creado por
| |  |
| -- | :-: |
| ![Sergio García](https://avatars.githubusercontent.com/sergioge99?size=50)  | [Sergio García](https://github.com/sergioge99)  |
| ![Daniel González](https://avatars.githubusercontent.com/uncastellum?size=50)  | [Daniel González](https://github.com/uncastellum)  |

### Dependencias

Librerias relacionadas con la paralelización del código (OPENMP)

### Estructura
| Carpetas | Descripción |
| :- | :- |
| [legacy](legacy/) | Ejercicios previos al desarrollo de los algoritmos |
| [objects](objects/) | Ficheros .PLY |
| [libs](libs/) | Conjunto de librerias necesarias para el funcionamiento de los algoritmos |
| [pr2-Mappers](pr2-Mappers/) | Implementación de un programa para el uso de los tone mappers |
| [pr3-RayTracing](pr3-RayTracing/) | Ejercicio relacionado con la implementación de la práctica 3 |
| [pr4-PathTracing](pr4-PathTracing/) | Implementación de un par de escenas con el uso del algoritmo PathTracing |
| [pr5-PhotonMapping](pr5-PhotonMapping/) | Algoritmo PhotonMapping (solo para Linux) |


### Compilación

La compilación de cada archivo se encuentra en un comentario al principio de cada fichero compilable:
```c++
...
#include "Scene.hpp"

// g++ -std=c++11 -I ../libs mainMapper.cpp -O3 -o mappers

using namespace std;
...
```

Ejecutar ese comando desde una terminal en el directorio en el que se encuentre el archivo a compilar.

! El proyecto PhotonMapping solo es compilable en Linux ->


### Licencia

[GNU GPL v3.0](LICENSE.md)
