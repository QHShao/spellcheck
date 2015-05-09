#include "distance_dict.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

int DistanceDict::GetDistance(const string & str1, const string& str2)
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


vector<pair<string, int>> * DistanceDict::GetList(string & query)
{
	Format(query);

	auto result = new vector<pair< string, int >>();

	ProcessWord(1, query, result);

	auto it = result->begin();

	if(it == result->end() || it->second < 10)
	{
		ChangeFirst(query, result);
		it = result->begin();
	}

	if(it == result->end() || it->second < 10)
	{
		ProcessWord(2, query, result);			
		it = result->begin();
	}

	return result;
}

bool VectorSort(pair<string, int> pair1 ,pair<string, int> pair2) 
{
	return (pair1.second > pair2.second);
}

bool DistanceDict::CheckHelp(const string& str)
{
	return (helpermap.find(str) != helpermap.end());
}

int DistanceDict::GetFrequency(const string & str)
{
	string tmp = str;
	
	Format(tmp);

	if (CheckExist(tmp))
		return dictionary[tmp];
	
	return -1;
}

bool DistanceDict::CheckExist(const string& str)
{
	return (dictionary.find(str) != dictionary.end());
}

DistanceDict::DistanceDict()
{
    ifstream words("data/words.txt");
	string word;

	if(words.is_open())
	{
	    while(getline(words, word))
			dictionary[word] = 0;
	}

	Update("data/frequency.txt");
}

void DistanceDict::Update(const string& filename)
{
    ifstream words(filename);
	string line;

	if(words.is_open())
	{
	    while(getline(words, line))
		{
			int len = line.length();
			size_t index = line.find(" ", 0);

			string value = line.substr(0, index);
			int val = stoi(value);
			string key = line.substr(index + 1, len);
			Format(key);
			
			if (CheckExist(key))
				dictionary[key] += val;
		}
	}
	
	return;
}

void DistanceDict::Format(string& str)
{
	int len = str.length();

	for (int i = 0; i < len; i++)
	{
		if (str[i] <= 'Z' && str[i] >= 'A' )
			str.replace(i, 1, 1, str[i] - ('Z'-'z'));
	}
}


string DistanceDict::SplitString(const string& str)
{
	int len = str.length();

	for (int i = 1; i < len - 1; i++)
	{
		std::string tmp = str;

		string part1 = tmp.substr(0, i);
		string part2 = tmp.substr(i, len - 1);

		if (CheckExist(part1) && CheckExist(part2))
			return part1 + " " + part2;
	}

	return NULL;
}

void DistanceDict::ProcessWord(int dis, const string& token, vector<pair< string, int >> * result)
{
	distance = dis;
	result->clear();
	helpermap.clear();

	EraseChar(token, result);
	InsertChar(token,result);
	ReplaceChar(token, result);

	if (dis == 1)
		ReverseChar(token, result);

	sort(result->begin(), result->end(), VectorSort);

	return;
}

void DistanceDict::ChangeFirst(const string& token, vector<pair< string, int >> * result)
{
	result->clear();
	helpermap.clear();

	std::string stmp = token;
	stmp.erase(stmp.begin());

	if (CheckExist(stmp) && !CheckHelp(stmp))
	{
		result->push_back(make_pair(stmp, dictionary[stmp]));
		helpermap[stmp] = 1;
	}

	for (int j = 0; j < 26; j++)
	{
		std::string tmp = token;
		tmp.replace(0, 1, 1, alphabet[j]);

		if (token.compare(tmp) != 0 && CheckExist(tmp) && !CheckHelp(tmp))
		{
			result->push_back(make_pair(tmp, dictionary[tmp]));
			helpermap[tmp] = 1;
		}
	}

	std::string out = token;
	char tmp1 = out.at(0);
	char tmp2 = out.at(1);
	out.replace(0, 1, 1, tmp2);
	out.replace(1, 1, 1, tmp1);

	if (token.compare(out) != 0 && CheckExist(out) && !CheckHelp(out))
	{
		result->push_back(make_pair(out, dictionary[out]));
		helpermap[out] = 1;
	}

	sort(result->begin(), result->end(), VectorSort);

	return;
}

