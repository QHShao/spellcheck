#include "ngram_dict.h"
#include "distance_dict.h"

#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
//	NgramDict dic("data/bigram.txt");

	while (1)
	{
		cout << "[ENTER] ";
		
		string line;
		getline(cin,line);

		vector<string> lines;
	
		int last = 0;

		for (int i = 0; i < line.length(); i++)
		{
			while (line[i] != '!' && line[i] != '.' && line[i] != '?' && line[i] != ',')
			{
				string cur = line.substring(last, i);
				last = i;
				cout << cur << endl;
			}
		}
/*
		istringstream cline(line);

		string token;
		vector <string> sentence;		

		while (getline(cline, token, ' '))
			sentence.push_back(token);

		dic.ProcessLine(sentence);
*/
	}

	return 0;
}
