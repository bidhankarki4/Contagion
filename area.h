
#ifndef _AREA_
#define _AREA_

#include  "agent.h"
#include <vector>

class Area {

  public:

    // the population is represented as agents, each agent has its own attributes (to represent the states)
    vector<Agent> agents;

    // vector for adjacent areas
    vector<int> adjacency;

    // data for each area
    int population, susceptible;
    int infectious, recovered, vaccinated;

    //  constructor
    
    Area() {

      population = 0;
      susceptible = 0;
      infectious = 0;
      recovered = 0;
      vaccinated = 0;

    }
};

#endif
