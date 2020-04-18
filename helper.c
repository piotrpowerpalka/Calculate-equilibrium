#include "helper.h"

const char* Names[25] = {"A", "B", "C", "D", "E", "F", "G", "I", "J"};

/**
 * Calculates factorial
 * @param n argument
 * @return factorial value, n!
 */ 
long int factorial(unsigned int n)
{
    long int rv = 1;
	int i;
	for(i = 1; i <= n; i++) rv *= i;
	return rv;
}

/**
 * Searches the array of the given name of coalition
 * @param equArr array holding the data
 * @param name name searched in the array
 * @return the index in the array of the given name of the coalition
 */ 
int getNameIndex(equ** equArr, const char* name){
	int idx = -1;
	int i = 1;
	while(i < pow(2,10))	{
		printf("getNameiNDEX: |%s| |%s|\n", (*equArr)[i].name, name);
		if (strcmp((*equArr)[i].name, name) == 0) return i;
		i++;
	}
	return -1;
}

/**
 * Translates the binary value to the name of the coalition
 * for example value 3 (b0101) is translated into AC
 * @param aarg name of the coalition
 * @param s set writen binary
 * @param N 2^n, where n is number of agents  
 * 
 * @return name of the coaltion
 */ 
char* set2str(char* aarg, int s, int N){
	int j;
	aarg[0] = 0;
	for(j = 0; j < N; j++){
		if (0x1<<j & s)	aarg = strcat(aarg, Names[j]);
	}
	return aarg;
}				



/**
 * Translates the binary value to the name of the coalition
 * for example value 3 (b0101) is translated into 0_2_
 * @param aarg name of the coalition
 * @param s set writen binary
 * @param N 2^n, where n is number of agents  
 *
 * @return name of coalition
 */ 
char* printSubset(char * temp, int k, int N){
	int i;
	temp[0] = 0;
	
	for(i = 0; i < N; i++){
		if (k & 0x1<<i) 
			sprintf(temp, "%s_%d", temp, i);
	}
	
	return temp;
}
/**
 * Checks whether a value is in the Set
 * @param Set set
 * @param val value
 * 
 * @return 1 when the value is found in the set
 */
int inSubset(int Set, int val){
	if ( (0x1<<val & Set) > 0) return 1;
	return 0;
}

/**
 * Returns the number of agents in binary set
 * @param Set set
 * @param N 2^n wher n is a number of agents
 * 
 * @return the number of agents in binary set
 */
int valSubset(int Set, int N){
	int i, rv = 0;
	for(i = 0; i < N; i++){
		if ( (0x1<<i & Set) > 0) rv++;
	}
	return rv;
}
/**
 * Adds the 1 on i-th position to the set
 * @param Set set
 * @param i position
 * 
 * @return the value of binary set
 */
int addToSubset(int Set, int i){
		return Set | 0x1<<i;
}

/**
 * Return the array representation of the binary set
 * @param tmp the array of int passed to the function
 * @param k the subset in binary format
 * @param N 2^n where n is the number of agents
 * 
 * @return the array representation of the binary set
 */ 
int* getSubset(int* tmp, int k, int N){
	int i,j = 0;
	tmp[0] = 0;
	
	for(i = 0; i < N; i++){
		if (k & 0x1<<i) {
			tmp[j++] = i+1;
		}
	}
	tmp[j] = 0;
	return tmp;
}

/**
 * Initialize the array with 0 values.
 * @param equArr initialized array 
 */ 
void nullval(equ** equArr){
	int i = 0;
	for (i = 0; i < pow(2,10); i++){
		(*equArr)[i].value = 0;
	}
}
/**
 * Prepares the array to use
 * @param N  2^n where n is the number of agents
 * @param equArr prepared array
 * 
 * @return prepared array
 */ 
