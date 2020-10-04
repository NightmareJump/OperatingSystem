#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 20
#define MAX_WORKS 50
#define IN_FILE_NAME "TaskSpec.txt"
#define OUT_FILE_NAME "Output.txt"

#define FCFS 1
#define RR 2
#define NSJF 3
#define PSJF 4
int i;
typedef struct work {
	char name[MAX_NAME_LENGTH];
	//original burst value from data file
	int burstOrigin;
	//arrival time
	int arrival;

	//runtime burst value, may be changed during scheduling
	int burst;
} Work;

//works array
Work works[MAX_WORKS];
//number of works
int size = 0;

//split tokens by ','
void split(char* str) {
	int length = strlen(str);
	for (i = 0; i < length; i++) {
		if (str[i] == ',') {
			str[i] = ' ';
		}
	}
}

//parse the work from data line
Work parseLine(char* str) {
	split(str);

	Work work;
	sscanf(str, "%s%d%d", work.name, &work.arrival, &work.burstOrigin);
	work.burst = work.burstOrigin;

	return work;
}

//read TaskSpec.txt
void readTaskSpec() {

	size = 0;
	char line[MAX_LINE_LENGTH];
	FILE* file = fopen(IN_FILE_NAME, "r");
	if (file) {

		while (fscanf(file, "%s", line) == 1) {
			Work work = parseLine(line);
			works[size] = work;
			size++;
		}

		fclose(file);
	} else {
		fprintf(stderr, "not found %s.\n", IN_FILE_NAME);
	}
}

//recover the status of all works
void reset() {
	for (i = 0; i < size; i++) {
		works[i].burst = works[i].burstOrigin;
	}
}

//check if all works finished.
int allFinished() {
	for (i = 0; i < size; i++) {
		if (works[i].burst > 0) {
			return 0;
		}
	}
	return 1;
}

//get next work to be executed.
void getNextWorkFCFS(int time, int* nextWork, int* runTime) {
	for (i = 0; i < size; i++) {
		if (works[i].burst > 0 && works[i].arrival <= time) {
			*nextWork = i;
			*runTime = works[i].burst;
			break;
		}
	}
}
int c;
//get next work to be executed.
void getNextWorkRR(int time, int lastWork, int* nextWork, int* runTime) {
	int i = lastWork;
	i = (i + 1) % size;

	for (c = 0; c < size; c++) {
		if (works[i].burst > 0 && works[i].arrival <= time) {
			if (works[i].burst >= 4) {
				*runTime = 4;
			} else {
				*runTime = works[i].burst;
			}
			*nextWork = i;
			break;
		}

		i = (i + 1) % size;
	}
}

//get next work to be executed.
void getNextWorkNSJF(int time, int* nextWork, int* runTime) {
	*nextWork = -1;

	for (i = 0; i < size; i++) {
		if (works[i].burst > 0 && works[i].arrival <= time) {
			if(*nextWork == -1){
				*nextWork = i;
				*runTime = works[i].burst;
			}else if(works[i].burst < *runTime){
				*nextWork = i;
				*runTime = works[i].burst;
			}
		}
	}
}

//get next work to be executed.
void getNextWorkPSJF(int time, int* nextWork, int* runTime) {
	*nextWork = -1;

	for (i = 0; i < size; i++) {
		if (works[i].burst > 0 && works[i].arrival <= time) {
			if(*nextWork == -1){
				*nextWork = i;
				*runTime = works[i].burst;
			}else if(works[i].burst < *runTime){
				*nextWork = i;
				*runTime = works[i].burst;
			}
		}
	}

	*runTime = 1;
}

void schedule(int type, FILE* out) {
	//reset arrival and burst of each work
	reset();

	double waiting = 0;
	int time = 0;
	int lastWork = -1;

	while (!allFinished()) {
		int nextWork;
		int runTime;

		switch (type) {
		case FCFS:
			getNextWorkFCFS(time, &nextWork, &runTime);
			break;
		case RR:
			getNextWorkRR(time, lastWork, &nextWork, &runTime);
			break;
		case NSJF:
			getNextWorkNSJF(time, &nextWork, &runTime);
			break;
		case PSJF:
			getNextWorkPSJF(time, &nextWork, &runTime);
			break;
		}

		if (nextWork != lastWork) {
			if (lastWork != -1) {
				fprintf(out, "\t%d\n", time);
			}
			fprintf(out, "%s\t%d", works[nextWork].name, time);
		}

		works[nextWork].burst -= runTime;
		time += runTime;

		if (works[nextWork].burst == 0) {
			waiting += time - works[nextWork].arrival
					- works[nextWork].burstOrigin;
		}

		lastWork = nextWork;
	}

	fprintf(out, "\t%d\n", time);
	fprintf(out, "Average Waiting Time: %g\n\n", waiting / size);
}

int main() {
	FILE* out = fopen(OUT_FILE_NAME, "w");

	//read TaskSpec.txt
	readTaskSpec();

	fprintf(out, "FCFS:\n");
	schedule(FCFS, out);
	fprintf(out, "RR:\n");
	schedule(RR, out);
	fprintf(out, "NSJF:\n");
	schedule(NSJF, out);
	fprintf(out, "PSJF:\n");
	schedule(PSJF, out);

	fclose(out);
	return 0;
}

