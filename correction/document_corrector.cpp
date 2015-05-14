#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>

#include "ngram_dict.h"

using namespace std;

void parse(const char * infile)
{
	ifstream in(infile);
 	ofstream out("tmp.txt");
  	char puncs[] = {'.', ',', ':', '?', '!'};
  	char tmp;
  	while(in.good()){
  		tmp = in.get();
  		bool find = false;
  		for(int i = 0; i < 5; i++){
  			if(puncs[i]==tmp)
  				find = true;
  		}
  		if(tmp=='\"'){
  			out << tmp;
  			out << '\n';
  			continue;
  		}
		if(tmp=='\n'){
			out << '\n';
		}
  		if(!((tmp >= 'a' && tmp <= 'z') || (tmp >= '0' && tmp <= '9') || tmp==' ' || find || (tmp >= 'A' && tmp <='Z'))){
  			continue;
  		}
  		if(find){
  			out << tmp;
  			out << '\n';
  			continue;
  		}
  		out << tmp ;
  }
}

void process()
{
	NgramDict ngram_dict;

	ifstream tmp("tmp.txt");
	ofstream out("result.txt");
	string line;

	while(getline(tmp, line))
	{
		if(line.compare("")==0)
		{
			out << endl;
			continue;
		}		
	
		vector<string> lines(0);
		vector<char> punct;

		int last = 0;

		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] == '!' || line[i] == '.' || line[i] == '?' || line[i] == ',' || line[i] == ':' || line[i] == ';' || line[i] == '\"' )
			{
				string tmp = line;
				string cur = tmp.substr(last, i - last);

				if (cur.compare("") != 0)
					lines.push_back(cur);

				last = i + 1;
				punct.push_back(line[i]);
			}
		}

		int size = lines.size();
		vector<vector<string>> sentences(size);

		for (int i = 0; i < lines.size(); i++)
		{
			istringstream cline(lines[i]);
			string token;

			while (getline(cline, token, ' '))
			{
				if (token.compare("") != 0)
				{
					string out = ngram_dict.Split(token);
					if (out != string())
					{
						size_t index = out.find(" ");
						string str1 = out.substr(0, index);
						string str2 = out.substr(index+1, out.length() - index);
						sentences[i].push_back(str1);
						sentences[i].push_back(str2);
					}
					else
						sentences[i].push_back(token);
				}
			}
		}

		for (int i = 0; i < sentences.size(); i++)
		{
			ngram_dict.ProcessLine(sentences[i]);
			
			for (int j = 0; j < sentences[i].size(); j ++)
			{
				out << sentences[i][j];
				if (j != sentences[i].size() - 1)
					out << " ";
			}

			out << punct[i] << " ";
		}

	}

}

int main(int argc, char ** argv)
{
	char * infile = argv[1];
	parse(infile);
	process();
}