equ* prepareEqu(int N, void (*equVal)(equ** equArr)){
	int  i;
	int subset[25];
	char temp[1000];
	
	//prepare equs
	equ * equArr = (equ*)calloc((int)pow(2,N), sizeof(equ));
	if (equArr == NULL) {fprintf(stderr, "Error alocating the memory\n"); exit(2);}
	
	for (i = 1; i < pow(2,N); i++){	
		int x = 0, j = 0;
	
		printSubset(temp, i, N); 
		getSubset(subset, i, N);
		
		equArr[i].name = (char*)malloc((strlen(temp)+1)*sizeof(char));
		if (equArr[i].name == NULL) {fprintf(stderr, "Error alocating the memory\n"); exit(2);}
		
		strcpy(equArr[i].name, temp);
		
		equArr[i].value = 0;
		while(subset[j] != 0){
			equArr[i].subset[j] = subset[j];
			j++;
		}
		equArr[i].distance = 9999;
		equArr[i].eps = 0.0;
		equArr[i].fixed = 0;
	
	}	
	equVal(&equArr);
	
	return equArr;
}

/**
 * Calculates the disrupt value
 * @param N  2^n where n is the number of agents
 * @param equArr array with data
 * @param xVal 
 * @param fout output file where the data is written
 * 
 * @return calculated value of disrupt
 */ 
float cntDistrupt(int N, equ* equArr, int* xVal, FILE* fout){
	int j,i;
	int M[25];
	
	float tmp;
	float sum = 0.0;
	int fullSet = pow(2,N) - 1;
	float arr[25];
	for(j = 0; j < N; j++){
		M[j] = equArr[fullSet].value - equArr[fullSet ^ 0x1<<j].value;
	}
	for(j = 0; j < N; j++){
		arr[j] = 0;
		for(i = 0; i < N; i++)
			arr[j] += M[i]; 
		
		arr[j] -= equArr[fullSet].value;
		
		tmp = 0.0;
		for(i = 0; i < N; i++)
			tmp += equArr[0x1<<i].value;
		
		arr[j] /= equArr[fullSet].value - tmp;
		
		fprintf(stderr, "Optimal disrupt[%d] = %f\n", j, arr[j]);
	}		
	return arr[0];
}

/**
 * Calculates the Gately point
 * @param N  2^n where n is the number of agents
 * @param equArr array with data
 * @param xVal 
 * @param fout output file where the data is written
 */ 
void cntGatelyPoint(int N, equ* equArr, int* xVal, FILE* fout){
	int j,i;
	int M[25];
	
	float tmp;
	float sum = 0.0;
	int fullSet = pow(2,N) - 1;
	float arr[25];
	float d = cntDistrupt(N, equArr, xVal, fout);
	
	printf("Optimal d = %f\n", d);
	
	for(j = 0; j < N; j++){
		M[j] = equArr[fullSet].value - equArr[fullSet ^ 0x1<<j].value;
	}
	
	for(j = 0; j < N; j++){
		arr[j] = M[j] + d * equArr[0x1<<j].value;
		arr[j] /= d + 1.0;
	}
	
	for(j = 0; j < N; j++){
		printf("Gatley point[%d] = %f\n", j, arr[j]);
		fprintf(fout, "Gately;%s;%f\n", equArr[0x1<<j].name, arr[j]);
	}
	
	//chech the Gately distances
	for(j = 0; j < N; j++) {
		float d = 0.0;
		d = M[j] - arr[j];
		d /= arr[j] - equArr[0x1<<j].value;
		printf("Distrupt Value[%d] = %f\n", j, d);
		fprintf(fout, "Disrupt;%s;%f\n", equArr[0x1<<j].name, d);
	}
}
/**
 * Calculates the Shapley value
 * @param N  2^n where n is the number of agents
 * @param equArr array with data
 * @param xVal 
 * @param fout output file where the data is written
 */ 
void cntShapleyValue(int N, equ* equArr, int* xVal, FILE* fout){
	int j,i;
	float tmp, atmp;
	char * aarg = malloc(100);;
	
	for(j = 0; j < N; j++){
		tmp = 0.0;
		tmp = factorial(N-1) * equArr[0x1<<j].value;
		for (i = 1; i < pow(2,N); i++){	
			if(!inSubset(i, j)){
				int s = addToSubset(i,j);
				atmp  = factorial(valSubset(s, N) - 1);
				atmp *= factorial(N - valSubset(s, N));
				atmp *= (equArr[s].value - equArr[i].value);
				tmp += atmp;
			}	
		}
		tmp = tmp / factorial(N);
		printf("Shapley[%d] = %f\n", j, tmp);
		fprintf(fout, "Shapley;%s;%f\n", equArr[0x1<<j].name, tmp);
	}
	free(aarg);
}

