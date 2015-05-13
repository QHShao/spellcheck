#ifndef NGRAM_DICT_H
#define NGRAM_DICT_H

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "distance_dict.h"

using namespace std;

class NgramDict
{
	public:

	NgramDict();

	void ProcessLine(vector<string> & sentence);

	bool ShouldIgnore(const string & str);

	bool NonWord(vector<string> & sentence);

	void RealWord(vector<string> & sentence);

	void FormatLine(vector<string> & sentence);

	string Split(const string& str);
	
	private:

	bool CheckExist(const string& key);

	vector<pair<string, int>> & GetNext(const string& key);

	int Levenshtein(const string & str1, const string& str2);

	unordered_map<string, vector<pair<string, int>>> bigram_dictionary;

	DistanceDict frequency_dict;
};
#endif
