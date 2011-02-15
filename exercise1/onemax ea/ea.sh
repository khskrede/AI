#!/bin/bash
if ./main $1 $2 $3 $4 $5 $6 $7 $8 $9; then
	DATE=$(date +%Y%d%d%H%M%S)
	OUTFILE="data/${DATE}.png"
	echo "set title \"Plot of EA\"
	set xlabel \"Generation\"
	set ylabel \"Fitness\"
	set yrange [-10:55]
	set terminal png size 800, 600
	set output \"${OUTFILE}\"
	plot \"data/ea.data\" using 1:2 with lines ti \"Best\", \"data/ea.data\" using 1:3 with lines ti \"Average\", \"data/ea.data\" using 1:4 with lines ti \"Standard deviation\"" | gnuplot
	mv data/ea.data data/${DATE}.data
	mv data/ea.info data/${DATE}.txt
fi;
