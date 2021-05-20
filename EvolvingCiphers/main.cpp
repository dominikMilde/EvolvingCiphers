#include <bitset>
#include <iostream>
#include "cgp.hpp"
#include "configs.hpp"
#include "evolution.hpp"

using namespace std;

int main(void)
{
	CGP cgpAlice(numInputs, numOutputs, numRows, numColumns, numNodeInputs);
    srand(time(NULL));
	cout << "stvaram pocetni skup" << endl;
	Key p;
	Key k;

    vector<vector<unsigned char>> plaintexts;
	vector<vector<unsigned char>> keys;

	for(int i = 0; i<sizeLearningSet; i++)
	{
		p.key = randomKey();
		k.key = randomKey();

		plaintexts.push_back(p.toChar());
		keys.push_back(k.toChar());
	}
	
	Graph alice = Graph(randomGraph(), 0);
	cgpAlice.graph = alice.graph;

	vector<vector<unsigned char>> ciphertexts;

	for(int i = 0; i < keys.size(); i++)
	{
		ciphertexts.push_back(cgpAlice.generateCipher(plaintexts.at(i), keys.at(i)));
	}

	//double f = fitnessFunctionMultiple(ciphertexts, plaintexts);
	//cout << f << endl;
	Graph bob = evaluateBob(plaintexts, keys, ciphertexts);
	
}