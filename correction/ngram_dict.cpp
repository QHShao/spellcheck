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

NgramDict::NgramDict()
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
		bigram_dictionary[key].push_back(val_pair);
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
	return (bigram_dictionary.find(key) != bigram_dictionary.end());
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

bool NgramDict::NonWord(vector<string> & sentence)
{
	vector<string> tmp = sentence;
	FormatLine(tmp);
	bool modified = false;

	for (int i = 0; i < sentence.size(); i++)
	{
		if (frequency_dict.GetFrequency(tmp[i]) < 100)
		{
			if (i == 0 || CheckExist(tmp[i - 1]) == false)
			{
				auto flist = frequency_dict.GetList(tmp[i]);
				string suggest = flist->begin()->first;

				if ( sentence[i][0] <= 'Z' && sentence[i][0] >= 'A' )
					suggest.replace(0, 1, 1, suggest[0] - ('Z'-'z'));

				sentence[i] = suggest;
			}
			else
			{
				auto list = GetNext(sentence[i - 1]);
				bool flg = true;

				for (auto it = list.begin(); it != list.end(); it ++)
				{
					if (Levenshtein(it->first, tmp[i]) < 2)
					{
						string suggest = it->first;						
						if ( sentence[i][0] <= 'Z' && sentence[i][0] >= 'A' )
							suggest.replace(0, 1, 1, suggest[0] + ('Z'-'z'));	

						sentence[i] = suggest;

						flg = false;
						break;
					}
				}

				if (flg)
				{
					auto flist = frequency_dict.GetList(tmp[i]);						
					string suggest = flist->begin()->first;

					if ( sentence[i][0] <= 'Z' && sentence[i][0] >= 'A' )
						suggest.replace(0, 1, 1, suggest[0] - ('Z'-'z'));

					sentence[i] = suggest;
				}
			}

			modified = true;
		}
	}

	return modified;
}

void NgramDict::RealWord(vector<string> & sentence)
{
	vector<string> tmp = sentence;
	FormatLine(tmp);
	bool modified = false;


	for (int i = 0; i < tmp.size() - 1; i++)
	{
		bool find = false;

		if (CheckExist(tmp[i]))
		{
			auto list = GetNext(tmp[i]);
			for (auto it = list.begin(); it != list.end(); it ++)
			{
				if (tmp[i + 1].compare(it->first) == 0)
					find = true;
			}
		}
		else
			find = true;
		
		if (!find)
		{
			cout << "[SUSPECT] "<< sentence[i+1] << endl;

			auto list = GetNext(sentence[i]);

			bool flg = true;

			for (auto it = list.begin(); it != list.end(); it ++)
			{
				if (Levenshtein(it->first, tmp[i + 1]) < 2)
				{
					sentence[i + 1] = it->first;
					flg = false;
					modified = true;
					break;
				}
			}
/*
			if (flg)
			{
				auto flist = frequency_dict.GetList(tmp[i + 1]);

				sentence[i + 1] = flist->begin()->first;
				modified = true;
			}
*/	
			i ++;
		}
	}

	if (modified)
		cout << "[SUGGEST] ";
}

void NgramDict::ProcessLine(vector<string> & sentence)
{
	bool modified = NonWord(sentence);

	if (modified == false)
	{
		cout << "(no non-word error)" << endl;
		RealWord(sentence);
	}
}

vector<pair<string, int>> & NgramDict::GetNext(const string & key)
{
	return (bigram_dictionary.find(key))->second;
}
