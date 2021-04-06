#pragma once

#include <vector>

#include "cgp.hpp"


double fitnessFunction(vector<unsigned char> inputs, vector<unsigned char> outputs);

Graph findBestGraph();

Graph mutation(Graph graphStruct);

vector<int> randomGraph();

void fillInitialPopulationCGP();

void print(Graph& g);

void runGeneration();

Graph runCGP(vector<unsigned char> inputsF, vector<unsigned char> outputsF, unsigned char key);
