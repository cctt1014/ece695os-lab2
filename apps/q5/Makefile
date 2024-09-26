default:
	cd makeprocs; make
	cd producer_n; make
	cd producer_o; make
	cd reaction_1; make
	cd reaction_2; make
	cd reaction_3; make
	cd reaction_4; make

clean:
	cd makeprocs; make clean
	cd producer_n; make clean
	cd producer_o; make clean
	cd reaction_1; make clean
	cd reaction_2; make clean
	cd reaction_3; make clean
	cd reaction_4; make clean

run:
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 10 10 100; ee469_fixterminal
