#include "contagion.h"
#include <fstream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <algorithm>
#include <limits.h>
#include <stdio.h>
#include <string>

/* Definition constructor */

using namespace std;

Contagion::Contagion() {

}

/* Definition methods */

void Contagion::loadData(string file_config) {

  //variables to hold the data
  string file_config2, file_region, file_pop;

  ifstream file;

  file.open(file_config, ios::in);

  if (file.fail()) { //checking if the file is opened or not

    cout << "Error." << '\n';
    exit(1);

  } else {

    string text;
    int pos;

    //For pop and csv files
    //reads in the string anf then look for the ":" and then take the string next to it
    file >> text;
    pos = text.find(":");
    file_pop = text.substr(pos + 1, text.size());

    file >> text;
    pos = text.find(":");
    file_region = text.substr(pos + 1, text.size());

    file >> text;

    // for other datas in txt file
    getline(file, text);
    pos = text.find(":");
    infected_area = stoi(text.substr(pos + 1, text.size()));
    getline(file, text);
    pos = text.find(":");
    infectious_period = stoi(text.substr(pos + 1, text.size()));
    getline(file, text);
    pos = text.find(":");
    contact_rate = stoi(text.substr(pos + 1, text.size()));
    getline(file, text);
    pos = text.find(":");
    number_vaccines = stoi(text.substr(pos + 1, text.size()));

    file.close();

  }

  //opening population file

  file.open(file_pop, ios::in);

  if (file.fail()) { //checking if it is open or not

    cout << "Error" << '\n';

  } else {

    string text;
    int pos, n = 0;

    while (!file.eof()) {

      file >> text;
      if(text != "null") {

        //reads in the string and then it looks for the ":", and the value next to is converted to int and pushed into the vector

        pos = text.find(":");
        n = stoi(text.substr(pos + 1, text.size()));

        // creation of the areas of a region
        Area new_area;
        new_area.population = n;
        new_area.susceptible = n;
        areas.push_back(new_area);
        region_population.push_back(n);

      }
      text = "null";
    }

    file.close();
  }

  //opening csv file for the adjacency list

  file.open(file_region, ios::in);

  if (file.fail()) { //checking the file

    cout << "Error." << '\n';

  } else {

    int pos = 0;
    string text;

    file >> text;

    while (!file.eof()) {

      text = "null";
      file >>  text;
      if (text != "null")
        areas[pos++].adjacency = split(text); //the string is passed into the function which passes back vector
    }

    file.close();
  }

  // initializing population for each region

  for (size_t i = 0; i < areas.size(); i++) {

    for (size_t k = 0; k < areas[i].population; k++) {

      Agent new_agent = Agent('S', 0);
      areas[i].agents.push_back(new_agent);
    }

    // applying the virus to the given area.

    if (i + 1 == infected_area) {

      // by default it is applied to the first agent in the area

      areas[i].agents[0].health_condition = 'I';
      areas[i].agents[0].suitable_to_transmit_day = "YES";
      areas[i].agents[0].infection_period = infectious_period;
      areas[i].susceptible -= 1;
      areas[i].infectious += 1;
    }

  }
}

vector<int> Contagion::split(string text) {

  vector<int> v; //vector to save integers
  vector<int> r1;
  bool band = false; //to find the first "," and starting the pos counter
  int pos = 0;
  for (size_t i = 0; i < text.size(); i++) {

    if(text[i] == ',') {
      band = true;
    }
    if(band == true && text[i] != ',') {
      pos++;
      if(text[i] == '1') {
        v.push_back(pos); //saving the integer to vector
        r1.push_back(1);
      }
      else {
        r1.push_back(0);
      }
    }
  }
  region.push_back(r1);
  return v;
}

