#pragma once

#include <vector>

#include "cgp.hpp"


double fitnessFunction(vector<unsigned char> inputs, vector<unsigned char> outputs);

Graph findBestGraph(vector<Graph>& graphs);

Graph mutation(Graph graphStruct, bool giveKey);

vector<int> randomGraph();

void fillInitialPopulationCGP(vector<Graph>& graphs, bool giveKey);

void print(Graph& g);

void runGeneration();

Graph runCGP(vector<unsigned char> inputsF, vector<unsigned char> outputsF, unsigned char key);
