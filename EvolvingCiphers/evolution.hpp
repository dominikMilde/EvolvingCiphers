#pragma once

#include <vector>

#include "cgp.hpp"
#include "key.hpp"

double fitnessIndividual(vector <int>& graph, vector <vector <unsigned char>>& keys, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts);

double fitnessFunction(vector<unsigned char>& inputs, vector<unsigned char>& outputs);

Graph findBestGraph(vector<Graph>& graphs);

Key findBestKey(vector<Key>& keys);

vector<int> crossover(vector<int>& mainGraph, vector<int>& otherGraph);

vector<bool> crossoverKey(vector<bool>& firstKey, vector<bool>& secondKey);

Graph crossAndReturnBestOfThree(Graph& firstParent, Graph& secondParent, vector <vector <unsigned char>>& keys, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts);

Key crossAndReturnBestOfThreeKey(Key& firstParent, Key& secondParent, vector <unsigned char>& plaintext, vector <unsigned char>& ciphertext, Graph& bob);

Graph mutation(Graph& graphStruct, vector <vector <unsigned char>>& keys, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts);

vector<int> randomGraph();

vector<bool> randomKey();

void fillInitialPopulationCGP(vector<Graph>& graphs, vector <vector <unsigned char>>& keys, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts);

void print(Graph& g);

Graph evaluateBob(vector<vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& keys, vector <vector <unsigned char>>& ciphertext);

Key evaluateEva(Graph &bob, vector <unsigned char>& plaintext, vector <unsigned char>& ciphertext);

void fillInitialPopulationKeys(vector<Key>& keys, Graph& bob, vector <unsigned char>& plaintext, vector <unsigned char>& ciphertext);

//double rateAlice(vector<int>& alice, vector <unsigned char>& plaintext, vector <unsigned char>& key);

Graph crossAndReturnBestOfThreeAlice(Graph& firstParent, Graph& secondParent, vector<vector<unsigned char>>& keys, vector<vector<unsigned char>>& plaintext);

Graph mutationAlice(Graph& alice, vector<vector<unsigned char>>& keys, vector<vector<unsigned char>>& plaintexts);

double fitnessFunctionMultiple(vector <vector<unsigned char>>& inputs, vector <vector<unsigned char>>& outputs);



