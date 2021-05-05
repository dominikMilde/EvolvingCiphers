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
	
    vector<unsigned char> plaintext = { 0b11100111, 0b10011000, 0b10101010, 0b01110100};
    vector <unsigned char> key =      { 0b10000011, 0b11001100, 0b00110001, 0b01010011};
	
    vector<Graph> alicePopulation;
	alicePopulation.reserve(populationSize);
	//generating population of Alice
	for(int i = 0; i<populationSize; i++)
	{
		alicePopulation.push_back(Graph(randomGraph(), 0));
		
	}
	//rating fitness of created Alice
	for (Graph alice : alicePopulation)
	{
		cgpAlice.graph = alice.graph;
		//cout << "ARHITEKTURA ALICE:\n";
		//for (int j = 0; j < alice.graph.size(); j++)
		//{
		//	cout << alice.graph[j];
		//}
		//cout << "\n";
		vector<unsigned char> ciphertext = cgpAlice.generateCipher(plaintext, key);
		//cout << "CIPHER:\n";
		//for (auto a : ciphertext)
		//{
		//	cout << bitset<8>(a) << endl;
		//}
		Graph bob = evaluateBob(plaintext, key, ciphertext);
		cout << "najbolji bob:" << bob.fitness << endl;
		//double eva = evaluateEva(bob, plaintext, ciphertext);
	}
	
}