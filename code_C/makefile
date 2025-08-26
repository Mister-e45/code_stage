
all:	
	cd src && make all
	cp src/main .
	rm src/main
	cp src/qcldpc .
	rm src/qcldpc
	cp src/svr .
	rm src/svr
	cp src/pge .
	rm src/pge
	cp src/wordgen .
	rm src/wordgen
	cp src/compareLinCodes .
	rm src/compareLinCodes

all_verbose:
	cd src && make verbose_project
	cp src/main .
	rm src/main

all_complexity:
	cd src && make project_complexity_measure
	cp src/main_complexity .
	rm src/main_complexity

all_debug:
	cd src && make project_debug
	cp src/main_debug .
	rm src/main_debug	

all_versions:
	cd src && make all_versions
	cp src/main .
	rm src/main
	cp src/main_verbose .
	rm src/main_verbose
	cp src/main_complexity .
	rm src/main_complexity
	cp src/main_debug .
	rm src/main_debug