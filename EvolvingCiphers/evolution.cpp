#include "evolution.hpp"
#include <iostream>
#include <bitset>
#include <random>

#include "configs.hpp"

CGP cgp(numInputs, numOutputs, numRows, numColumns, numNodeInputs);

vector<Graph> graphsBob;
vector<Graph> graphsEve;

Graph bestBob;
Graph bestEve;

minstd_rand randomEngineCGP;

vector<unsigned char> plaintext;
vector<unsigned char> ciphertext;
unsigned char key;

double fitnessIndividual(vector <int> graph, bool giveKey)
{
	cgp.graph = graph;
	vector <unsigned char> runnedOutputs;
	for (auto cipher : ciphertext)
	{
		//cout << a << endl;
		if (giveKey)
		{
			vector<unsigned char> forCgpIn = { cipher, key };
			runnedOutputs.push_back(cgp.propagate(forCgpIn));
		}
		else
		{
			vector<unsigned char> forCgpIn = { cipher, cipher };
			runnedOutputs.push_back(cgp.propagate(forCgpIn));
		}
		
	}
	//cout << runnedOutputs[0] << endl;
	return fitnessFunction(plaintext, runnedOutputs);
}

double fitnessFunction(vector<unsigned char> inputs, vector<unsigned char> outputs)
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
	for (int i = 1; i < populationSize; i++) {
		if (graphs.at(i).fitness > graphsBob.at(idxBest).fitness) {
			idxBest = i;
		}
	}
	return graphs.at(idxBest);
}
vector<int> crossover(vector<int>& mainGraph, vector<int>& otherGraph)
{
	vector<int> child = mainGraph;
	int index = rand() % (numRows * numColumns);
	int startOfCut = index * (1 + numNodeInputs);
	for (startOfCut; startOfCut < mainGraph.size(); startOfCut++)
	{
		child.at(startOfCut) = otherGraph.at(startOfCut);
	}
	return child;
}

Graph crossAndReturnBestOfThree(Graph firstParent, Graph secondParent, bool giveKey)
{
	Graph child(crossover(firstParent.graph, secondParent.graph), NULL);
	child.fitness = fitnessIndividual(child.graph, giveKey);

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

Graph mutation(Graph graphStruct, bool giveKey)
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
	
	
	
	double fitness = fitnessIndividual(graph, giveKey);
	Graph mutatedGraph(graph, fitness);
	
	return mutatedGraph; //novi!!!
}

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
		graph.push_back(rand() % maxNodeOut);
	}
	return graph;
}


void fillInitialPopulationCGP(vector<Graph> &graphs, bool giveKey)
{
	cout << endl;
	graphs.clear();
	graphs.reserve(populationSize);

	cout << "Generating initial population..." << endl;
	for (int i = 1; i <= populationSize; i++)
	{
		vector<int> rG = randomGraph();
		//cout << "prva tri:" << rG[0] << rG[1] << rG[2] << endl;
		
		
		//cout << "pop #" << i << ": ";
		double fitness = fitnessIndividual(rG, giveKey);
		//cout << "fitness: " << fitness << endl;
		Graph randGraph = Graph(rG, fitness);
		graphs.push_back(randGraph);
	}
}


void print(Graph& g) {
	int graphSize = g.graph.size();
	for (int i = 0; i < graphSize - numOutputs; i++)
	{
		if (i % (numNodeInputs + 1) == 0)
			cout << endl;
		cout << g.graph[i] << " ";
	}
	cout << endl << "Output: ";
	for (int i = graphSize - numOutputs; i < graphSize; i++)
	{
		cout << g.graph[i] << " ";
	}

	cout << endl
		<< "CGP Fitness: " << g.fitness << endl << endl << endl;
}


