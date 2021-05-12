#pragma once 
#include <vector>

#include "configs.hpp"

using namespace std;

class Key {
public:
	vector<bool> key;
    double fitness;

    Key(vector<bool> key, double fitness);
    Key();

    vector <unsigned char> toChar();
    void printKey();
};

