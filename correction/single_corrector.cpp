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

	DistanceDict distance_dict;
	MetaphoneDict mataphone_dict;

	while(1)
	{
		cout << "[ENTER]: ";
		getline(cin,query);
	
		if (query.compare(quit) == 0)
			break;

		istringstream cline(query);
		distance_dict.Format(query);

		auto list = distance_dict.GetList(query);
		auto it = list->begin();

		if (it != list->end() && distance_dict.GetDistance(query, it->first) > 1)
		{
			if (mataphone_dict.VerifySame(query, it->first) == 0)
			{
				auto best = list->begin();
				int max = 0;

				for (it = list->begin() + 1; it != list->end(); it++)
				{
					int score = mataphone_dict.VerifySame(query, it->first);

					if (score > max)
					{
						max = score;
						best = it;
					}
				}

				auto tmp = * best;
				* best = list->front();
				list->front() = tmp;
			}

		}			

		cout << "[RESULT]:" <<it->first << endl;
	
	}

	return 0;
}
