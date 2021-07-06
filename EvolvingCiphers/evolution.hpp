#pragma once

#include <vector>

#include "cgp.hpp"
#include "key.hpp"

double fitnessIndividual(vector <int>& graph, vector <unsigned char>& key, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts);

double fitnessFunction(vector<unsigned char>& inputs, vector<unsigned char>& outputs);

Graph findBestGraph(vector<Graph>& graphs);

Key findBestKey(vector<Key>& keys);

vector<int> crossover(vector<int>& mainGraph, vector<int>& otherGraph);

vector<bool> crossoverKey(vector<bool>& firstKey, vector<bool>& secondKey);

Graph crossAndReturnBestOfThree(Graph& firstParent, Graph& secondParent, vector  <unsigned char>& key, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts);

Key crossAndReturnBestOfThreeKey(Key& firstParent, Key& secondParent, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts, Graph& bob);

Graph mutation(Graph& graphStruct, vector <unsigned char>& key, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts);

vector<int> randomGraph();

vector<bool> randomKey();

void fillInitialPopulationCGP(vector<Graph>& graphs, vector <unsigned char>& key, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts);

void print(Graph& g);

Graph evaluateBob(vector<vector <unsigned char>>& plaintexts, vector <unsigned char>& key, vector <vector <unsigned char>>& ciphertext);

Key evaluateEva(Graph &bob, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts);

void fillInitialPopulationKeys(vector<Key>& keys, Graph& bob, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts);

double rateAlice(vector<int>& alice, vector <vector <unsigned char>>& plaintexts, vector <unsigned char>& key);

Graph crossAndReturnBestOfThreeAlice(Graph& firstParent, Graph& secondParent, vector<unsigned char>& key, vector<vector<unsigned char>>& plaintext);

Graph mutationAlice(Graph& alice, vector<unsigned char>& key, vector<vector<unsigned char>>& plaintexts);

double fitnessFunctionMultiple(vector <vector<unsigned char>>& inputs, vector <vector<unsigned char>>& outputs);

void primjeniRunde(vector <vector <unsigned char>>& plaintexts, CGP& cgpAlice, CGP& cgpBob, vector <unsigned  char>& k, vector <unsigned char>& keyEva, int numOfRounds);