/**
 * Calculates the Nucleoulus
 * @param N  2^n where n is the number of agents
 * @param equArr array with data
 * @param xVal 
 * @param fout output file where the data is written
 */ 	
void cntNukleous(int N, equ* equArr, int* xVal, FILE* fout){
	int end = 0;	
	while(!end){
		char cmd[100];
		// prepare the LP model
		nukleousPrepareModel("model.gms", N, equArr);
		// solve the model
		sprintf(cmd, "%s model.gms", PATH_TO_GAMS);
		system(cmd);
		// parse the result file
		end = nukleousAnalyzeModel("model.lst", N, equArr, xVal, fout);
	}
	
}
/**
 * Prepares the LP program with the GAMS model for solving the nucleoulus 
 * 
 * @param fname file where the model is put
 * @param N 2^n where n is the number of agents
 * @param equArr the array with data
 */ 
void nukleousPrepareModel(const char* fname, int N, equ* equArr){
	int i,j;
	char temp[1000];
	
	// open files
	FILE * fptr = fopen(fname, "w");
	if (fptr == NULL) {fprintf(stderr, "Cannot create file %s !!!!", fname); exit(1); }

	// write the model
	fprintf(fptr,"sets\n");

	fprintf(fptr,"n / ");
	for (i = 1; i < N; i++) fprintf(fptr,"'%d', ", i);
	fprintf(fptr,"'%d' /\n", N);
	
	fprintf(fptr,"s / ");
	for(i = 1; i < (pow(2,N)-1); i++) { 
		fprintf(fptr,"'%s', ", equArr[i].name); 
	}
	fprintf(fptr,"'%s' /\n;\n", equArr[(int)pow(2,N)-1].name);
	
	fprintf(fptr,"variables\ne\n;\n");
	fprintf(fptr,"positive variables\nx(n)\n;\n");
	fprintf(fptr,"parameters\nv(s)\n/");
	
	for(i = 1; i < pow(2,N); i++) { 
		fprintf(fptr,"   '%s'   %d\n", equArr[i].name, equArr[i].value); 
	}
	
	fprintf(fptr,"/;\n"); 
	
	fprintf(fptr,"equations\n");
	
	for (i = 1; i <= N; i++)
		fprintf(fptr,"EQ_x%d\n",i);
			
	for(i = 1; i < pow(2,N); i++) { 
		fprintf(fptr,"EQ_%s\n", equArr[i].name); 
	}
	fprintf(fptr,";\n", temp); 

	// write equations
	for(i = 1; i < (pow(2,N)-1); i++) { 
		fprintf(fptr,"EQ_%s..\n\t", equArr[i].name); 
		
		j = 0;
		while(equArr[i].subset[j] != 0){
			fprintf(fptr,"x('%d')",equArr[i].subset[j]); 
			if (equArr[i].subset[j + 1] != 0) fprintf(fptr,"+");
			j++;
		}
		if (equArr[i].fixed == 1){
			fprintf(fptr," =E= v('%s') - (%f);\n", equArr[i].name, equArr[i].eps);
			
		} else {
			fprintf(fptr," =G= v('%s') - e;\n", equArr[i].name);
		}
	}
	{
	fprintf(fptr,"EQ_%s..\n\t", equArr[(int)pow(2,N)-1].name); 
		
		j = 0;
		while(equArr[(int)pow(2,N)-1].subset[j] != 0){
			fprintf(fptr,"x('%d')",equArr[(int)pow(2,N)-1].subset[j]); 
			if (equArr[(int)pow(2,N)-1].subset[j + 1] != 0) fprintf(fptr,"+");
			j++;
		}
		fprintf(fptr," =E= v('%s');\n", equArr[(int)pow(2,N)-1].name);
	}
	
	for (i = 1; i <= N; i++)
		fprintf(fptr,"EQ_x%d..\n x('%d') =G= 0;\n", i, i);
			
	fprintf(fptr,"MODEL Nukleous  \"Nukleous\" /All/;\n");
	fprintf(fptr,"SOLVE Nukleous minimizing e using LP;\n");

	
	fprintf(fptr,"DISPLAY e.l;\n");
	
	for(i = 1; i < (pow(2,N)-1); i++) { 
		fprintf(fptr,"DISPLAY EQ_%s.slack;\n", equArr[i].name);
	}
	fprintf(fptr,"DISPLAY x.l;\n");
	fclose(fptr);
}

