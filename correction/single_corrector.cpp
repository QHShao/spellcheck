#include "distance_dict.h"
#include "metaphone_dict.h"

#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
	string query;
	string quit ("Q");

//	DistanceDict distance_dict("data/words.txt");

	MetaphoneDict mataphone_dict;

	while(1)
	{
		cout << "[ENTER]: ";
		getline(cin,query);
	
		if (query.compare(quit) == 0)
			break;

		istringstream cline(query);
		
		auto list = mataphone_dict.GetList(query);

	/*
		for (auto it = list->begin(); it != list->end(); it++)
			cout << it->first << endl;
	*/
	}

	return 0;
}
