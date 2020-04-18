#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//#define DEBUGMODE
#define PATH_TO_GAMS "C:\\GAMS\\win64\\23.8\\gams.exe"


typedef struct _equ{
	char* name;
	int subset[25];
	float distance;
	float eps;
	int value;
	int fixed;
} equ;

int getNameIndex(equ** equArr, const char* name);
char* set2str(char* aarg, int s, int N);
void nullval(equ** equArr);
equ* prepareEqu(int N, void (*equVal)(equ** equArr));
float cntDistrupt(int N, equ* equArr, int* xVal, FILE* fout);
void cntGatelyPoint(int N, equ* equArr, int* xVal, FILE* fout);
void cntShapleyValue(int N, equ* equArr, int* xVal, FILE* fout);
void cntNukleous(int N, equ* equArr, int* xVal, FILE* fout);
void nukleousPrepareModel(const char* fname, int N, equ* equArr);
int nukleousAnalyzeModel(const char*  rname, int N, equ* equArr, int* xVal, FILE* fout);
long int factorial(unsigned int n);
char* printSubset(char * temp, int k, int N);
int inSubset(int Set, int val);
int valSubset(int Set, int N);
int addToSubset(int Set, int i);
int* getSubset(int* tmp, int k, int N);
