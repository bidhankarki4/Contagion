
#ifndef _DATA_
#define _DATA_

#include <iostream>

class Data {

  public:


    // variables to store the requested records, for each day in an experiment.

    int peak_number_infected;
    int peak_day;
    int outbreak_ended;
    int infected_individuals;

    // constructor
    
    Data() {

      peak_number_infected = 0;
      peak_day = 0;
      outbreak_ended = 0;
      infected_individuals = 0;

    }

};

#endif
