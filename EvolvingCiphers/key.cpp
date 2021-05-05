#include "key.hpp"
Key::Key(vector<bool> key, double fitness) : key(key), fitness(fitness){}

vector<unsigned char> Key::toChar()
{
    vector <unsigned char> vectored;
    for (int i = 0; i < keyLength; i++)
    {
        unsigned char c = 0;
        short s = 128;
        for (int j = 0; j < 8; j++)
        {
            if (key[i * 8 + j])
            {
                c += s;
            }
            s /= 2;
        }
        vectored.push_back(c);
    }
    return vectored;
}
