/* Public domain */

#include <cstdio>

#include "base/string.h"

void memory_report()
{
	static const char* statm_path = "/proc/self/statm";

	static uint vmsize;
	static uint resident;
	static uint shared;
	static uint text;
	static uint lib;
	static uint data;
	static uint dirty;

	static FILE* f;

	f = fopen(statm_path,"r");
	if(f) {
		if(fscanf(f, "%u %u %u %u %u %u %u",
		          &vmsize, &resident, &shared, &text, &lib, &data, &dirty)
		   == 7) {
			printf("Memory: VM: %6d   RSS: %6d   SH: %6d   TXT: %6d   DATA: %6d\n\n",
			                vmsize, resident, shared, text, data);
		} else {
			printf("Error reading %s\n", statm_path);
		}
		fclose(f);
	} else {
		printf("Error opening %s\n", statm_path);
	}
}

