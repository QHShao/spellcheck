#ifndef METAPHONE_DICT_H
#define METAPHONE_DICT_H

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class MetaphoneDict
{
	public:

	MetaphoneDict();

	vector<pair<string, int>> * GetList(string & query);

	private:

	void LoadPrimary();
	
	void LoadSecondary();

	bool CheckExist(string & query);

	unordered_map<string, vector<string>> primary_meta_dictionary;

	unordered_map<string, vector<string>> secondary_meta_dictionary;


};

#endif
