#include <iostream>
#include <string>
#include <sstream>

#include "Color.hpp"

//g++ -std=c++11 -I ../libs mainTocho.cpp -O3 -o main2

using namespace std;

string to_str(t_mapper a){
  if ( a == clamp) return "clamp";
  if ( a == equalization) return "equalization";
  if ( a == clamp_equaliz) return "clamp_equaliz";
  if ( a == gamma_curve) return "gamma_curve";
  if ( a == clamp_gamma) return "clamp_gamma";
  return "";
}

string ftm(float a){
  stringstream ss;
  ss << a;
  return ss.str();
}

int main() {
  int choice;
  bool gameOn = true;
  while (gameOn != false){
    cout << "*************** CHOOSE FILE ****************\n";
    cout << " 1 - forest_path.ppm\n";
    cout << " 2 - mpi_atrium_1.ppm\n";
    cout << " 3 - mpi_atrium_3.ppm\n";
    cout << " 4 - mpi_office.ppm\n";
    cout << " 5 - nancy_church_1.ppm\n";
    cout << " 6 - nancy_church_2.ppm\n";
    cout << " 7 - nancy_church_3.ppm\n";
    cout << " 8 - seymour_park.ppm\n\n";
    cout << " 0 - EXIT\n\n";
    cout << " Enter your choice and press return: ";
    cin >> choice;
    string file = "ppm/";

    switch (choice) {
      case 1:
        file += "forest_path.ppm";
        break;
      case 2:
        file += "mpi_atrium_1.ppm";
        break;
      case 3:
        file += "mpi_atrium_3.ppm";
        break;
      case 4:
        file += "mpi_office.ppm";
        break;
      case 5:
        file += "nancy_church_1.ppm";
        break;
      case 6:
        file += "nancy_church_2.ppm";
        break;
      case 7:
        file += "nancy_church_3.ppm";
        break;
      case 8:
        file += "seymour_park.ppm";
        break;
      case 0:
        cout << "End of Program.\n";
        gameOn = false;
        break;
      default:
        while (choice < 0 && choice > 8){
          cout << "Not a Valid Choice. \n";
          cout << "Choose again: ";
          cin >> choice;
        }
        break;
    }
    if (!gameOn) break;

    cout << " >> Loading File...\n\n";
    Image i(file);


    cout << "\n*************** TONE MAPPER ****************\n";
    cout << " 1 - clamp\n";
    cout << " 2 - equalization\n";
    cout << " 3 - clamp_equaliz\n";
    cout << " 4 - gamma_curve\n";
    cout << " 5 - clamp_gamma\n";
    cout << " 6 - reinhard\n\n";
    cout << " 0 - EXIT\n\n";
    cout << " Enter your choice and press return: ";
    cin >> choice;
    if (choice == 0){
      cout << " >> End of Program.\n";
      gameOn = false;
      break;
    }
    while (choice < 0 && choice > 5){
      cout << " >> Not a Valid Choice. \n";
      cout << "Choose again: ";
      cin >> choice;
    }
    t_mapper select[] = { clamp, equalization, clamp_equaliz, gamma_curve, clamp_gamma };

    cout << " >> Tone Mapping...\n";
    string mapper;
    float clamp, gamma;
    if (choice == 1 || choice == 2){
      mapper = to_str(select[choice-1]);
      i.apply_tone_mapper(select[choice-1]);
    } else if (choice == 3 || choice == 5) {
      cout << " [!] Enter Clamp Value: ";
      cin >> clamp;
      mapper = to_str(select[choice-1]) + "-clmp" + ftm(clamp);
      if (choice == 3) i.apply_tone_mapper(select[choice-1], clamp);
      else {
        cout << " [!] Enter Gamma Value: ";
        cin >> gamma;
        mapper += "-gmma" + ftm(gamma);
        i.apply_tone_mapper(select[choice-1], clamp, gamma);
      }
    } else if (choice == 4) { //choice == 4
      cout << " [!] Enter Gamma Value: ";
      cin >> gamma;
      mapper = to_str(select[choice-1]) + "-gmma" + ftm(gamma);
      i.apply_tone_mapper(select[choice-1], 1, gamma);
    } else {
      mapper = "reinhard";
      i.apply_reinhard_tmapper();
    }


    file.erase(file.end()-4,file.end());
    file = file + "_" + mapper + ".ppm";
    file[0] = 'r'; file[1] = 'e'; file[2] = 's';
    cout << " >> Saving in \"" + file + "\"...\n";
    i.exportLDR(file);

  }
  return 0;
}
