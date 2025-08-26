CFLAGS=-std=c11 -Wall -Wextra -O3 -g -fsanitize=address
CFLAGSVERBOSE=-std=c11 -Wall -Wextra -O0 -DVERBOSE=1
CFLAGSDEBUG=-std=c11 -Wall -Wextra -O0 -DDEBUG=3 -g -fsanitize=address
CFLAGSCOMPLEXITY=-std=c11 -Wall -Wextra -O3 -DCOMPLEXITY=2
INCLUDEFLAGS= -I ../include

BASICSRC=binary_tree.c linearcode.c list.c matrix_reductions.c matrix.c permutation.c PGE.c rawmatrix.c sparse_vector.c statistics.c SVR.c vector.c
BASICOBJ=binary_tree.o linearcode.o list.o matrix_reductions.o matrix.o permutation.o PGE.c rawmatrix.o sparse_vector.o statistics.o SVR.c vector.o

qcldpcgen:
	gcc $(CFLAGS) $(INCLUDEFLAGS) -o qcldpc qc_ldpc_generator.c $(BASICSRC) -ldl -lm && rm -f *.o


SVR:
	gcc $(CFLAGS) $(INCLUDEFLAGS) -o svr SVR_program.c $(BASICSRC) -ldl -lm && rm -f *.o
	
PGE:
	gcc $(CFLAGS) $(INCLUDEFLAGS) -o pge PGE_program.c $(BASICSRC) -ldl -lm && rm -f *.o
	
genWords:
	gcc $(CFLAGS) $(INCLUDEFLAGS) -o wordgen code_words_gen.c $(BASICSRC) -ldl -lm && rm -f *.o

main:
	gcc $(CFLAGS) $(INCLUDEFLAGS) -o main main.c $(BASICSRC) -ldl -lm && rm -f *.o

compareLinCode:
	gcc $(CFLAGS) $(INCLUDEFLAGS) -o compareLinCodes compare_linear_codes.c $(BASICSRC) -ldl -lm && rm -f *.o

all:
	make qcldpcgen
	make SVR
	make PGE
	make genWords
	make main
	make compareLinCode


verbose_project:
	gcc $(CFLAGSVERBOSE) $(INCLUDEFLAGS) -o main_verbose *.c -ldl -lm && rm -f *.o

project_complexity_measure:
	gcc $(CFLAGSCOMPLEXITY) $(INCLUDEFLAGS) -o main_complexity *.c -ldl -lm && rm -f *.o

project_debug:
	gcc $(CFLAGSDEBUG) $(INCLUDEFLAGS) -o main_debug *.c -ldl -lm && rm -f *.o


all_versions:
	make project
	make verbose_project
	make project_complexity_measure
	make project_debug
