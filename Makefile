.PHONY: default q2 q3 q4 run

default: build_os
	cd apps/example; make

q2: build_os
	cd apps/q2; make

q3: build_os
	cd apps/q3; make

q4: build_os
	cd apps/q4; make

build_os:
	cd os; make

clean:
	cd os; make clean
	cd apps/example; make clean
	cd apps/q2; make clean
	cd apps/q3; make clean
	cd apps/q4; make clean

run:
	cd ../lab2/bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 3; ee469_fixterminal