/**
 * Parses and analyzes the output file generated by GAMS application
 * 
 * @param rname file where the results are located
 * @param N 2^n where n is the number of agents
 * @param equArr the array with data
 * @param xVal
 * @param fout output file
 */ 
int nukleousAnalyzeModel(const char*  rname, int N, equ* equArr, int* xVal, FILE* fout)	{
	char *cptr = NULL;
	float obj = 9999999, x = 0.0;
	char lline[100];
	int numNonZeros = 0;
	int i,j;
	char temp[1000];
	
	FILE * rptr = fopen(rname, "r");
	while(fgets(temp, 1000, rptr) != NULL){
		if ((cptr = strstr(temp, "VARIABLE e.L")) != NULL) {
			if (sscanf(cptr, "VARIABLE e.L%*[ ]=%*[ ]%f", &x) == 1) {
				obj = x;
			} else { fprintf(stderr, "Objective value not found!!!"); exit(2); 	}
		}
		if ((cptr = strstr(temp, "EQUATION EQ_")) != NULL) {
			if (sscanf(cptr, "EQUATION EQ_%[_0123456789].Slack%*[ ]=%*[ ]%f", lline, &x) == 2) {
				
				printf("Equ %s - slack = %f\n", lline, x);
				
				for (i = 1; i < (int)pow(2,N); i++){
					if (strcmp(equArr[i].name, lline) == 0) {
						if (x == 0.0 && equArr[i].fixed == 0) {
								equArr[i].distance = x;
								equArr[i].eps = obj;
								equArr[i].fixed = 1;
								break;
						}
					}
				}
			} 
		}
		if ((cptr = strstr(temp, "VARIABLE x.L")) != NULL) {
			int idx;
			cptr += strlen("VARIABLE x.L");
			while (*cptr == ' ' || *cptr == '\n' || *cptr == '\t') cptr++;
			for (int i = 0; i < N; i++){
				if (sscanf(cptr, "%d%[ ]%f%[, ]", &idx, &x) == 2) {
					xVal[idx] = x;
				}
			}
		}
	}
	fclose(rptr);
	
	#ifdef DEBUGMODE
	for (i = 0; i < N; i++){
		fprintf(stdout, "x(%d)=%f\n", i, xVal[i]);
	}
	for (i = 1; i < (int)pow(2,N); i++){
		fprintf(stdout, "eps(%d)=%f\n", i, equArr[i].eps);
	}
	for (i = 1; i < (int)pow(2,N); i++){
		fprintf(stdout, "distance(%d)=%f\n", i, equArr[i].distance);
	}
	for (i = 1; i < (int)pow(2,N); i++){
		fprintf(stdout, "fixed(%d)=%d\n", i, equArr[i].fixed);
	}
	for (i = 1; i < (int)pow(2,N); i++){
		fprintf(stdout, "name(%d)=%s\n", i, equArr[i].name);
	}
	#endif
	
	// Analyse of results
	if (obj == 0.0) {
		fprintf(stdout, "Calculations finished, nucleuolus found\n");
		for (i = 0; i < N; i++){
			fprintf(stdout, "x(%d)=%f", i, xVal[i]);
			fprintf(fout, "Nukleous;%s;%f\n", equArr[0x1<<i].name, -equArr[0x1<<i].eps);
		}
		
		return 1;
	}
	
	#ifdef DEBUGMODE
	getchar();
	#endif
	
	for (i = 1; i < (int)pow(2,N); i++){
		if (equArr[i].eps == 0.0) numNonZeros++;
	}
	if (numNonZeros <= 1) {
		fprintf(stdout, "Calculations finished, nucleuolus found\n");
		for (i = 0; i < N; i++){
			fprintf(stdout, "x(%d)=%f", i, xVal[i]);
			fprintf(fout, "Nukleous;%s;%f\n", equArr[0x1<<i].name, equArr[0x1<<i].eps);

		}
		return 1;
	}
	return 0;
}
