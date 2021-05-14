#include <bitset>
#include <iostream>
#include "cgp.hpp"
#include "configs.hpp"
#include "evolution.hpp"

using namespace std;

int main(void)
{
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
	
    vector<Graph> alicePopulation;
	Graph bestAlice;
	alicePopulation.reserve(populationSizeA);
	
	//generating population of Alice
	for(int i = 0; i<populationSizeA; i++)
	{
		alicePopulation.push_back(Graph(randomGraph(), 0));	
	}
	
	cout << "ocjenjujem pocetni" << endl;
	//rating fitness of created Alice
	for (Graph alice : alicePopulation)
	{
		double fitnessSum = 0;
		for(int i=0; i<keys.size(); i++)
		{
			fitnessSum += rateAlice(alice.graph, plaintexts.at(i), keys.at(i));
		}
		alice.fitness = fitnessSum * 1.0 / keys.size();
	}
	
	//evolution algorithm
	for (int generation = 0; generation < generationsA; generation++)
	{
		for (int n = 0; n < crossoversInGenerationA; n++)
		{
			vector<int> idsToCross; //ids from population
			while(idsToCross.size() < tournamentSizeA)
			{
				int id = rand() % populationSizeA;
				if (std::find(idsToCross.begin(), idsToCross.end(), id) != idsToCross.end()) {
					//already in vector
				}
				else {
					idsToCross.push_back(id);
				}
			}
			int idOfWorst = idsToCross.at(0);
			for (int i = 0; i < idsToCross.size(); i++)
			{
				if (alicePopulation.at(idsToCross.at(i)).fitness < alicePopulation.at(idOfWorst).fitness) {
					idOfWorst = idsToCross.at(i);
				}
			}
			//cout << "\n najgora alice: ";
			//print(alicePopulation.at(idOfWorst));
			//cout << endl;
			int indexOfFirst, indexOfSecond;
			do {
				indexOfFirst = rand() % tournamentSizeA;
			} while (idsToCross.at(indexOfFirst) == idOfWorst);
			do
			{
				indexOfSecond = rand() % tournamentSizeA;
			} while (idsToCross.at(indexOfSecond) == idOfWorst || idsToCross.at(indexOfSecond) == idsToCross.at(indexOfFirst));
			Graph offspring = crossAndReturnBestOfThreeAlice(alicePopulation.at(indexOfFirst), alicePopulation.at(indexOfSecond), keys, plaintexts);
			alicePopulation.at(idOfWorst) = offspring;
			double probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

			if (probability < mutationProbabilityA)
			{
				Graph mutated = mutationAlice(alicePopulation.at(idOfWorst), keys, plaintexts);
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

	cout << "PRINTAM RANDOMIZIRANE INPUTE:" << endl;
	
	vector<vector<unsigned char>> plaintextsRand;
	vector<vector<unsigned char>> keysRand;

	for (int i = 0; i < sizeTestingSet; i++)
	{
		p.key = randomKey();
		k.key = randomKey();

		plaintextsRand.push_back(p.toChar());
		keysRand.push_back(k.toChar());
	}

	for(int i = 0; i<keysRand.size(); i++)
	{
		double fitnessSum = 0;
		for (int i = 0; i < keysRand.size(); i++)
		{
			fitnessSum += rateAlice(bestAlice.graph, plaintextsRand.at(i), keysRand.at(i));
		}
		cout << fitnessSum * 1.0 / keysRand.size() << endl;
	}
}