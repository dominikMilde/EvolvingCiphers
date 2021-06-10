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
	Key p;
	Key k;

	vector<vector<unsigned char>> plaintexts;

	for (int i = 0; i < sizeTestingSet; i++)
	{
		p.key = randomKey();
		plaintexts.push_back(p.toChar());
	}

	k.key = randomKey();
	vector <unsigned char>key = k.toChar();

	cgpAlice.graph = randomGraph();
	for (auto a : cgpAlice.graph) {
		cout << a << " ";
	}
	cout << endl;

	vector <vector<unsigned char>> ciphertexts;
	for (int i = 0; i < plaintexts.size(); i++) {
		vector <unsigned char> c = cgpAlice.generateCipher(plaintexts.at(i), key);
		ciphertexts.push_back(c);
	}

	double f = fitnessFunctionMultiple(plaintexts, ciphertexts);
	cout << "Slicnost plaina i ciphera: " << f << endl;

	Graph bob = evaluateBob(plaintexts, key, ciphertexts);
}

	
