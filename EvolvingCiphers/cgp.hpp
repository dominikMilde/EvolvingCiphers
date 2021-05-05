#pragma once 
#include <vector>

using namespace std;

struct Graph {
    vector<int> graph;
    double fitness;
    Graph(vector<int> graph, double fitness);
    Graph();
};

struct CGP {
    // num of plaintext
    int n;
    // num of ciphertext
    int m;
    // num of rows
    int r;
    // num of columns
    int c;
    // num of node plaintext
    int a;
    vector<int> graph;

    CGP(int n, int m, int r, int c, int a);
    // racuna outpute za dani graf na racun danih inputa
    unsigned char propagate(vector<unsigned char> inputs);

    // racuna zadanu operaciju nad danim argumentima
    double calculateFunction(vector<unsigned char> calcOutputs, int functionId, vector<int> nodeInputIds);
	
    vector<unsigned char> generateCipher(vector<unsigned char> plaintext, vector <unsigned char> key);
};
