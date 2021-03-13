#include <bitset>
#include <iostream>
#include "cgp.hpp"

using namespace std;

int main(void)
{
    srand(time(NULL));
    CGP cgp(2, 1, 1, 2, 2);
    vector<unsigned char> inputs = { 0b11001100, 0b10000011 };
    bitset<8> i1(inputs.at(0));
    bitset<8> i2(inputs.at(1));
    vector<int> graph = cgp.randomGraph();
    cout << "genotype: ";
    for (auto i : graph) 
    {
        cout << i << " ";
    }
    cout << endl;
    cout << "input 0: " << i1 << " input 1: " << i2 << endl;
	
    cgp.graph = graph;
    cgp.propagate(inputs);
    return 0;
}