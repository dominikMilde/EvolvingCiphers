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
	k.printKey();

	cgpAlice.graph = { 1, 1, 1, 2, 2, 2, 3, 3, 1, 7, 4, 0, 6, 1, 0, 2, 1, 0, 6, 4, 7, 4, 2, 0, 6, 6, 0, 3, 0, 8, 7, 6, 0, 2, 3, 1, 5, 11, 0, 2, 13, 6, 3, 2, 7, 4, 9, 16, 1, 17, 3, 3, 10, 12, 4, 2, 9, 6, 16, 19, 14 };
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

	vector<int> bobgraph = { 2, 0, 0, 1, 0, 1, 4, 0, 3, 0, 2, 2, 4, 3, 3, 3, 2, 0, 2, 1, 5, 0, 4, 6, 3, 3, 8, 7, 4, 1, 3, 3, 8, 7, 12, 10, 5, 6, 2, 2, 14, 3, 1, 7, 15, 6, 13, 2, 7, 7, 14, 0, 3, 5, 2, 14, 18, 0 ,17, 17, 11 };
	Graph bob(bobgraph, NULL);

	Key eva = evaluateEva(bob, plaintexts, ciphertexts);
}

	
