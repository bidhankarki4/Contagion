#include "contagion.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {

  //Prompting the user for txt file and saving it
  Contagion simulation;

  string file_config;

  cout << "Please enter the name of the configuration file:";
  cin >> file_config;

  simulation.loadData(file_config);
  simulation.startSimulation();
  return 0;
}
