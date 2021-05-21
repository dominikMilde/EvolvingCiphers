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
	//Key p;
	//Key k;

    vector<vector<unsigned char>> plaintexts;

	vector <unsigned char> pl = { 0b10011000 };
	vector<unsigned char> key = { 0b11110000 };

	plaintexts.push_back(pl);

	//for(int i = 0; i<sizeLearningSet; i++)
	//{
		//p.key = randomKey();
		//plaintexts.push_back(p.toChar());
//	}

	//k.key = randomKey();
	//key = k.toChar();

	vector <int> graf = { 3,0,1,5,2,1,3 };
	Graph alice = Graph(graf, 0);
	cgpAlice.graph = alice.graph;

	print(alice);

	vector<vector<unsigned char>> ciphertexts;
	for(int i=0; i<plaintexts.size(); i++)
	{
		vector <unsigned char> c = cgpAlice.generateCipher(plaintexts.at(i), key);
		ciphertexts.push_back(c);
	}

	bitset<8> bt{ ciphertexts[0][0] };
	cout << bt << endl;

	graf = { 4,0,0,3,2,1,3};
	cgpAlice.graph = graf;

	vector <unsigned char> poc = cgpAlice.generateCipher(ciphertexts[0], key);

	bitset<8> poce{ poc[0] };
	cout << poce;
	//evaluateBob(plaintexts, key, ciphertexts);
	
	/*
	vector<Graph> alicePopulation;
	Graph bestAlice;
	
	alicePopulation.reserve(populationSizeA);

	//generating population of Alice
	for (int i = 0; i < populationSizeA; i++)
	{
		alicePopulation.push_back(Graph(randomGraph(), 0));
	}

	cout << "ocjenjujem pocetni" << endl;
	//rating fitness of created Alice
	
	for (Graph alice : alicePopulation)
	{
		double fitness = rateAlice(alice.graph, plaintexts, key);
		alice.fitness = fitness;
	}
	cout << "zavrsio ocjenjivanje" << endl;
	
	//evolution algorithm
	for (int generation = 0; generation < generationsA; generation++)
	{
		for (int n = 0; n < tournamentSizeA; n++)
		{
			int indexOfFirst, indexOfSecond;
			indexOfFirst = rand() % populationSizeA;

			do {
				indexOfSecond = rand() % populationSizeA;
			} while (indexOfFirst == indexOfSecond);

			Graph offspring = crossAndReturnBestOfThreeAlice(alicePopulation.at(indexOfFirst), alicePopulation.at(indexOfSecond), key, plaintexts);
			double probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

			if (probability < mutationProbability)
			{
				offspring = mutationAlice(offspring, key, plaintexts);
			}

			int indexOfSmallerFit = indexOfFirst;
			double fitnessOfSmaller = alicePopulation.at(indexOfFirst).fitness;

			if (alicePopulation.at(indexOfSecond).fitness < fitnessOfSmaller)
			{
				fitnessOfSmaller = alicePopulation.at(indexOfSecond).fitness;
				indexOfSmallerFit = indexOfSecond;
			}

			if (offspring.fitness > fitnessOfSmaller)
			{
				alicePopulation.at(indexOfSmallerFit) = offspring;
			}
		}

		cout << "Generation: " << generation + 1 << endl;
		for (auto a : alicePopulation)
		{
			print(a);
		}
		bestAlice = findBestGraph(alicePopulation);
		cout << "FITNESS Alice: " << bestAlice.fitness << endl;
		print(bestAlice);

		if (bestAlice.fitness > 0.49) {
			break;
		}
	}
	
	print(bestAlice);
	cout << "\nfitness: " << bestAlice.fitness << endl;
	*/

}

	
