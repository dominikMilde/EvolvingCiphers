#include "evolution.hpp"
#include <iostream>
#include <bitset>
#include <random>

#include "configs.hpp"

CGP cgp(numInputs, numOutputs, numRows, numColumns, numNodeInputs);

vector<Graph> graphs;
minstd_rand randomEngineCGP;

vector<unsigned char> inputs;
vector<unsigned char> outputs;
unsigned char key;

double fitnessIndividual(vector <int> graph)
{
	cgp.graph = graph;
	vector <unsigned char> runnedOutputs;
	for (auto a : outputs)
	{
		//cout << a << endl;
		vector<unsigned char> forCgpIn = { a, key };

		runnedOutputs.push_back(cgp.propagate(forCgpIn));
	}
	//cout << runnedOutputs[0] << endl;
	return fitnessFunction(inputs, runnedOutputs);
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

Graph findBestGraph()
{

	int idxBest = 0;
	for (int i = 1; i < populationSize; i++) {
		if (graphs.at(i).fitness > graphs.at(idxBest).fitness) {
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

Graph crossAndReturnBestOfThree(Graph firstParent, Graph secondParent)
{
	Graph child(crossover(firstParent.graph, secondParent.graph), NULL);
	child.fitness = fitnessIndividual(child.graph);

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

Graph mutation(Graph graphStruct)
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
	
	
	
	double fitness = fitnessIndividual(graph);
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


void fillInitialPopulationCGP()
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
		double fitness = fitnessIndividual(rG);
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


Graph runCGP(vector<unsigned char> inputsF, vector<unsigned char> outputsF, unsigned char keyF)
{

	inputs = inputsF;
	outputs = outputsF;
	key = keyF;
	
	fillInitialPopulationCGP();
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
				Graph offspring = crossAndReturnBestOfThree(graphs.at(indexOfFirst), graphs.at(indexOfSecond));

				double fitnessOfBetterParent = graphs.at(indexOfSecond).fitness;
				if(graphs.at(indexOfSecond).fitness > fitnessOfBetterParent){
					fitnessOfBetterParent = graphs.at(indexOfSecond).fitness;
				} 
				if(offspring.fitness > fitnessOfBetterParent)
				{
					if (graphs.at(indexOfFirst).fitness < graphs.at(indexOfSecond).fitness)
					{
						graphs.at(indexOfFirst) = offspring;
					}
					else
					{
						graphs.at(indexOfSecond) = offspring;
					}
				}
			}
			probability = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
			if(probability < mutationProbability)
			{
				int indexToMutate = rand() % populationSize;
				Graph mutated = mutation(graphs.at(indexToMutate));
				graphs.at(indexToMutate) = mutated;
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
	return findBestGraph();
}