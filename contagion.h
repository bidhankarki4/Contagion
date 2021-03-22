

#ifndef _CONTAGION_
#define _CONTAGION_

#include <iostream>
#include <vector>
#include "area.h"
#include "data.h"
#include "agent.h"

using namespace std;



class Contagion {

  // Attributes .

  private:

    vector<Data> data; //only 5 element
    vector<Area> areas;   //vector for areas (nodes)
    vector<int> region_population; //vector for population
    vector<vector<int>> region;    //vector for adjacency list
    int infected_area, infectious_period;
    int contact_rate, number_vaccines;



  public:

    // Constructors

    Contagion();

    // Methods

    string getOutputformat(int, int, int, int, int, int);
    void loadData(string );
    vector<int> split(string );
    void startSimulation();
    void displayInitialData();
    vector<int> getDistanceMatrix();
    int shortestRoute(vector<int> , vector<int>);
    vector<int> findRoute(int );
    void applyVaccine(vector<int> );
    void resetInformation(vector<Data> a, vector<Area> b, vector<int> c, vector<vector<int>> d, int e, int f, int g, int h);

    // Experiments

    void closenessDistribution();
    void degreeDistribution();
    void randomDistribution();
    void equalDistribution();
    void weightedDistribution();

    void startExperiment();

};


#endif
