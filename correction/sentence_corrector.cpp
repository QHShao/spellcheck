#include "ngram_dict.h"
#include "distance_dict.h"

#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
	NgramDict dic("data/bigram.txt");

	while (1)
	{
		cout << "[ENTER] ";
		
		string line;
		getline(cin,line);

		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] == '!' || line[i] == '.' || line[i] == '?' || line[i] == ',')
			{
				line.erase(i,1);
				i --;
			}
		}

		istringstream cline(line);

		string token;
		vector <string> sentence;		

		while (getline(cline, token, ' '))
			sentence.push_back(token);

		dic.ProcessLine(sentence);
	}

	return 0;
}
