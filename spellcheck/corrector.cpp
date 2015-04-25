#include "corrector_dict.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
	string line;
	string quit ("Q");

	CorrectorDict dict("data/words.txt");
	dict.Update("data/frequency.txt");

	int correct = 0;

	while(1)
	{
		cout << "ENTER TEXT: ";
		getline(cin,line);
	
		if (line.compare(quit) == 0)
			break;

		istringstream cline(line);
		string token;
		string processed;

		while(getline(cline, token, ' '))
		{
			dict.Format(token);

			vector<pair<string, int>> * result = new vector<pair< string, int >>();

			dict.ProcessWord(1, token, result);

			vector<pair <string, int>>::iterator it = result->begin();

			if(it->second < 10)
				dict.ChangeFirst(token, result);

			it = result->begin();

			if(it->second < 10)
				dict.ProcessWord(2, token, result);
			
			it = result->begin();

			while (it != result->end())
			{
				cout << it->first << " " << it->second << '\n';
				it++;
			}
		}
	}

	return 0;
}
