#ifndef DISTANCE_DICT_H
#define DISTANCE_DICT_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class DistanceDict
{
    public:

	vector<pair<string, int>> * GetList(string & query);

    DistanceDict(const string& filename);
	
	void Format(string& str);

	int GetFrequency(const string & str);

	bool CheckExist(const string & str);

	private:

	void Update(const string& filename);

	void ProcessWord(int dis, const string& token, vector<pair< string, int >> * result);

	void ChangeFirst(const string& token, vector<pair< string, int >> * result);

	bool CheckHelp(const string& str);

	string SplitString(const string& str);

	void Erase(const string& str, vector<pair< string, int >> * result, int start);

	void EraseChar(const string& str, vector<pair< string, int >> * result);

	void Insert(const string& str, vector<pair< string, int >> * result, int start);

	void InsertChar(const string& str, vector<pair< string, int >> * result);

	void Replace(const string& str, vector<pair< string, int >> * result, int start);

	void ReplaceChar(const string& str, vector<pair< string, int >> * result);

	void ReverseChar(const string& str, vector<pair< string, int >> * result);

	int distance;

	map<string, int> helpermap;

    map<string, int> dictionary;

	const string alphabet = "abcdefghijklmnopqrstuvwxyz";
};


#endif
