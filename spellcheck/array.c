#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	FILE* f = fopen("ABODAT.643", "r+");
	char* buffer = NULL;
	size_t size = 0;
	char* out1 = calloc(99999, 1);
	char* out2 = calloc(99999, 1);
	strcat(out1, "{");
	strcat(out2, "{");
	while(getline(&buffer, &size, f)!=-1){
		char* a[20];
		char* b[20];
		sscanf(buffer, "%s %s", a, b);
		buffer = NULL;
		strcat(out1, " ,\"");
		strcat(out1, a);
		strcat(out1, "\"");
		strcat(out2, " ,\"");
		strcat(out2, b);
		strcat(out2, "\"");
		size = 0;
	}
	strcat(out1, "}");
	strcat(out2, "}");
	out1[strlen(out1)] = '\n';
	out2[strlen(out2)] = '\n';
	printf(out1);
	printf(out2);
	return 0;
}