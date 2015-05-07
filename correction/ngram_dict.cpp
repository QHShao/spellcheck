#include "ngram_dict.h"
#include "distance_dict.h"

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <istream>
#include <cstring>
#include <utility> 

using namespace std;

bool NgramSort(pair<string, int> pair1 ,pair<string, int> pair2) 
{
	return (pair1.second > pair2.second);
}

NgramDict::NgramDict(const string& filename)
{
    ifstream file("data/bigram.txt");
	string line;

    while(getline(file, line))
	{
		const char * cline = line.c_str();
		int frequency;
 		char key[50];
		char val[50];
		sscanf(cline,"%d	%s	%s", &frequency, key, val);
		pair <string, int> val_pair = make_pair(val, frequency);
		ngram_dictionary[key].push_back(val_pair);
	}

}

int NgramDict::Levenshtein(const string & str1, const string& str2)
{
	size_t len1 = str1.size();
	size_t len2 = str2.size();

	vector<vector<int>> d(len1 + 1, vector<int>(len2 + 1));
	d[0][0] = 0;

	for(int i = 1; i <= len1; i ++)
		d[i][0] = i;

	for(int i = 1; i <= len2; i ++)
		d[0][i] = i;

	for(int i = 1; i <= len1; i ++)
		for(int j = 1; j <= len2; j ++)
    		d[i][j] = min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + (str1[i - 1] == str2[j - 1] ? 0 : 1) });

	return d[len1][len2];
}

bool NgramDict::CheckExist(const string & key)
{
	return (ngram_dictionary.find(key) != ngram_dictionary.end());
}

void NgramDict::FormatLine(vector<string> & sentence)
{
	for (int i = 0; i < sentence.size(); i++)
	{
		string str = sentence[i];
		int len = str.length();

		for (int j = 0; j < len; j++)
		{
			if (str[j] <= 'Z' && str[j] >= 'A' )
				str.replace(j, 1, 1, str[j] - ('Z'-'z'));
		}
		sentence[i] = str;
	}
}

vector<int> * NgramDict::ScaneLine(vector<string> & sentence)
{
	auto result = new vector<int>();


	for (int i = 0; i < sentence.size(); i++)
	{
		if (frequency_dict.GetFrequency(sentence[i]) < 10)
			result->push_back(i);
	}

	for (int i = 0; i < sentence.size() - 1; i++)
	{
		if (CheckExist(sentence[i]))
		{
			auto list = GetNext(sentence[i]);
			bool flg = true;

			for (auto it = list.begin(); it < list.end(); it++)
			{
				if (sentence[i + 1].compare(it->first) == 0)
				{
					flg = false;								
				}			
			}

			if (flg)
			{
				if (find(result->begin(), result->end(), i+1) == result->end())
					result->push_back(i + 1);
				i++;
			}
		}
	}

	return result;
}

void NgramDict::ProcessLine(vector<string> sentence)
{
	FormatLine(sentence);

	auto result = ScaneLine(sentence);

	for (auto it = result->begin(); it != result->end(); it++)
		cout << sentence[*it] << " ";

	cout << endl;
}

vector<pair<string, int>> & NgramDict::GetNext(const string& key)
{
	return (ngram_dictionary.find(key))->second;
}
