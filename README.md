# Calculate-equilibrium
The  program calculates the Shapley, nucleolus, and Gately values for given values of the characteristic functions.  The program reads data from a file given as the runtime argument, processes it, and outputs the results on stdout.

Format of the input is:

_0_1_2;100
_0_1;60
_1_2;30
_0_2;35
_0;30
_1;25
_2;10

where _X is the number of the agent in coaltion, 
and value after semicolon is the valus of characteristic function.
A number of agents is up to 10.

Program outputs the results to the file, the name of the output file is: res_<input_file_name>_.txt
The format of the output file is:

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

The program needs, for calculation of Nucleoulus, GAMS application installed (https://www.gams.com/)
If you don't have the application, comment line no 143
Path to the GAMS instalation is in the helper.h file in macro PATH_TO_GAMS

To compile the program type:
make
