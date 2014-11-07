Main: NBody_Par.cpp
	/usr/lib64/openmpi/bin/mpic++ NBody_Par.cpp -o NBody_Parallel.out
	qsub submitNBody_2_6.sge
	qsub submitNBody_8_12.sge
	qsub submitNBody_14_20.sge
	qsub submitNBody_Seq.sge
Seq:
	/usr/lib64/openmpi/bin/mpic++ NBody_Seq.cpp -o NBody_Sequential.out
	qsub submitNBody_Seq.sge
2_6:
	/usr/lib64/openmpi/bin/mpic++ NBody_Par.cpp -o NBody_Parallel.out
	qsub submitNBody_2_6.sge
8_12:
	/usr/lib64/openmpi/bin/mpic++ NBody_Par.cpp -o NBody_Parallel.out
	qsub submitNBody_8_12.sge
14_20:
	/usr/lib64/openmpi/bin/mpic++ NBody_Par.cpp -o NBody_Parallel.out
	qsub submitNBody_14_20.sge
clean:
	rm NBody_Parallel.out
	rm NBody_Sequential.out