void Contagion::startSimulation() {

  /*
   * Temporary variables to save the initial configuration.
   * since for each experiment the data is modified.
   */

  vector<Data> temp_data = data;
  vector<Area> temp_areas = areas;
  vector<int> temp_region_population = region_population;
  vector<vector<int>> temp_region = region;
  int temp_infected_area =  infected_area;
  int temp_infectious_period = infectious_period;
  int temp_contact_rate = contact_rate;
  int temp_number_vaccines = number_vaccines;

  displayInitialData();

  cout << "\nCLOSENESS DISTRIBUTION\n";

  /*
   *  For each experiment an initial configuration is made, then
   * the experiment starts, at the end the data is reset.
   */

  closenessDistribution();
  startExperiment();

  resetInformation(temp_data, temp_areas, temp_region_population, temp_region, temp_infected_area, temp_infectious_period, temp_contact_rate, temp_number_vaccines);

  cout << "\nDEGREE DISTRIBUTION\n";

  degreeDistribution();
  startExperiment();

  resetInformation(temp_data, temp_areas, temp_region_population, temp_region, temp_infected_area, temp_infectious_period, temp_contact_rate, temp_number_vaccines);

  cout << "\nRANDOM DISTRIBUTION\n";

  randomDistribution();
  startExperiment();

  resetInformation(temp_data, temp_areas, temp_region_population, temp_region, temp_infected_area, temp_infectious_period, temp_contact_rate, temp_number_vaccines);

  cout << "\nEQUAL DISTRIBUTION\n";

  equalDistribution();
  startExperiment();

  resetInformation(temp_data, temp_areas, temp_region_population, temp_region, temp_infected_area, temp_infectious_period, temp_contact_rate, temp_number_vaccines);

  cout << "\nWEIGHTED DISTRIBUTION\n";

  weightedDistribution();
  startExperiment();

  cout << "\nUsing the closeness centrality distribution method, the peak number of infected was " << data[0].peak_number_infected << " on day " << data[0].peak_day << ".";
  cout << "The outbreak ended on day " << data[0].outbreak_ended << " and the total number of infected individuals was " << data[0].infected_individuals << ".\n";

  cout << "Using the degree centrality distribution method, the peak number of infected was " << data[1].peak_number_infected << " on day " << data[1].peak_day << ".";
  cout << "The outbreak ended on day " << data[1].outbreak_ended << " and the total number of infected individuals was " << data[1].infected_individuals << ".\n";

  cout << "Using the random distribution method, the peak number of infected was " << data[2].peak_number_infected << " on day " << data[2].peak_day << ".";
  cout << "The outbreak ended on day " << data[2].outbreak_ended << " and the total number of infected individuals was " << data[2].infected_individuals << ".\n";

  cout << "Using the equal distribution method, the peak number of infected was "<< data[3].peak_number_infected << " on day " << data[3].peak_day << ".";
  cout << "The outbreak ended on day " << data[3].outbreak_ended << " and the total number of infected individuals was " << data[3].infected_individuals << ".\n";

  cout << "Using the weighted distribution method, the peak number of infected was "<< data[4].peak_number_infected << " on day " << data[4].peak_day << ".";
  cout << "The outbreak ended on day " << data[4].outbreak_ended << " and the total number of infected individuals was " << data[4].infected_individuals << ".\n";

}


void Contagion::applyVaccine(vector<int> turn) {

  for (size_t i = 0; i < areas.size(); i++) {

    //Given an order of distribution of vaccines, the areas are traversed in that order

    for (size_t iter = 0; iter < areas[turn[i]].agents.size() && number_vaccines > 0; iter++) {

      if (areas[turn[i]].agents[iter].health_condition == 'S') {

        areas[turn[i]].agents[iter].health_condition = 'V'; // the status of each individual changes after being vaccinated.
        areas[turn[i]].agents[iter].infection_period = infectious_period; // the recovery period for that individual is established
        areas[turn[i]].susceptible -= 1; //susceptible individuals are reduced by 1
        areas[turn[i]].vaccinated += 1; // vaccinated individuals increases by 1
        number_vaccines -= 1;

      }
      // if vaccines have been applied to 99 percent of the population,
      //  then the individual who initially started the virus will be vaccinated

      else if ((areas[turn[i]].agents[iter].health_condition == 'I' ) || (areas[turn[i]].vaccinated + 1 == areas[turn[i]].population)) {

        areas[turn[i]].agents[iter].health_condition = 'R';
        areas[turn[i]].infectious -= 1;
        areas[turn[i]].vaccinated += 1;
        number_vaccines -= 1;
      }
    }
  }
}

