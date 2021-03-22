

#ifndef _AGENT_
#define _AGENT_

#include <iostream>

using namespace std;

class Agent {

  public:

    // Attributes

    // condition of the individual or agent

    char health_condition;

    // recovery period
    int infection_period;

    // to verify if it is suitable to spread the virus on the current day or for the next day.
    string suitable_to_transmit_day;

    // Constructors
    Agent();

    Agent(char new_health_condition, int new_infection_period) {

        health_condition = new_health_condition;
        infection_period = new_infection_period;
        suitable_to_transmit_day = "NO"; // initially no individual is fit to spread the virus
    }

};


#endif
