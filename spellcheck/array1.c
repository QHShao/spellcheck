#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	FILE* f = fopen("wikipedia.txt", "r+");
	char* buffer = NULL;
	size_t size = 0;
	char* out1 = calloc(999999, 1);
	char* out2 = calloc(999999, 1);
	strcat(out1, "{");
	strcat(out2, "{");
	int count = 0;
	while(getline(&buffer, &size, f)!=-1){
		if(buffer[0]=='0' || strstr(buffer, "\'") || strstr(buffer, "\"")){
			free(buffer);
			buffer = NULL;
			size = 0;
			continue;
		}
		//char out[100];
		char* a[20];
		char* b[20];
		sscanf(buffer, "%s  %s", a, b);
		free(buffer);
		buffer = NULL;
		strcat(out1, " ,\"");
		strcat(out1, a);
		strcat(out1, "\"");
		strcat(out2, " ,\"");
		strcat(out2, b);
		strcat(out2, "\"");
		//sprintf(out, "%s  %s", a, b);
		//printf("%s\n", out);
		size = 0;
		count++;
	}
	strcat(out1, "}");
	strcat(out2, "}");
	out1[strlen(out1)] = '\n';
	out2[strlen(out2)] = '\n';
	printf("%s\n", out1);
	printf("%s\n", out2);
	printf("%d\n", count);
	free(out1);
	free(out2);
	return 0;
}