void Contagion::closenessDistribution() {

  vector<int> distance_matix; // vector to store the distances between the areas.
  vector<int> centrality; // vector to store the average centrality between the nodes
  vector<int> turn; // vector to store the shifts corresponding to each area

  distance_matix = getDistanceMatrix(); //this function is invoked, and then this function returns the distance vector.
  double result = 0.0, temp = 0.0;

  for (int i = 0; i < distance_matix.size(); i++) {

    // Given the distance values, the following formulas are used
    // to know the order in which the vaccines should be distributed.
    temp = double(i + 1) / double(distance_matix[i]);
    result = double(distance_matix[i]) + temp; // distance = p + (i/p)
    centrality.push_back(result);

  }

  for (int i = 0; i < centrality.size(); i++) {

    // After applying the previous formula, the minimum is
    // found, and it is removed, thus obtaining the turns corresponding to each area.

    int index_min = std::min_element(centrality.begin(), centrality.end()) - centrality.begin(); // returns the index of the minimum value in the vector
    turn.push_back(index_min); //stores the minimum value in the vector
    centrality[index_min] = 100000; // a very large value is placed, at the value that we extracted previously, so as not to take it again.

  }

  applyVaccine(turn); // This function is invoked, with data shifts, to apply the vaccines.
}

void Contagion::degreeDistribution() {

  vector<int> degree; // stores the degree centrality of each area.
  vector<int> turn; // vector to store the shifts corresponding to each area

  double result = 0.0, temp = 0.0;

  for (int iter = 0; iter < areas.size(); iter++) {

    // The centrality of degrees is given by:
    //  * sum of all the adjacent areas, or also the size of the adjacent areas, then
    //    we apply the formula degrees = p + (i / p) if and only if p is the vector size of the adjacent areas.

    result = float(areas[iter].adjacency.size()) + (1 / float(iter + 1));
    degree.push_back(result);

  }

  for (int i = 0; i < degree.size(); i++) {

    // the centrality of maximum degree is calculated, then the index
    // is saved where it is maximum, that maximum happens to do the -1, so as not to take it again.

    int index_max = std::max_element(degree.begin(), degree.end()) - degree.begin();
    turn.push_back(index_max);
    degree[index_max] = -1;

  }

  applyVaccine(turn); // This function is invoked, with data shifts, to apply the vaccines.

}

void Contagion::randomDistribution() {

  vector<int> turn;

  // The random distribution is from the smallest label to the largest
  // label, so the turn vector is given by the values 1, .... n where n is the size of the region.

  for (size_t i = 0; i < region.size(); i++) {
    turn.push_back(i);
  }

  applyVaccine(turn); // This function is invoked, with data shifts, to apply the vaccines.
}

void Contagion::equalDistribution() {

  int i = 0;

  // The equal distribution is, one vaccine for each area, from the smallest to the largest.

  while (number_vaccines > 0) {

    for (size_t i = 0; i < areas.size(); i++) {

      for (size_t iter = 0; iter < areas[i].agents.size(); iter++) {

        // The respective changes are made to each individual, if the condition is met

        if (areas[i].agents[iter].health_condition == 'S' && number_vaccines > 0) {

          areas[i].agents[iter].health_condition = 'V';
          areas[i].agents[iter].infection_period = infectious_period;
          areas[i].susceptible -= 1;
          areas[i].vaccinated += 1;
          number_vaccines -= 1;
          // abandons the cycle, since it is distributed equally to all regions.
          // and only one vaccine is given per area.
          break;
        }
        else if (areas[i].agents[iter].health_condition == 'I' && areas[i].vaccinated + 1 == areas[i].population) {

          areas[i].agents[iter].health_condition = 'R';
          areas[i].infectious -= 1;
          areas[i].vaccinated += 1;
          number_vaccines -= 1;
          // abandons the cycle, since it is distributed equally to all regions.
          // and only one vaccine is given per area.
          break;
        }
      }
    }
  }
}

