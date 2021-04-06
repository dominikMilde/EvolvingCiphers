#include <bitset>
#include <iostream>
#include "cgp.hpp"
#include "configs.hpp"
#include "evolution.hpp"

using namespace std;

int main(void)
{
    srand(time(NULL));
    CGP cgpAlice(numInputs, numOutputs, numRows, numColumns, numNodeInputs);
    vector<unsigned char> inputs = { 0b11100111, 0b00000000, 0b10101010};
    vector<unsigned char> outputs;
    unsigned char key = 0b10000011;
   
    vector<int> graph = {3,0,1,0,0,0,2};
	for(auto i : graph)
	{
        cout << i;
	}
    cout << endl;
    cgpAlice.graph = graph;

	for(auto a : inputs)
	{
        vector<unsigned char> inputRound = { a, key };
        outputs.push_back(cgpAlice.propagate(inputRound));
	}

    //cout << fitnessFunction(inputs, outputs);
    runCGP(inputs, outputs, key); //prepravi nazive
    return 0;
}