void DistanceDict::Erase(const string& str, vector<pair< string, int >> * result, int start)
{
	int len = str.length();

	for (int j = start; j < len; j++)
	{
		std::string stmp = str;
		stmp.erase(stmp.begin() + j);

		if (CheckExist(stmp) && !CheckHelp(stmp))
		{
			result->push_back(make_pair(stmp, dictionary[stmp]));
			helpermap[stmp] = 1;
		}
	}

	return;
}

void DistanceDict::EraseChar(const string& str, vector<pair< string, int >> * result)
{
	int len = str.length();

	for (int i = 1; i < len; i++)
	{
		std::string tmp = str;
		tmp.erase(tmp.begin() + i);

		if(distance == 2)
		{
			Insert(tmp, result, i);	
			Erase(tmp, result, i);
			Replace(tmp, result, i);
		}
		
		if (CheckExist(tmp) && !CheckHelp(tmp))
		{
			result->push_back(make_pair(tmp, dictionary[tmp]));
			helpermap[tmp] = 1;
		}
	}

	return;
}

void DistanceDict::Insert(const string& str, vector<pair< string, int >> * result, int start)
{
	int len = str.length();

	for (int i = start; i < len; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			std::string tmp = str;
			tmp.insert(tmp.begin() + i, alphabet[j]);
	
			if (str.compare(tmp) != 0 && CheckExist(tmp) && !CheckHelp(tmp))
			{
				result->push_back(make_pair(tmp, dictionary[tmp]));
				helpermap[tmp] = 1;
			}
		}
	}
}

void DistanceDict::InsertChar(const string& str, vector<pair< string, int >> * result)
{
	int len = str.length();

	for (int i = 1; i < len + 1; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			std::string tmp = str;
			tmp.insert(tmp.begin() + i, alphabet[j]);

			if(distance == 2)
			{
				Insert(tmp, result, i);	
				Erase(tmp, result, i);
				Replace(tmp, result, i);
			}
			if (CheckExist(tmp) && !CheckHelp(tmp))
			{
				result->push_back(make_pair(tmp, dictionary[tmp]));
				helpermap[tmp] = 1;
			}
		}
	}

	return;
}


void DistanceDict::Replace(const string& str, vector<pair< string, int >> * result, int start)
{
	int len = str.length();

	for (int i = start; i < len; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			std::string tmp = str;
			tmp.replace(i, 1, 1, alphabet[j]);

			if (str.compare(tmp) != 0 && CheckExist(tmp) && !CheckHelp(tmp))
			{
				result->push_back(make_pair(tmp, dictionary[tmp]));
				helpermap[tmp] = 1;
			}
		}
	}

	return;
}

void DistanceDict::ReplaceChar(const string& str, vector<pair< string, int >> * result)
{
	int len = str.length();

	for (int i = 1; i < len; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			std::string tmp = str;
			tmp.replace(i, 1, 1, alphabet[j]);

			if(distance==2)
			{
				Insert(tmp, result, i);	
				Erase(tmp, result, i);
				Replace(tmp, result, i);
			}
			if (str.compare(tmp) != 0 && CheckExist(tmp) && !CheckHelp(tmp))
			{
				result->push_back(make_pair(tmp, dictionary[tmp]));
				helpermap[tmp] = 1;
			}
		}
	}

	return;
}

void DistanceDict::ReverseChar(const string& str, vector<pair< string, int >> * result)
{
	int len = str.length();

	for(int i = 0; i< len - 1 ; i++)
	{
		string out = str;
		char tmp1 = out.at(i);
		char tmp2 = out.at(i+1);
		out.replace(i, 1, 1, tmp2);
		out.replace(i + 1, 1, 1, tmp1);

		if (str.compare(out) != 0 && CheckExist(out) && !CheckHelp(out))
		{
			result->push_back(make_pair(out, dictionary[out]));
			helpermap[out] = 1;
		}
	}

	return;
}