void Contagion::weightedDistribution() {

  double proportional_vaccine = 0;
  int number_inhabitants = 0;

  // The proportional distribution is given by:
  // * Number of vaccines / sum of the population.
  // This value is a percentage for each area, the amount of
  // population is multiplied by that value and the number of
  // vaccines is obtained (the floor function is applied)

  for (int iter = 0; iter < areas.size(); iter++) {
    number_inhabitants += areas[iter].population;
  }

  proportional_vaccine = double(number_vaccines) / double(number_inhabitants);

  for (size_t i = 0; i < areas.size(); i++) {
    int cant_vac = 0, temp = 0;

    cant_vac = floor(double(areas[i].population) * proportional_vaccine); // amount of population of the area by the corresponding percentage.

    for (size_t iter = 0; iter < areas[i].agents.size() && cant_vac > 0; iter++) {

      // The respective changes are made to each individual, if the condition is met

      if (areas[i].agents[iter].health_condition == 'S' && number_vaccines > 0) {

        areas[i].agents[iter].health_condition = 'V';
        areas[i].agents[iter].infection_period = infectious_period;
        areas[i].susceptible -= 1;
        areas[i].vaccinated += 1;
        number_vaccines -= 1;
        cant_vac -= 1; // decreases the vaccines that correspond to each population
      }
    }
  }
  int i = 0;

  //The remaining vaccines are distributed from the smallest label to the largest

  while (number_vaccines > 0) {

    for (size_t iter = 0; iter < areas[i].agents.size(); iter++) {

      if (areas[i].agents[iter].health_condition == 'S') {
        areas[i].agents[iter].health_condition = 'V';
        areas[i].agents[iter].infection_period = infectious_period;
        areas[i].susceptible -= 1;
        areas[i].vaccinated += 1;
        number_vaccines -= 1;

        // abandons the cycle, since it is distributed equally to all regions.
        // and only one vaccine is given per area.
        break;
      }
    }
    i++;
  }
}

