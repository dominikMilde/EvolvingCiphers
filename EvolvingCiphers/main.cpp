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

	for (int i = 0; i < sizeLearningSet; i++)
	{
		vector <unsigned char> t = { 70, 69, 82, 85, 110, 105, 122, 103, 72, 114, 118, 97, 116, 115, 107, 97 };
		plaintexts.push_back(t);
	}

	k.key = randomKey();
	vector <unsigned char>key = k.toChar();

	cgpAlice.graph = { 3,1,0,4,1,0,1,1,0,1,4,2,0,0,5,7,4,6,4,7,3,5,3,8,4,2,9,1,8,2,1,5,8,5,9,11,2,2,10,7,13,12,6,4,6,7,9,7,6,11,14,0,6,17,3,13,10,7,2,13,21 };

	cgpBob.graph = { 6,0,1,6,1,0,7,0,0,2,1,2,1,5,1,7,0,5,2,2,5,0,7,2,0,3,9,5,3,7,5,0,6,5,1,8,6,8,12,6,13,11,0,7,2,1,3,15,0,3,9,6,13,3,4,14,16,4,9,10,19 };
	
	vector<unsigned char> kz = { 0b00010111, 0b10001000 , 0b01000000 , 0b11100111 , 0b01100001 , 0b11101011 , 0b00000011 , 0b01101011 , 0b00010001 , 0b00101101 , 0b10001110 , 0b10011101 , 0b10000111 , 0b00000001 , 0b10100101 , 0b01110010};
	vector<unsigned char> kljucEva = { 0b01110111 , 0b01110111 , 0b01011111 , 0b00000111 , 0b01100010 , 0b11101011 , 0b11110000 , 0b10010010 , 0b11100000 , 0b01011011 , 0b01110001 , 0b01100100 , 0b10000000 , 0b11000001 , 0b10111011 , 0b10001101 };

	primjeniRunde(plaintexts, cgpAlice, cgpBob, kz, kljucEva, 1);
}
