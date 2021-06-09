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

	for(int i = 0; i<sizeLearningSet; i++)
	{
		p.key = randomKey();
		plaintexts.push_back(p.toChar());
	}

	k.key = randomKey();
	vector <unsigned char>key = k.toChar();


	//print(alice);
	/*
	vector<vector<unsigned char>> ciphertexts;
	for(int i=0; i<plaintexts.size(); i++)
	{
		vector <unsigned char> c = cgpAlice.generateCipher(plaintexts.at(i), key);
		ciphertexts.push_back(c);
	}


	Graph bob = evaluateBob(plaintexts, key, ciphertexts);
	
	cgpAlice.graph = bob.graph;

	vector<vector<unsigned char>> ciphertexts1;
	for (int i = 0; i < plaintexts.size(); i++)
	{
		vector <unsigned char> c = cgpAlice.generateCipher(ciphertexts.at(i), key);
		ciphertexts1.push_back(c);
	}
	
	double provjera = fitnessFunctionMultiple(plaintexts, ciphertexts1);
	cout << "PROV:" << provjera << endl;
	*/

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
	
	for (Graph& alice : alicePopulation)
	{
		double fitness = rateAlice(alice.graph, plaintexts, key);
		alice.fitness = fitness;
	}
	cout << "zavrsio ocjenjivanje" << endl;
	
	//evolution algorithm
	for (int generation = 0; generation < generationsA; generation++)
	{
		cout << "Generation: " << generation << endl;
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

		
	}
	
	print(bestAlice);
	cout << "\nfitness: " << bestAlice.fitness << endl;
	

}

	