void Contagion::startExperiment() {

  int first_position = infected_area - 1; // the area that starts with the virus
  int day = 0, amount_infectious_agents = 0;
  int maximum_infected =  0, day_max = 0, aux_inf = 0;
  int affected_individuals = 0;

  vector<int> positions_areas_adj;
  positions_areas_adj.push_back(first_position);

  while (true) {

    cout << "Day " << day++ << endl;
    vector<int> tmp;

    for (size_t i = 0; i < areas.size(); i++) {

      amount_infectious_agents += areas[i].infectious;
      aux_inf += areas[i].infectious;

    }
    // the maximum number of infected is obtained, if the current number
    // is greater than the previous one, then the change is made.

    if (aux_inf > maximum_infected) {

      maximum_infected = aux_inf;
      day_max = day - 1;

    }
    aux_inf = 0;

    for (int iter = 0; iter < areas.size(); iter++) {

      string format;
      // given the values for one day, then the function is invoked,
      // which then returns a string with the corresponding format.

      format = getOutputformat( iter + 1, areas[iter].population, areas[iter].susceptible,
                      areas[iter].infectious, areas[iter].recovered, areas[iter].vaccinated);
      cout << format << endl;
    }

    if (amount_infectious_agents == 0) break; // if the number of infected is 0, then the simulation ends
    else amount_infectious_agents = 0;


    tmp = positions_areas_adj; // stores current adjacent positions.

    for (int pos = 0; pos < positions_areas_adj.size(); pos++) {

      int  position = positions_areas_adj[pos]; // A tour is made for each adjacent infected position.

      if ( (areas[position].infectious >=  (areas[position].population)/2 ) ) {

       for (int l = 0; l < areas[position].adjacency.size(); l++) {

          int pos_sig = areas[position].adjacency[l] - 1;

          // Verify that it is not traveled in the same area.

          if (pos_sig != position) {

            // If the number of infected in the adjacent area is zero, then only one individual from that area is infected.

            if (areas[pos_sig].infectious == 0 && areas[pos_sig].susceptible > 0) {

              // YES-> SIG means that for the next day this individual is apt to infect, not for the same day.
              areas[pos_sig].agents[0].suitable_to_transmit_day = "YES->SIG";

              // The respective changes are made.
              areas[pos_sig].agents[0].health_condition = 'I';
              areas[pos_sig].agents[0].infection_period = infectious_period;
              areas[pos_sig].susceptible -= 1;
              areas[pos_sig].infectious += 1;

              tmp.push_back(pos_sig); // the position of the adjacent area that will spread on the next day is added
            }
          }
        }
      }

      for (int i = 0; i < areas[position].agents.size(); i++) {

        // if there are susceptible individuals then they can become infected

        if (areas[position].susceptible > 0) {

          // "YES" verifies that this area can infect on that day.

          if (areas[position].agents[i].suitable_to_transmit_day == "YES" && areas[position].agents[i].health_condition == 'I' ) {

          int cont = contact_rate; // contagion rate

          for (int k = 0; k < areas[position].agents.size() && cont > 0; k++) {

            if ( (k != i) && (areas[position].susceptible > 0) ) {

              // there must be susceptible individuals, to be able to infect.

              if (areas[position].agents[k].suitable_to_transmit_day == "NO" && (areas[position].agents[k].health_condition != 'R' || areas[position].agents[k].health_condition != 'C') ) {

                // if it infects an individual, that individual can only infect
                // the next day. with "YES-> SIG" I make sure that it does not spread that same day,

                areas[position].agents[k].health_condition = 'I';
                areas[position].agents[k].suitable_to_transmit_day = "YES->SIG";
                areas[position].agents[k].infection_period = infectious_period;
                areas[position].susceptible -= 1;
                areas[position].infectious += 1;
                cont -= 1;
              }

            }
          }
          areas[position].agents[i].suitable_to_transmit_day == "YES->SIG";

       }
        }
      }

    }

    positions_areas_adj = tmp;

    for (size_t i = 0; i < areas.size(); i++) {
        for (size_t k = 0; k < areas[i].agents.size(); k++) {

          if (areas[i].agents[k].suitable_to_transmit_day  == "YES") {

            // it is verified if the recovery period has ended, if so the values are updated.

            areas[i].agents[k].infection_period -= 1;

            if (areas[i].agents[k].infection_period == 0) {

              areas[i].agents[k].suitable_to_transmit_day  == "X";
              areas[i].agents[k].health_condition = 'R'; // when he recovers he is no longer susceptible or infectious
              areas[i].infectious -= 1;
              areas[i].recovered += 1;
            }
          }
          // for all those infected that day, the next day they can infect.

          if(areas[i].agents[k].suitable_to_transmit_day == "YES->SIG") {
            areas[i].agents[k].suitable_to_transmit_day = "YES";
          }
        }
        amount_infectious_agents += areas[i].infectious;
    }

    cout << "\n";
  }

  for (size_t i = 0; i < areas.size(); i++) {
    affected_individuals += areas[i].recovered; // counts the number of infected individuals
  }

  // are saved are data of the day.

  Data d1;
  d1.peak_number_infected = maximum_infected;
  d1.peak_day = day_max;
  d1.outbreak_ended = day - 1;
  d1.infected_individuals = affected_individuals;

  data.push_back(d1);
}


