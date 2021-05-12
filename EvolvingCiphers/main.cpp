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
	CGP cgpEva(numInputs, numOutputs, numRows, numColumns, numNodeInputs);

	Key p;
	Key k;
	p.key = randomKey();
	k.key = randomKey();

    vector<unsigned char> plaintext = p.toChar();
    vector<unsigned char> key =      k.toChar();
	
    vector<Graph> alicePopulation;
	Graph bestAlice;
	alicePopulation.reserve(populationSizeA);
	//generating population of Alice
	for(int i = 0; i<populationSizeA; i++)
	{
		alicePopulation.push_back(Graph(randomGraph(), 0));
		
	}
	//rating fitness of created Alice
	for (Graph alice : alicePopulation)
	{
		alice.fitness = rateAlice(alice.graph, plaintext, key);
	}
	//evolution algorithm
	for (int generation = 0; generation < generationsA; generation++)
	{
		for (int n = 0; n < crossoversInTournamentA; n++)
		{
			vector<int> idsToCross;
			for (int t = 0; t < tournamentSizeA; t++)
			{
				int id = rand() % populationSizeA;
				idsToCross.push_back(id);
			}
			int idOfWorst = idsToCross.at(0);
			for (int i = 0; i < idsToCross.size(); i++)
			{
				if (alicePopulation.at(idsToCross.at(i)).fitness <= alicePopulation.at(idOfWorst).fitness) {
					idOfWorst = idsToCross.at(i);
				}
			}
			int indexOfFirst, indexOfSecond;
			do {
				indexOfFirst = rand() % tournamentSizeA;
			} while (idsToCross.at(indexOfFirst) == idOfWorst);
			do
			{
				indexOfSecond = rand() % tournamentSizeA;
			} while (idsToCross.at(indexOfSecond) == idOfWorst || idsToCross.at(indexOfSecond) == idsToCross.at(indexOfFirst));
			Graph offspring = crossAndReturnBestOfThreeAlice(alicePopulation.at(indexOfFirst), alicePopulation.at(indexOfSecond), key, plaintext);
			alicePopulation.at(idOfWorst) = offspring;
			double probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

			if (probability < mutationProbabilityA)
			{
				Graph mutated = mutationAlice(alicePopulation.at(idOfWorst), key, plaintext);
				alicePopulation.at(idOfWorst) = mutated;
			}
		}

		cout << "Generation: " << generation + 1 << endl;
		//for (auto a : alicePopulation)
		//{
		//	print(a);
		//}
		bestAlice = findBestGraph(alicePopulation);
		cout << "FITNESS Alice: " << bestAlice.fitness << endl;
		print(bestAlice);

		if (bestAlice.fitness > 0.49) {
			break;
		}
	}
	print(bestAlice);
	cout << "fitness: " << bestAlice.fitness << endl;

	cout << " IDEM PRINTATI RANDOMIZIRANE!!!! +++++++++++++++++++" << endl;
	for(int i = 0; i<10; i++)
	{
		p.key = randomKey();
		k.key = randomKey();

		plaintext = p.toChar();
		key = k.toChar();

		double fitnessRand = rateAlice(bestAlice.graph, plaintext, key);
		cout << fitnessRand << endl;
	}
}