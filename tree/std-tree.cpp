#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>

bool cmp(const char* a, const char* b) { return strcmp(a, b) < 0; };

int main(int argc, const char *argv[]) {
	const char *file = argv[1];
	size_t limit = 30000000L;
	
	char *data = (char*)malloc(100L * 30000000L);
	if (!data) {
		return 0;
	}
	
	std::set<const char*, decltype(cmp)*> st(cmp);
	
	char line[700];
	FILE *fp = fopen(file, "r");
	char *pdata = data;
	size_t i = 0;
	while (fgets(line, sizeof line, fp)) {
		char *comma = strchr(line, ',');
		char *comma2 = strchr(comma + 1, ',');
		*comma = *comma2 = 0;
		size_t length = comma2 - line + 1;
		memcpy(pdata, line, length);
		
		const char* number = pdata;
		const char* name = pdata + (comma - line) + 1;
		
		st.insert(name);
		
		i++;
		if (i % 10000 == 0) {
			printf("build tree %ld\n", i);
		}
		if (i > limit) {
			break;
		}
		pdata += length;
	}
	fclose(fp);
	
	printf("search\n");
	printf("%ld\n", st.count("花うさぎ"));
	return 0;
}
