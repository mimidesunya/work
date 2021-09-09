#include <stdio.h>
#include <string.h>

void linear_search() {
	FILE* fp = fopen("2007.csv", "r");
	char line[1000];

	while(fgets(line, sizeof line, fp)) {
		if (strstr(line, "0857-53-1777")) {
			printf("%s", line);
			break;
		}
	}
	fclose(fp);
}

void main() {
	linear_search();
}
