.PHONY: default q2 run

default: build_os
	cd apps/example; make

q2: build_os
	cd apps/q2; make

build_os:
	cd os; make

clean:
	cd os; make clean
	cd apps/example; make clean
	cd apps/q2; make clean

run:
	cd ../lab2/bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 3; ee469_fixterminal