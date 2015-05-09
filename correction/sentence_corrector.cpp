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
			if (line[i] == '!' || line[i] == '.' || line[i] == '?' || line[i] == ',')
			{
				string tmp = line;
				string cur = tmp.substr(last, i - last);
				last = i + 1;
				lines.push_back(cur);
			}
		}

		int size = lines.size();
		vector<vector<string>> sentences(size);

		for (int i = 0; i < lines.size(); i++)
		{
			istringstream cline(lines[i]);
			string token;

			while (getline(cline, token, ' '))
			{
				if (token.compare("") != 0)
					sentences[i].push_back(token);
			}
		}


		for (int i = 0; i < sentences.size(); i++)
		{
			for (int j = 0; j < sentences[i].size(); j ++)
			{
				cout << sentences[i][j] << endl;
			}

			cout << "-------------------" << endl;			
		}
	}

	return 0;
}
