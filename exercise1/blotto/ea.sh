#!/bin/bash
if ./main $@; then
	DATE=$1"-"$3"-"$4
	OUTFILE="data/${DATE}.png"
	echo "set title \"Plot of EA\"
	set xlabel \"Generation\"
	set ylabel \"Fitness\"
	set terminal png size 800, 600
	set output \"${OUTFILE}\"
	plot \"data/ea.data\" using 1:2 with lines ti \"Best\", \"data/ea.data\" using 1:3 with lines ti \"Average\", \"data/ea.data\" using 1:4 with lines ti \"Standard deviation\",  \"data/ea.data\" using 1:5 with lines ti \"Average entropy\"" | gnuplot
	mv data/ea.data data/${DATE}.data
	mv data/ea.info data/${DATE}.txt
fi;
