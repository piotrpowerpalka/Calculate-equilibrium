/**
 * @author: Piotr Pałka
 * @licence: GNU General Public License (GPL)
 * @description:
 * Program calculates the Shapley, nucleolus, and Gately values for given values of the characteristic functions.
 * Program reads data from a file given as the runtime argument, processes it, and outputs the results on stdout.
 * Format of the input is:
_0_1_2;100
_0_1;60
_1_2;30
_0_2;35
_0;30
_1;25
_2;10
 * where _X is the number of the agent in coaltion, 
 * and value after semicolon is the valus of characteristic function.
 * A number of agents is up to 10.
 * 
 * Program outputs the results to the file, the name of the output file is: res_<input_file_name>_.txt
 * The format of the output file is:
Value;A;30
Value;B;25
Value;AB;60
Value;C;10
Value;AC;35
Value;BC;30
Value;ABC;100
Gately;_0;42.727272
Gately;_1;37.727272
Gately;_2;19.545454
Disrupt;_0;2.142857
Disrupt;_1;2.142857
Disrupt;_2;2.142857
Shapley;_0;43.333332
Shapley;_1;38.333332
Shapley;_2;18.333334
Nukleous;_0;-41.667000
Nukleous;_1;-36.667000
Nukleous;_2;-21.667000
 *
 * The program needs, for calculation of Nucleoulus, GAMS application installed (https://www.gams.com/)
 * If you don't have the application, comment line no 143
 * Path to the GAMS instalation is in the helper.h file in macro PATH_TO_GAMS
 */
#include "helper.h"
extern char* Names[10];

/**
 * Outputs help
 */
void help(){
		printf("usage: program calculates the Shapley, nucleolus, and Gately values for given values of the characteristic functions.\n");
		printf("[-h] [--help] [-f FILE] [--file FILE]\n");
		printf("arguments:\n");
		printf("-h, --help show this help message and exit\n");
		printf("-f FILE, --file FILE path to file with data\n");
}

/**
 * main program 
 */
int main(int argc, char *argv[]){
	unsigned int N = 4, i, j, ii;
	int xVal[25];
	char *pol, *coliczyc;
	FILE* fp;
	FILE* fout = stdout;
	equ * equArr;
	
	if (argc >= 2) 
		pol = argv[1];
	if (argc >= 3)
		coliczyc = argv[2];
	if (argc == 1){
		help();
		exit(0);
	}	
	
	if (strcmp(pol, "--file") == 0 || strcmp(pol, "-f") == 0) {
		int x;
		char name[10];
		char foutname[100];
		N = 3;
		int r = 0;
		equArr = prepareEqu(10, nullval);
		
		if (argc <= 2) {
			fprintf(stderr, "File name not specified !!!\n");
			exit(1);
		}
		fp = fopen(argv[2], "r");
		if (fp == 0) {
			fprintf(stderr, "File %s does not exist!!!!\n", argv[2]);
			exit(2);
		}
		puts("reading...\n");
		while (feof(fp) == 0){
			r = fscanf(fp, "%[^;];%d\n", name, &x); // scans the input file for data
			#ifdef DEBUG_MODE
			printf("%d, reading [%s] = %d\n", r, name, x);
			#endif 
			
			int idx = getNameIndex(&equArr, name); // translates the '_X_Y_Z' format to numeric format
			
			#ifdef DEBUG_MODE
			printf("idx = %d, name = |%s|\n", idx, name);
			#endif 
			
			if (idx >= 0) {
				equArr[idx].value = x;			// writes the equArr the data read
				#ifdef DEBUG_MODE
				printf("value[%s] = %d\n", name, x);
				#endif 
			}
			if (r != 2) break;					// break when the row is broken
		}
		
		sprintf(foutname, "res_%s_.txt", argv[2]); // prepare the output file name
		fout = fopen(foutname, "w"); // open the output file
		
		fclose(fp);
	}
	else if (strcmp(pol, "--help") == 0 || strcmp(pol, "-h") == 0){
		help();
		exit(0);
	}
	
	for (i = 1; i < (int)pow(2,N); i++){
		char * aarg = malloc(100);;
		aarg[0] = 0;
		for(j = 0; j < N; j++){
			if (0x1<<j & i)	aarg = strcat(aarg, Names[j]);
		}
		fprintf(stdout, "value(%s)=%d\n", aarg, equArr[i].value); // output the prepared data into the stdout
		fprintf(fout, "Value;%s;%d\n", aarg, equArr[i].value); // output the prepared data into the output file
		
		free(aarg);
	}
	
	
	cntDistrupt(N, equArr, xVal, fout); 		// calculate the disrupt value
	cntGatelyPoint(N, equArr, xVal, fout); 		// calculate the Gately point
	cntShapleyValue(N, equArr, xVal, fout);		// calculate the Shapley values
	cntNukleous(N, equArr, xVal, fout);			// calculate the nucleoulus point
	fclose(fout);								// closes the output file
	
	return 0;
}
