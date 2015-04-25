#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	char* name = "source.txt";
	FILE* f = fopen(name, "r+");
	char* buffer = NULL;
	size_t size = 0;
	getline(&buffer, &size, f);
	ssize_t a = getline(&buffer, &size, f);
	int count = 0;
	while(count<=938955){
		int temp1;
		char temp2[100];
		int temp4;
		char temp3[100];
		sscanf(buffer, "%d %s %s %d", &temp1, temp2, temp3, &temp4);
		printf("%d %s\n", temp1, temp2);
		free(buffer);
		buffer = NULL;
		size = 0;
		ssize_t a = getline(&buffer, &size, f);
		count++;
	}
	free(buffer);
	return 0;
}