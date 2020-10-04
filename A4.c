#include <stdio.h>
#include <stdlib.h>

int main() {
	unsigned int logicalAddress[100];
	int lenLogical = 0;
	int pageTable[1000];
	int i;
	int frame, page, offset;

	FILE *fptr = fopen("LogicalAddress.txt", "r");
	if (!fptr) {
		printf("can't open the file \"LogicalAddress.txt\"\n");
		return 0;
	}

	while (!feof(fptr)) {
		if (fscanf(fptr, "%d", &logicalAddress[lenLogical]) == 1)
			++lenLogical;
	}
	fclose(fptr);

	fptr = fopen("PageTable.txt", "r");
	if (!fptr) {
		printf("can't open the file \"PageTable.txt\"\n");
		return 0;
	}

	for (i = 0; i < 1000; ++i)
		pageTable[i] = -1;

	while (!feof(fptr)) {
		if (fscanf(fptr, "%d,%d", &page, &frame) == 2) {
			pageTable[page] = frame;
		}
	}
	fclose(fptr);

	fptr = fopen("PhysicalAddress.txt", "w");
	if (!fptr) {
		printf("can't create the file \"PhysicalAddress.txt\"\n");
		return 0;
	}

	for (i = 0; i < lenLogical; ++i) {
		page = (logicalAddress[i] & 12) / 4;
		offset = (logicalAddress[i] & 3);
		fprintf(fptr, "%d\n", pageTable[page] * 4 + offset);
	}

	fclose(fptr);
}
