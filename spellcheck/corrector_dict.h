#ifndef CORRECTOR_DICT_H
#define CORRECTOR_DICT_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class CorrectorDict
{
    public:

    CorrectorDict(const std::string& filename);

	void Update(const string& filename);

	void PrintDict();

	void Format(string& str);

	void ProcessWord(int dis, const string& token, vector<pair< string, int >> * result);

	void ChangeFirst(const string& token, vector<pair< string, int >> * result);

    std:: map<std:: string, int> dictionary;

	bool CheckExist(const string& str);

	private:

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

	std:: map<std:: string, int> helpermap;

	const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
};


#endif
