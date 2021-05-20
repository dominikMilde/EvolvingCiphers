#include "evolution.hpp"
#include <iostream>
#include <bitset>
#include <random>

#include "configs.hpp"
#include "key.hpp"

CGP cgp(numInputs, numOutputs, numRows, numColumns, numNodeInputs);

vector<Graph> graphsBob;
Graph bestBob;

vector<Key> keyPopulation;
Key bestEvaKey;

minstd_rand randomEngineCGP;


double fitnessIndividual(vector <int> &graph, vector <vector <unsigned char>> &keys, vector <vector <unsigned char>> &plaintexts, vector <vector <unsigned char>> &ciphertexts)
{
	cgp.graph = graph;
	vector <vector <unsigned char>> runnedOutputs;
	for (int i=0; i < keys.size(); i++)
	{
		vector<unsigned char> out;
		for(int j=0; j<keys.at(i).size(); j++)
		{
			vector<unsigned char> forCgpIn = { ciphertexts.at(i).at(j), keys.at(i).at(j) };
			out.push_back(cgp.propagate(forCgpIn));
		}
		runnedOutputs.push_back(out);
		
	}
	//cout << runnedOutputs[0] << endl;
	return fitnessFunctionMultiple(plaintexts, runnedOutputs);
}

double fitnessFunctionMultiple(vector <vector<unsigned char>>& inputs, vector <vector<unsigned char>> & outputs)
{
	double sum = 0;
	for(int i = 0; i<inputs.size(); i++)
	{
		sum += fitnessFunction(inputs.at(i), outputs.at(i));
	}
	return sum * 1.0 / inputs.size();
}

double fitnessFunction(vector<unsigned char>& inputs, vector<unsigned char>& outputs)
{
	double fitness = 0;
	for (int i = 0; i < inputs.size(); i++)
	{
		unsigned char xored = inputs.at(i) ^ outputs.at(i);
		fitness += (8 - bitset<8>(xored).count());

	}
	return fitness / (8.0 * inputs.size());
}

Graph findBestGraph(vector<Graph>& graphs)
{

	int idxBest = 0;
	for (int i = 1; i < graphs.size(); i++) {
		if (graphs.at(i).fitness >= graphs.at(idxBest).fitness) {
			idxBest = i;
		}
	}
	return graphs.at(idxBest);
}
Key findBestKey(vector<Key>& keys)
{

	int idxBest = 0;
	for (int i = 1; i < keys.size(); i++) {
		if (keys.at(i).fitness > keys.at(idxBest).fitness) {
			idxBest = i;
		}
	}
	return keys.at(idxBest);
}
vector<int> crossover(vector<int>& mainGraph, vector<int>& otherGraph)
{
	vector<int> child1 = mainGraph;
	int index = rand() % (numRows * numColumns);
	int startOfCut = index * (1 + numNodeInputs);
	for (startOfCut; startOfCut < mainGraph.size(); startOfCut++)
	{
		child1.at(startOfCut) = otherGraph.at(startOfCut);
	}
	return child1;
}

vector<bool> crossoverKey(vector<bool>& firstKey, vector<bool>& secondKey)
{
	vector<bool> child = firstKey;
	int index = rand() % (keyLength*8);

	for (index; index < keyLength*8; index++)
	{
		child.at(index) = secondKey.at(index);
	}
	return child;
}

Graph crossAndReturnBestOfThree(Graph &firstParent, Graph &secondParent, vector <vector <unsigned char>>& keys, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts)
{
	Graph child1(crossover(firstParent.graph, secondParent.graph), NULL);
	child1.fitness = fitnessIndividual(child1.graph, keys, plaintexts, ciphertexts);

	Graph child2(crossover(secondParent.graph, firstParent.graph), NULL);
	child2.fitness = fitnessIndividual(child1.graph, keys, plaintexts, ciphertexts);

	if(child2.fitness > child1.fitness)
	{
		return child2;
	}
	return child1;
		
}

