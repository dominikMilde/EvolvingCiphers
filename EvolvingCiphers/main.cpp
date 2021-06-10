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

	cgpAlice.graph = { 2, 1, 1, 5, 0, 2, 0, 0, 1, 1, 0, 0, 3, 5, 2, 5, 3, 3 ,3 ,4 ,6 ,5, 6, 8, 3, 6, 9, 5, 4, 3, 0, 8, 7, 5, 11, 8, 5, 3, 2, 2, 9, 8, 2, 1, 3, 4, 12, 6, 3, 11, 13, 3, 6, 16, 1, 18, 17, 4, 9, 14, 21 };
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

	vector<int> bobgraph = { 0, 1, 0, 4, 2, 0, 4, 3, 2, 4, 4, 1, 1, 1, 0, 2, 5, 1, 1, 7, 5, 5, 0, 3, 3, 0, 3, 3, 1, 0, 2, 11, 10, 1, 10, 5, 0, 8, 1, 2, 11, 3, 4, 10, 8, 2, 11, 10, 5, 6, 0, 3, 3, 3, 5, 10, 7, 0, 20, 4, 1 };
	Graph bob(bobgraph, NULL);

	Key eva = evaluateEva(bob, plaintexts, ciphertexts);
}

	
