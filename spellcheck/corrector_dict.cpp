#include "corrector_dict.h"

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

bool VectorSort(pair<string, int> pair1 ,pair<string, int> pair2) 
{
	return (pair1.second > pair2.second);
}

CorrectorDict::CorrectorDict(const string& filename)
{
    ifstream words(filename);
	string word;

	if(words.is_open())
	{
	    while(getline(words, word))
			dictionary[word] = 0;
	}
}

bool CorrectorDict::CheckHelp(const string& str)
{
	return (helpermap.find(str) != helpermap.end());
}

bool CorrectorDict::CheckExist(const string& str)
{
	return (dictionary.find(str) != dictionary.end());
}

void CorrectorDict::Update(const string& filename)
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

			if (CheckExist(key))
				dictionary[key] += val;
		}
	}

	return;
}

void CorrectorDict::Format(string& str)
{
	int len = str.length();

	for (int i = 0; i < len; i++)
	{
		if (str[i] <= 'Z' && str[i] >= 'A' )
			str.replace(i, 1, 1, str[i] - ('Z'-'z'));
	}
}


string CorrectorDict::SplitString(const string& str)
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

void CorrectorDict::ProcessWord(int dis, const string& token, vector<pair< string, int >> * result)
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

void CorrectorDict::ChangeFirst(const string& token, vector<pair< string, int >> * result)
{
	result->clear();
	helpermap.clear();

	std::string stmp = token;
	stmp.erase(stmp.begin());

	if (CheckExist(stmp) && !CheckHelp(stmp))
	{
		pair<string, int> val = make_pair(stmp, dictionary[stmp]);
		result->push_back(val);
		helpermap[stmp] = 1;
	}

	for (int j = 0; j < 26; j++)
	{
		std::string tmp = token;
		tmp.replace(0, 1, 1, alphabet[j]);

		if (token.compare(tmp) != 0 && CheckExist(tmp) && !CheckHelp(tmp))
		{
			pair<string, int> val = make_pair(tmp, dictionary[tmp]);
			result->push_back(val);
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
		pair<string, int> val = make_pair(out, dictionary[out]);
		result->push_back(val);
		helpermap[out] = 1;
	}

	sort(result->begin(), result->end(), VectorSort);

	return;
}

void CorrectorDict::Erase(const string& str, vector<pair< string, int >> * result, int start)
{
	int len = str.length();

	for (int j = start; j < len; j++)
	{
		std::string stmp = str;
		stmp.erase(stmp.begin() + j);

		if (CheckExist(stmp) && !CheckHelp(stmp))
		{
			pair<string, int> val = make_pair(stmp, dictionary[stmp]);
			result->push_back(val);
			helpermap[stmp] = 1;
		}
	}

	return;
}

void CorrectorDict::EraseChar(const string& str, vector<pair< string, int >> * result)
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
/*		
		if (CheckExist(tmp) && !CheckHelp(tmp))
		{
			
		}
*/
		pair<string, int> val = make_pair(tmp, dictionary[tmp]);
		result->push_back(val);
		helpermap[tmp] = 1;
	}

	return;
}

void CorrectorDict::Insert(const string& str, vector<pair< string, int >> * result, int start)
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
				pair<string, int> val = make_pair(tmp, dictionary[tmp]);
				result->push_back(val);
				helpermap[tmp] = 1;
			}
		}
	}
}

void CorrectorDict::InsertChar(const string& str, vector<pair< string, int >> * result)
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
				pair<string, int> val = make_pair(tmp, dictionary[tmp]);
				result->push_back(val);
				helpermap[tmp] = 1;
			}
		}
	}

	return;
}


void CorrectorDict::Replace(const string& str, vector<pair< string, int >> * result, int start)
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
				pair<string, int> val = make_pair(tmp, dictionary[tmp]);
				result->push_back(val);
				helpermap[tmp] = 1;
			}
		}
	}

	return;
}

void CorrectorDict::ReplaceChar(const string& str, vector<pair< string, int >> * result)
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
				pair<string, int> val = make_pair(tmp, dictionary[tmp]);
				result->push_back(val);
				helpermap[tmp] = 1;
			}
		}
	}

	return;
}

void CorrectorDict::ReverseChar(const string& str, vector<pair< string, int >> * result)
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
			pair<string, int> val = make_pair(out, dictionary[out]);
			result->push_back(val);
			helpermap[out] = 1;
		}
	}

	return;
}

void CorrectorDict::PrintDict()
{
	map<std:: string, int>::iterator it = dictionary.begin();

	while (it != dictionary.end())
	{
		cout << it->first << ":" << it->second << '\n';
		it ++;
	}
}