/*
Key crossAndReturnBestOfThreeKey(Key &firstParent, Key &secondParent, vector <unsigned char>& plaintext, vector <unsigned char>& ciphertext, Graph& bob)
{
	Key child(crossoverKey(firstParent.key, secondParent.key), NULL);
	vector<unsigned char> kChar = child.toChar();
	child.fitness = fitnessIndividual(bob.graph, kChar, plaintext, ciphertext);

	Key betterParent = firstParent;
	if (secondParent.fitness > firstParent.fitness)
	{
		betterParent = secondParent;
	}

	if (child.fitness > betterParent.fitness)
	{
		return child;
	}
	return betterParent;
}*/

Graph mutation(Graph& graphStruct, vector <vector <unsigned char>>& keys, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts)
{
	int graphSize = graphStruct.graph.size();
	vector<int> graph = graphStruct.graph;
	
	int index = rand() % (graphSize);

	if (index < graphSize - numOutputs)
	{
		if (index % (numNodeInputs + 1) == 0)
		{
			int functionId = rand() % numFunctions;
			graph[index] = functionId;
		}
		else
		{
			int indOfColumn = index / (numRows * (1 + numNodeInputs));
			int currMaxNodeOut = numInputs + indOfColumn * numRows;
			graph[index] = rand() % currMaxNodeOut;
		}
	}
	else
	{
		int outRandom = rand() % (numInputs + numColumns * numRows);
		graph[index] = outRandom;
	}
	
	
	
	double fitness = fitnessIndividual(graph, keys, plaintexts, ciphertexts);
	Graph mutatedGraph(graph, fitness);
	
	return mutatedGraph; //novi!!!
}

/*
Key mutationKey(Key& key, vector <unsigned char>& plaintext, vector <unsigned char>& ciphertext, Graph& bob)
{
	int index = rand() % (keyLength * 8);
	if (key.key.at(index))
	{
		key.key.at(index) = false;
	}
	else
	{
		key.key.at(index) = true;
	}

	vector<unsigned char> kChar = key.toChar();
	double fitness = fitnessIndividual(bob.graph, kChar, plaintext, ciphertext);
	Key retKey(key.key, fitness);

	return retKey;
}
*/
vector<int> randomGraph()
{
	vector<int> graph;

	for (int i = 0; i < numColumns; i++)
	{
		int currMaxNodeOut = numInputs + i * numRows;
		for (int j = 0; j < numRows; j++)
		{
			graph.push_back(rand() % numFunctions);
			for (int k = 0; k < numNodeInputs; k++)
			{
				graph.push_back(rand() % currMaxNodeOut);
			}
		}
	}

	int maxNodeOut = numInputs + numRows * numColumns;
	for (int i = 0; i < numOutputs; i++)
	{
		graph.push_back(maxNodeOut/2 + rand()%(maxNodeOut/2));
	}
	return graph;
}

vector<bool> randomKey()
{
	vector<bool> key;
	for(int i=0; i<8*keyLength; i++)
	{
		int a = rand() % 2;
		if(a == 1)
		{
			key.push_back(true);
		}
		else
		{
			key.push_back(false);
		}
	}
	return key;
}

//prebaciti u računanje fitnesa vector vector
void fillInitialPopulationCGP(vector<Graph> &graphs, vector <vector <unsigned char>>& keys, vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& ciphertexts)
{
	graphs.clear();
	graphs.reserve(populationSize);

	//cout << "Generating initial population..." << endl;
	for (int i = 1; i <= populationSize; i++)
	{
		vector<int> rG = randomGraph();
		double fitness = fitnessIndividual(rG, keys, plaintexts, ciphertexts);
		Graph randGraph = Graph(rG, fitness);
		graphs.push_back(randGraph);
	}
}
/*
void fillInitialPopulationKeys(vector<Key>& keys, Graph &bob, vector <unsigned char>& plaintext, vector <unsigned char>& ciphertext)
{
	keys.clear();
	keys.reserve(populationSize);

	//cout << "Generating initial population..." << endl;
	for (int i = 1; i <= populationSize; i++)
	{
		vector<bool> rK = randomKey();
		Key k = Key(rK, 0);
		vector<unsigned char>key = k.toChar();
		double fitness = fitnessIndividual(bob.graph, key, plaintext, ciphertext);
		
		k.fitness = fitness;
		//k.printKey();

		keys.push_back(k);
	}
}*/

