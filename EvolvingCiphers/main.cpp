#include <bitset>
#include <iostream>
#include "cgp.hpp"
#include "configs.hpp"
#include "evolution.hpp"

using namespace std;

int main(void)
{
	CGP cgpAlice(numInputs, numOutputs, numRows, numColumns, numNodeInputs);
	CGP cgpBob(numInputs, numOutputs, numRows, numColumns, numNodeInputs);

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
	
	cgpAlice.graph = { 2, 0, 1, 3, 1, 1, 2, 3, 2, 2, 2, 4, 2, 3, 3, 5, 1, 1, 3, 2, 1, 3, 8, 8, 4, 1, 7, 2, 3, 5, 0, 3, 4, 4, 5, 9, 3, 9, 8, 5, 6, 5, 2, 15, 6, 1, 2, 2, 4, 8, 16, 3, 8, 18, 5, 14, 19, 4, 7, 17, 14 };


	vector <vector<unsigned char>> ciphertexts;
	for(int i = 0; i < plaintexts.size(); i++){
		vector <unsigned char> c = cgpAlice.generateCipher(plaintexts.at(i), key);
		ciphertexts.push_back(c);
	}

	double f = fitnessFunctionMultiple(plaintexts, ciphertexts);
	cout << "Slicnost plaina i ciphera: " << f << endl;

	cgpBob.graph = {1, 1, 1, 2, 0, 2, 0, 1, 0, 0, 2, 0, 3, 4, 0, 2, 0, 3, 4, 7, 0, 1, 3, 6, 5, 5, 5, 4, 4, 10, 5, 6, 6, 1, 1, 7, 1, 4, 0, 3 ,6, 6, 1, 10, 13, 2, 8, 16, 4, 12, 15, 3, 2, 3, 2, 1, 2, 4, 6, 5, 19 };

	vector <vector<unsigned char>> plaintextBob;
	for (int i = 0; i < plaintexts.size(); i++) {
		vector <unsigned char> c = cgpBob.generateCipher(ciphertexts.at(i), key);
		plaintextBob.push_back(c);
	}

	double g = fitnessFunctionMultiple(plaintexts, plaintextBob);
	cout << "plain i bobov plain: " << g << endl;

	Graph bob(cgpBob.graph, 0);

	Key e = evaluateEva(bob, plaintexts, ciphertexts);
	cout << "Eva: " << e.fitness << endl;

	vector <vector<unsigned char>> ciphertextsOneOff;
	plaintexts.at(0).at(0) += 1;
	for (int i = 0; i < plaintexts.size(); i++) {
		vector <unsigned char> c = cgpAlice.generateCipher(plaintexts.at(i), key);
		ciphertextsOneOff.push_back(c);
	}
	plaintexts.at(0).at(0) -= 1;
	double h = fitnessFunctionMultiple(ciphertexts, ciphertextsOneOff);
	cout << "Slicnost za jedan bit promjene " << h << endl;

	cout << "--------------------------------------------- uvodim runde" << endl;

	vector <vector <unsigned char>> cipherAfterTwoRounds;

	vector <vector <unsigned char>> lastRoundPlain = ciphertexts;
	for (int i = 0; i < plaintexts.size(); i++) {
		vector<unsigned char> cR = cgpAlice.generateCipher(ciphertexts.at(i), key);
		cipherAfterTwoRounds.push_back(cR);
	}


	vector <vector <unsigned char>> plainBobAfterOneRound;
	for (int i = 0; i < plaintexts.size(); i++) {
		vector<unsigned char> pl = cgpAlice.generateCipher(cipherAfterTwoRounds.at(i), key);
		plainBobAfterOneRound.push_back(pl);
	}

	vector <vector <unsigned char>> plainBobAfterTwoRound;
	for (int i = 0; i < plaintexts.size(); i++) {
		vector<unsigned char> pl = cgpAlice.generateCipher(plainBobAfterOneRound.at(i), key);
		plainBobAfterTwoRound.push_back(pl);
	}
	double twoRound = fitnessFunctionMultiple(plainBobAfterTwoRound, plaintexts);
	cout << "dvije runde: plain i Bob plain: " << twoRound << endl;
}
