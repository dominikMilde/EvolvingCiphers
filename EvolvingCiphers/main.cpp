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
    vector<unsigned char> plaintext = { 0b11100111, 0b00000000, 0b10101010, 0b01110100, 0b11110101};
    vector<unsigned char> ciphertext;
    unsigned char key = 0b10000011; //produzi key na isti velicinu
   
    vector<int> graph = {3, 0, 1, 5, 2, 0, 3}; //za Alice reverzibilna
	for(auto i : graph)
	{
        cout << i;
	}
    cout << endl;
    cgpAlice.graph = graph;

	for(auto a : plaintext)
	{
        vector<unsigned char> inputRound = { a, key };
        ciphertext.push_back(cgpAlice.propagate(inputRound));
	}

    //cout << fitnessFunction(plaintext, ciphertext);
    runCGP(plaintext, ciphertext, key); //prepravi nazive
    return 0;
}