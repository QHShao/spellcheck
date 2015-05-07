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

vector<pair<string, int>> * MetaphoneDict::GetList(string & query)
{
	auto result = dm::double_metaphone(query);

	string primary = result.first;
	string second = result.second;

	if (CheckExist(primary))
	{
		auto list1 = primary_meta_dictionary[primary];
		auto list2 = secondary_meta_dictionary[second];

		for (auto it = list1.begin(); it != list1.end(); it ++)
			cout << *it << " ";

		cout << endl;
		
		for (auto it = list2.begin(); it != list2.end(); it ++)
			cout << *it << " ";
	
		cout << endl;

		return new vector<pair<string, int>>();
	}

	return new vector<pair<string, int>>();
}