void print(Graph& g) {
	int graphSize = g.graph.size();
	for (int i = 0; i < graphSize; i++)
	{
		cout << g.graph.at(i) << " ";
	}

	cout	<< "CGP Fitness: " << g.fitness << endl;
}

Graph evaluateBob(vector <vector <unsigned char>>& plaintexts, vector <vector <unsigned char>>& keys, vector <vector <unsigned char>>& ciphertexts)
{
	fillInitialPopulationCGP(graphsBob, keys, plaintexts, ciphertexts);
	cout << "Generation: 0" << endl;
	bestBob = findBestGraph(graphsBob);
	cout << "Score of best individual BOB: " << bestBob.fitness << endl;
	//cout << "Genes of the best individual BOB: ";
	for (auto b : graphsBob)
	{
		print(b);
	}
	for (int generation = 0; generation < generations; generation++)
	{
		for (int n = 0; n < crossoversInGeneration; n++)
		{
			vector<int> idsToCross; //ids from population
			while (idsToCross.size() < tournamentSize)
			{
				int id = rand() % populationSize;
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
				if (graphsBob.at(idsToCross.at(i)).fitness <= graphsBob.at(idOfWorst).fitness) {
					idOfWorst = idsToCross.at(i);
				}
			}
			int indexOfFirst, indexOfSecond;
			do {
				indexOfFirst = rand() % tournamentSize;
			} while (idsToCross.at(indexOfFirst) == idOfWorst);
			do
			{
				indexOfSecond = rand() % tournamentSize;
			} while (idsToCross.at(indexOfSecond) == idOfWorst || idsToCross.at(indexOfSecond) == idsToCross.at(indexOfFirst));
			Graph offspring = crossAndReturnBestOfThree(graphsBob.at(indexOfFirst), graphsBob.at(indexOfSecond), keys, plaintexts,ciphertexts);
			graphsBob.at(idOfWorst) = offspring;
			double probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

			if (probability < mutationProbability)
			{
				Graph mutated = mutation(graphsBob.at(idOfWorst), keys, plaintexts, ciphertexts);
				graphsBob.at(idOfWorst) = mutated;
			}
		}

		cout << "Generation: " << generation + 1 << endl;
		bestBob = findBestGraph(graphsBob);
		cout << "Score of best individual BOB: " << bestBob.fitness << endl;
		//cout << "Genes of the best individual BOB: ";
		for(auto b : graphsBob)
		{
			print(b);
		}

		cout << "------------------------------------" << endl;

		if (bestBob.fitness > 0.99) {
			return bestBob;
		}
	}
	return bestBob;
}
/*
Key evaluateEva(Graph& bob, vector <unsigned char>& plaintext, vector <unsigned char>& ciphertext)
{

	fillInitialPopulationKeys(keyPopulation, bob, plaintext, ciphertext);
	for (int generation = 0; generation < generations; generation++)
	{
		for (int n = 0; n < crossoversInGeneration; n++)
		{
			vector<int> idsToCross; //ids from population
			while (idsToCross.size() < tournamentSize)
			{
				int id = rand() % populationSize;
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
				if (keyPopulation.at(idsToCross.at(i)).fitness < keyPopulation.at(idOfWorst).fitness) {
					idOfWorst = idsToCross.at(i);
				}
			}
			int indexOfFirst, indexOfSecond;
			do {
				indexOfFirst = rand() % tournamentSize;
			} while (idsToCross.at(indexOfFirst) == idOfWorst);
			do
			{
				indexOfSecond = rand() % tournamentSize;
			} while (idsToCross.at(indexOfSecond) == idOfWorst || idsToCross.at(indexOfSecond) == idsToCross.at(indexOfFirst));
			Key offspring = crossAndReturnBestOfThreeKey(keyPopulation.at(indexOfFirst), keyPopulation.at(indexOfSecond), plaintext, ciphertext, bob);
			keyPopulation.at(idOfWorst) = offspring;
			double probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

			if (probability < mutationProbability)
			{
				Key mutated = mutationKey(keyPopulation.at(idOfWorst), plaintext, ciphertext, bob);
				keyPopulation.at(idOfWorst) = mutated;
			}
		}

		//cout << "Generation: " << generation + 1 << endl;
		bestEvaKey = findBestKey(keyPopulation);
		//cout << "Score of best individual BOB: " << bestBob.fitness << endl;
		//cout << "Genes of the best individual BOB: ";
		//print(bestBob);
		//cout << endl;

		//cout << "------------------------------------" << endl;

		if (bestEvaKey.fitness > 0.99) {
			return bestEvaKey;
		}
	}
	return bestEvaKey;
}*/
double calculateFitness(double b, double e)
{
	return b - e; //magic number
}
/*
double rateAlice(vector<int>& aliceGraph, vector<unsigned char>& plaintext, vector<unsigned char>& key)
{
	cgp.graph = aliceGraph;
	//cout << "ARHITEKTURA ALICE:\n";
	//for (int j = 0; j < alice.graph.size(); j++)
	//{
	//	cout << alice.graph[j];
	//}
	//cout << "\n";
	vector<unsigned char> ciphertext = cgp.generateCipher(plaintext, key);
	//cout << "CIPHER:\n";
	//for (auto a : ciphertext)
	//{
	//	cout << bitset<8>(a) << endl;
	//}
	Graph bob = evaluateBob(plaintext, key, ciphertext);
	//cout << "najbolji bob:" << bob.fitness << endl;
	//cout << "arhitektura boba: ";
	//print(bob);
	Key eva = evaluateEva(bob, plaintext, ciphertext);
	//cout << "najbolja eva:" << eva.fitness << endl;
	//eva.printKey();
	//cout << endl;
	//cout << "FITNES ALICE: " << bob.fitness - eva.fitness << endl;
	return calculateFitness(bob.fitness, eva.fitness);
}*/