Graph runCGP(vector<unsigned char> plainF, vector<unsigned char> cipherF, unsigned char keyF)
{

	plaintext = plainF;
	ciphertext = cipherF;
	key = keyF;
	
	fillInitialPopulationCGP(graphsBob, true);
	//SteadyStateTournament
	for (int generation = 0; generation < generations; generation++)
	{
		for(int n=0; n< crossoversInTournament; n++)
		{
			vector<int> idsToCross;
			for (int t = 0; t < tournamentSize; t++)
			{
				int id = rand() % populationSize;
				idsToCross.push_back(id);
			}
			int idOfWorst = idsToCross.at(0);
			for (int i = 0; i < idsToCross.size(); i++)
			{
				if (graphsBob.at(idsToCross.at(i)).fitness < graphsBob.at(idOfWorst).fitness) {
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
			Graph offspring = crossAndReturnBestOfThree(graphsBob.at(indexOfFirst), graphsBob.at(indexOfSecond), true);
			graphsBob.at(idOfWorst) = offspring;
			double probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

			if (probability < mutationProbability)
			{
				Graph mutated = mutation(graphsBob.at(idOfWorst), true);
				graphsBob.at(idOfWorst) = mutated;
			}
		}
		
		cout << "Generation: " << generation + 1 << endl;
		bestBob = findBestGraph(graphsBob);
		cout << "Score of best individual BOB: " << bestBob.fitness << endl;
		cout << "Genes of the best individual BOB: ";
		print(bestBob);
		cout << endl;

		cout << "------------------------------------" << endl;

		if (bestBob.fitness > 0.99) {
			break;
		}
		

	}
	fillInitialPopulationCGP(graphsEve, false);
	for (int generation = 0; generation < generations; generation++)
	{
		for (int n = 0; n < crossoversInTournament; n++) //zadani br evaluacija
		{
			vector<int> idsToCross;
			for (int t = 0; t < tournamentSize; t++)
			{
				int id = rand() % populationSize;
				idsToCross.push_back(id);
			}
			int idOfWorst = idsToCross.at(0);
			for (int i = 0; i < idsToCross.size(); i++)
			{
				if (graphsEve.at(idsToCross.at(i)).fitness < graphsEve.at(idOfWorst).fitness) {
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
			Graph offspring = crossAndReturnBestOfThree(graphsEve.at(indexOfFirst), graphsEve.at(indexOfSecond), false);
			graphsEve.at(idOfWorst) = offspring;
			double probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

			if (probability < mutationProbability)
			{
				Graph mutated = mutation(graphsEve.at(idOfWorst), false);
				graphsEve.at(idOfWorst) = mutated;
			}
		}

		cout << "Generation: " << generation + 1 << endl;
		bestEve = findBestGraph(graphsEve);
		cout << "Score of best individual EVE: " << bestEve.fitness << endl;
		cout << "Genes of the best individual EVE: ";
		print(bestEve);
		cout << endl;

		cout << "------------------------------------" << endl;

		if (bestEve.fitness > 0.99) {
			break;
		}


	}
	/*
	for(int generation=0; generation < generations; generation++)
	{
		for (int t = 0; t < tournamentSize; t++)
		{
			double probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
			if (probability < crossoverProbability) {
				int indexOfFirst = rand() % populationSize;
				int indexOfSecond;

				do {
					indexOfSecond = rand() % populationSize;
				} while (indexOfSecond == indexOfFirst);
				Graph offspring = crossAndReturnBestOfThree(graphsBob.at(indexOfFirst), graphsBob.at(indexOfSecond));

				double fitnessOfBetterParent = graphsBob.at(indexOfSecond).fitness;
				if(graphsBob.at(indexOfSecond).fitness > fitnessOfBetterParent){
					fitnessOfBetterParent = graphsBob.at(indexOfSecond).fitness;
				} 
				if(offspring.fitness > fitnessOfBetterParent)
				{
					if (graphsBob.at(indexOfFirst).fitness < graphsBob.at(indexOfSecond).fitness)
					{
						graphsBob.at(indexOfFirst) = offspring;
					}
					else
					{
						graphsBob.at(indexOfSecond) = offspring;
					}
				}
			}
			probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
			if(probability < mutationProbability)
			{
				int indexToMutate = rand() % populationSize;
				Graph mutated = mutation(graphsBob.at(indexToMutate));
				graphsBob.at(indexToMutate) = mutated;
			}
		}
		cout << "Generation: " << generation + 1 << endl;
		Graph bestGraph = findBestGraph();
		cout << "Score of best individual: " << bestGraph.fitness << endl;
		cout << "Genes of the best individual: ";
		print(bestGraph);
		cout << endl;

		cout << "------------------------------------" << endl;

		if (bestGraph.fitness > 0.99) {
			return bestGraph;
		}
		
	}
	*/
	cout << "Score of best individual BOB: " << bestBob.fitness << endl;
	cout << "Score of best individual EVE: " << bestEve.fitness << endl;
	return bestBob;
}