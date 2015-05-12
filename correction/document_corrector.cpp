#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
using namespace std;

int main(){
	ifstream in("essay.txt");
 	ofstream out("out.txt");
  	char puncs[] = {'.', ',', ':', '?', '!'};
  	char tmp;
  	while(in.good()){
  		tmp = in.get();
  		bool find = false;
  		for(int i = 0; i < 5; i++){
  			if(puncs[i]==tmp)
  				find = true;
  		}
  		if(tmp=='\"' || tmp=='\''){
  			out << tmp;
  			out << '\n';
  			continue;
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