/*
Graph crossAndReturnBestOfThreeAlice(Graph& firstParent, Graph& secondParent, vector<vector<unsigned char>>& keys, vector<vector<unsigned char>>& plaintexts)
{
	Graph child(crossover(firstParent.graph, secondParent.graph), NULL);
	double fitnessSum = 0;
	for(int i = 0; i< keys.size(); i++)
	{
		fitnessSum += rateAlice(child.graph, keys.at(i), plaintexts.at(i));
	}
	
	child.fitness = fitnessSum * 1.0 / keys.size();

	Graph betterParent = firstParent;
	if (secondParent.fitness > firstParent.fitness)
	{
		betterParent = secondParent;
	}

	if (child.fitness > betterParent.fitness)
	{
		return child;
	}
	return betterParent;
}
*/

/*
Graph mutationAlice(Graph& alice, vector<vector<unsigned char>>& keys, vector<vector<unsigned char>>& plaintexts)
{
	int graphSize = alice.graph.size();
	vector<int> graph = alice.graph;

	int index = rand() % (graphSize);

	if (index < graphSize - numOutputs)
	{
		if (index % (numNodeInputs + 1) == 0)
		{
			int functionId = rand() % numFunctions;
			graph[index] = functionId;
		}
		else
		{
			int indOfColumn = index / (numRows * (1 + numNodeInputs));
			int currMaxNodeOut = numInputs + indOfColumn * numRows;
			graph[index] = rand() % currMaxNodeOut;
		}
	}
	else
	{
		int outRandom = rand() % (numInputs + numColumns * numRows);
		graph[index] = outRandom;
	}

	double fitnessSum = 0;
	for (int i = 0; i < keys.size(); i++)
	{
		fitnessSum += rateAlice(graph, keys.at(i), plaintexts.at(i));
	}

	double fitness = fitnessSum * 1.0 / keys.size();


	Graph mutatedGraph(graph, fitness); 

	return mutatedGraph; //novi!!!
}*/