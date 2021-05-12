
#include <vector>

#include "cgp.hpp"
#include <iostream>
#include <bitset>
using namespace std;

Graph::Graph(vector<int> graph, double fitness) : graph(graph), fitness(fitness) {}
Graph::Graph() {}

//CGP cgp(2, 1, 1, 2, 2);
CGP::CGP(int n, int m, int r, int c, int a) :  n(n), m(m), r(r), c(c), a(a){}

unsigned char CGP::propagate(vector<unsigned char> inputs)
{
    vector<unsigned char> nodeOutputs = inputs;
    for (int i = 0; i < c; i++)
    {
        vector<unsigned char> currNodeOutputs = nodeOutputs;
        for (int j = 0; j < r; j++)
        {
            int functionIdPos = (i * r + j) * (a + 1);
            int functionId = graph.at(functionIdPos);
            vector<int> nodeInputIds;
            for (int k = functionIdPos + 1; k < functionIdPos + 1 + a; k++)
            {
                nodeInputIds.push_back(graph.at(k));
            }

            nodeOutputs.push_back(calculateFunction(currNodeOutputs, functionId, nodeInputIds));
        }
    }

    for (int i = 0; i < m; i++)
    {
        int pos = graph.at(graph.size() - m + i);
        unsigned char out = nodeOutputs.at(pos);

        bitset<8> x(out);
        //std::cout << "outputCGP: " << x << '\n';

        return out;
    }
}

// racuna zadanu operaciju nad danim argumentima
double CGP::calculateFunction(vector<unsigned char> calcOutputs, int functionId, vector<int> nodeInputIds)
{

    vector<unsigned char> calcInputs;
    for (int i = 0; i < nodeInputIds.size(); i++)
    {
        calcInputs.push_back(calcOutputs.at(nodeInputIds.at(i)));
    }

    unsigned char rez;
    switch (functionId)
    {
    case 0: //AND
        rez = calcInputs[0] & calcInputs[1];
        break;
    case 1: //OR
        rez = calcInputs[0] | calcInputs[1];
        break;
    case 2: //NOT
        rez = ~calcInputs[0];
        break;
    case 3: //XOR
        rez = calcInputs[0] ^ calcInputs[1];
        break;
    case 4: //ROR
    { 
        bool odd = calcInputs[0] % 2;
        rez = calcInputs[0] >> 1;
        if (odd)
        {
            rez = rez | 0b10000000;
        }
        break;
    }
        
    case 5: //ROL
    {
    	bool leftMostBit = (((calcInputs[0] & 0b10000000) == 0b10000000) ? true : false);
		rez = calcInputs[0] << 1;
	    if (leftMostBit)
	    {
	        rez += 1;
	    }
        break;
    }
    case 6: //XOR ponovno pokretanje vi�e puta
    {
        rez = calcInputs[0] ^ calcInputs[1];
        break;
    }
    case 7: //XOR ponovno pokretanje vi�e puta
    {
        rez = calcInputs[0] ^ calcInputs[1];
        break;
    }
    case 8: //XOR ponovno pokretanje vi�e puta
    {
        rez = calcInputs[0] ^ calcInputs[1];
        break;
    }
    }
    return rez;
}

vector<unsigned char> CGP::generateCipher(vector<unsigned char> plaintext, vector <unsigned char> key)
{
    vector <unsigned char> ciphertext;
	for(int i = 0; i<plaintext.size(); i++)
	{
        vector<unsigned char> input = { plaintext.at(i), key.at(i) };
        ciphertext.push_back(propagate(input));
	}
    return ciphertext;
}

