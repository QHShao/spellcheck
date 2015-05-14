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

string NgramDict::Split(const string& str)
{
	string sub = str;

	bool cap = false;
	if (sub[0] >= 'A' && sub[0] <= 'Z')
		cap = true;

	frequency_dict.Format(sub);
	
	if(frequency_dict.GetFrequency(sub) > 10)
	{
		return string();
	}

	int len = str.length();

	size_t best = 0;
	string curBest = string();
	string curPart1;
	string curPart2;

	for (int i = 1; i < len - 1; i++)
	{
		std::string tmp = sub;

		string part1 = tmp.substr(0, i);
		string part2 = tmp.substr(i, len - 1);

		if (frequency_dict.CheckExist(part1) && frequency_dict.CheckExist(part2))
		{
			size_t score = frequency_dict.GetFrequency(part1) * frequency_dict.GetFrequency(part2);
			if(score > best)
			{
				curBest = part1 + " " + part2;
				curPart1 = part1;
				curPart2 = part2;
				best = score;
			}
		}
	}

	bool bigramExist = false;

	if(!CheckExist(curPart1))
	{
		return string();
	}
	auto list = GetNext(curPart1);

	for (auto it = list.begin(); it != list.end(); it++)
	{
		if(curPart2.compare(it->first) ==0 )
		{
			bigramExist = true;
		}
	}

	if(bigramExist)
	{
		if (cap)
			curBest[0] += ('Z'-'z');

		return curBest;
	}
	return string(); 
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

bool NgramDict::ShouldIgnore(const string & str)
{
	if (string::npos != str.find_first_of("0123456789"))
		return true;

	return false;
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
		if ( i != 0 && sentence[i][0] <= 'Z' && sentence[i][0] >= 'A' )
			continue;
		else if (ShouldIgnore(sentence[i]))
			continue;
		else if (frequency_dict.GetFrequency(tmp[i]) < 10)
		{

			if (i == 0 || CheckExist(tmp[i - 1]) == false)
			{
				auto flist = frequency_dict.GetList(tmp[i]);

				if (flist->size() == 0)
					continue;

				string suggest = flist->begin()->first;

				suggest.insert(0, "[");
				suggest.insert(suggest.length(), "]");
				sentence[i] = suggest;
			}
			else
			{
				auto list = GetNext(tmp[i - 1]);
				bool flg = true;

				for (auto it = list.begin(); it != list.end(); it ++)
				{
					if (Levenshtein(it->first, tmp[i]) < 2)
					{
						string suggest = it->first;

						suggest.insert(0, "[");
						suggest.insert(suggest.length(), "]");
						sentence[i] = suggest;

						flg = false;
						break;
					}
				}

				if (flg)
				{
					auto flist = frequency_dict.GetList(tmp[i]);

					if (flist->size() == 0)
						continue;
						
					string suggest = flist->begin()->first;

					suggest.insert(0, "[");
					suggest.insert(suggest.length(), "]");
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
		if ( i != 0 && sentence[i][0] <= 'Z' && sentence[i][0] >= 'A' )
			continue;

		if (ShouldIgnore(sentence[i]))
			continue;

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
			auto list = GetNext(tmp[i]);

			bool flg = true;

			for (auto it = list.begin(); it != list.end(); it ++)
			{
				if (Levenshtein(it->first, tmp[i + 1]) < 2)
				{
					string suggest = it->first;
					suggest.insert(0, "[");
					suggest.insert(suggest.length(), "]");
	
					sentence[i + 1] = suggest;
					flg = false;
					modified = true;
					break;
				}
			}

			if (flg)
			{
				sentence[i].insert(0, "|");
				sentence[i].insert(sentence[i].length(), "|");
			}

			i ++;
		}
	}

}

void NgramDict::ProcessLine(vector<string> & sentence)
{
	bool modified = NonWord(sentence);
	
	if (modified == false)
		RealWord(sentence);
}

vector<pair<string, int>> & NgramDict::GetNext(const string & key)
{
	auto& it = bigram_dictionary.find(key)->second;
	sort(it.begin(), it.end(), NgramSort);

	return it;
}
