
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

#include "Image.hpp"

//g++ -std=c++11 -I ../libs mainMapper.cpp -O3 -o mappers

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

int main(int argc, char *argv[]) {
  int choice;
  unsigned t0, t1;
  double read_time;
  bool gameOn = true;
  if (argc != 2) {
    cout << "Uso: <exec> <nombre archivo> " << endl;
    return 88;
  }
  string file = argv[1];

  cout << " >> Loading File... ";
  t0 = clock();
  Image img(file);
  t1 = clock();
  read_time = (double(t1-t0)/CLOCKS_PER_SEC);
  cout << read_time << " segundos" << endl << endl;

  while (gameOn != false){
    file = argv[1];
    Tone_Mapper tm(img);
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
      cout << " :)\n";
      gameOn = false;
      break;
    }
    while (choice < 0 && choice > 6){
      cout << " >> Not a Valid Choice. \n";
      cout << "Choose again: ";
      cin >> choice;
    }
    t_mapper select[] = { clamp, equalization, clamp_equaliz, gamma_curve, clamp_gamma };

    cout << " >> Tone Mapping...\n";
    string mapper = "";
    float clamp = 1, gamma = 1, k = 1;
    if (choice == 1 || choice == 3 || choice == 5) {
      float aux = tm.getMax();
      cout << " [!] Enter Clamp Value (float or *percentage) (max value=" << aux << "): ";
      string cin_clamp;
      cin >> cin_clamp;
      if(cin_clamp[0] == '*') clamp = aux*atof(cin_clamp.erase(0,1).c_str());
      else clamp = atof(cin_clamp.c_str());
      mapper += "-clmp" + ftm(clamp);
    }
    if (choice == 4 || choice == 5) {
      cout << " [!] Enter Gamma Value: ";
      cin >> gamma;
      mapper += "-gmma" + ftm(gamma);
    }
    if (choice == 6) {
      cout << " [!] Enter k Value: ";
      cin >> k;
      mapper = "reinhard-k" + ftm(k);
      tm.apply_reinhard_tmapper(k);
    } else {
      mapper = to_str(select[choice-1]) + mapper;
      tm.apply_tone_mapper(select[choice-1], clamp, gamma);
    }

    file.erase(file.end()-4,file.end());
    file = file + "_" + mapper;
    //file[0] = 'r'; file[1] = 'e'; file[2] = 's';
    cout << " >> Saving in \"" + file + ".bmp\"...\n";
    tm.exportBMP(file);

  }
  return 0;
}
