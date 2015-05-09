#include "metaphone_dict.h"

using namespace std;

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <istream>
#include <cstring>
#include <utility> 

#include "double_metaphone.h"

void MetaphoneDict::LoadPrimary()
{

	ifstream file("data/phonetic_primary_dict.txt");
	string line;

    while(getline(file, line))
	{
		const char * cline = line.c_str();
 		char key[50];
		char val[50];
		sscanf(cline,"%s %s", val, key);

		primary_meta_dictionary[key].push_back(val);
	}

}

void MetaphoneDict::LoadSecondary()
{
	ifstream file("data/phonetic_secondary_dict.txt");
	string line;

    while(getline(file, line))
	{
		const char * cline = line.c_str();
 		char key[50];
		char val[50];
		sscanf(cline,"%s %s", val, key);

		secondary_meta_dictionary[key].push_back(val);
	}
}

MetaphoneDict::MetaphoneDict()
{
	LoadPrimary();
	LoadSecondary();
}

bool MetaphoneDict::CheckExist(string & query)
{
	return (primary_meta_dictionary.find(query) != primary_meta_dictionary.end());
}

int MetaphoneDict::VerifySame(const string & str1, const string & str2)
{
	auto result1 = dm::double_metaphone(str1);
	auto result2 = dm::double_metaphone(str2);
	
	int score = 0;

	if (result1.first.compare(result2.first) == 0)
		score += 2;

	if (result1.second.compare(result2.second) == 0)
		score += 1;

	return score;
}

vector<string> * MetaphoneDict::GetList(string & query)
{
	auto result = dm::double_metaphone(query);

	string primary = result.first;
	string second = result.second;

	if (CheckExist(primary))
	{
		auto list1 = primary_meta_dictionary[primary];
		auto list2 = secondary_meta_dictionary[second];

		sort(list1.begin(), list1.end());
		sort(list2.begin(), list2.end());

		vector<string> * common = new vector<string>;

		set_intersection(list1.begin(),list1.end(),list2.begin(),list2.end(),back_inserter(*common));

		return common;
	}

	return new vector<string>();
}

