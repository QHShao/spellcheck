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
	NgramDict ngram_dict; ////////////

	while (1)
	{
		cout << "[ENTER  ] ";
		
		string line;
		getline(cin,line);

		vector<string> lines(0);
		vector<char> punct;

		int last = 0;

		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] == '!' || line[i] == '.' || line[i] == '?' || line[i] == ',' || line[i] == ':' || line[i] == ';')
			{
				string tmp = line;
				string cur = tmp.substr(last, i - last);

				if (cur.compare("") != 0)
					lines.push_back(cur);

				last = i + 1;
				punct.push_back(line[i]);
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

		cout << "[SUGGEST] ";

		for (int i = 0; i < sentences.size(); i++)
		{
			ngram_dict.ProcessLine(sentences[i]);
			
			for (int j = 0; j < sentences[i].size(); j ++)
			{
				cout << sentences[i][j];
				if (j != sentences[i].size() - 1)
					cout << " ";
			}

			cout << punct[i] << " ";
		}

		cout << endl;
	}
	return 0;
}