string Contagion::getOutputformat(int w_iter, int w_population, int w_susceptible, int w_infectious, int w_recovered, int w_vaccinated) {

  // data values, build the format corresponding to the outputs.

  string a, b, c, d, e, f;

  a = to_string(w_iter);
  b = to_string(w_population);
  c = to_string(w_infectious);
  d = to_string(w_recovered);
  e = to_string(w_vaccinated);
  f = to_string(w_susceptible);

  if (w_iter < 10) a += "   ";
  else a += "  ";

  int cont = b.size();
  while (cont++ < 7)
      b += " ";

  cont = c.size();
  while (cont++ < 7)
      c += " ";

  cont = d.size();
  while (cont++ < 7)
      d += " ";

  cont = e.size();
  while (cont++ < 7)
      e += " ";

  cont = f.size();
  while (cont++ < 7)
      f += " ";

  cont = e.size();
  while (cont++ < 7)
      e += " ";

  return a + "POP: " + b + "S: " + f + "I: " + c + "R: " + d + "V: " + e;
}

void Contagion::displayInitialData() {

  //Printing the regional population

  cout << "Regional Population\n";
  for (size_t i = 0; i < areas.size(); i++) {
    cout << i + 1 << " " << areas[i].population << "\n";;
  }

  //Printing the adjacency list
  cout << "\nAdjacency List\n";

  for (size_t i = 0; i < areas.size(); i++) {
    cout << i + 1 << ":";
    for (size_t k = 0; k < areas[i].adjacency.size(); k++) {
      cout << " " << areas[i].adjacency[k];
    }
    cout << "\n";
  }
}

vector<int> Contagion::getDistanceMatrix() {

  vector<int> distances;

  for (int area = 0; area < areas.size(); area++) {

    vector<int> v;
    v = findRoute(area);

    int result = 0;
    for (size_t i = 0; i < v.size(); i++)
      result += v[i];

    distances.push_back(result);
  }

  return distances;
}

int Contagion::shortestRoute(vector<int> roads, vector<int> band){

  // find the shortest route

  int iterator = 0, shortest_path, position = 0;

  // a very large value, simulates the shortest distance.
  shortest_path = region.size() * region.size();

  //  Find the area with the minimum distance
  while (iterator < region.size()) {

    if (roads[iterator] <= shortest_path &&  band[iterator] == 0) {
      shortest_path = roads[iterator], position = iterator;
    }

    iterator++;
  }

  return position;
}


vector<int> Contagion::findRoute(int area) {

  // stores the shortest distances, from the given area to the other areas.
  vector<int> roads(region.size(), region.size() * region.size());
  // to check if the distance to an area is the shortest
  vector<int> band(region.size(), 0);

  // the distance to the same area is always 0
  roads[area] = 0;

  int cont = 0, position = 0;

  while (cont  < region.size() - 1) {

    // the shortest distance to an area is searched, for the variable "area"
    position = shortestRoute(roads, band);

    band[position] = 1; // the area is = 1, it means that it has already been used
    int cont_adj = 0;

    while (cont_adj < region.size()) {

       // the vector roads is updated if and only if the path is the shortest
        if (!band[cont_adj] && region[position][cont_adj] && roads[position] != INT_MAX
            && roads[position] + region[position][cont_adj] < roads[cont_adj]) {
              roads[cont_adj] = roads[position] + region[position][cont_adj];
        }
      cont_adj++;
    }
    cont++;
  }

  // returns a vector of the shortest distances to all adjacent areas.
  return roads;
}

void Contagion::resetInformation(vector<Data> a, vector<Area> b, vector<int> c, vector<vector<int>> d, int e, int f, int g, int h) {

  // information is reset
  areas = b;
  region_population = c;
  region = d;
  infected_area = e;
  infectious_period = f;
  contact_rate = g;
  number_vaccines = h;

}
