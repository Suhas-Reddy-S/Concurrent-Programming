#!/bin/bash

### Build
make

### Generate a test file
### (of unspecified range and size)
# shuf -i0-2147483643 -n382 > case1.txt

### Sort it using sort
# sort -n case1.txt > soln1.txt

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NOCOLOR='\033[0m'



EXEC="./mysort"
chmod u+x $EXEC

TESTDIR=""
if [ $# -eq 1 ] 
then
    TESTDIR=$1
else
    TESTDIR="./autograde_tests"
fi

SCORE=0
for threads in 2 4 8; do
	echo "Threads: ${threads}"
	for alg in merge quick; do
		echo "Algorithm: ${alg} Sort"
		for file in $TESTDIR/*; do
			if [ "${file: -4}" == ".txt" ]
			then
				IN=$file
				CASE=${IN%.*}
				MY=$CASE.my
				ANS=$CASE.ans
				$EXEC -i $IN -o $MY --alg=$alg -t $threads
				if cmp --silent $MY $ANS;
				then 
					SCORE=$(($SCORE+5))
					echo -e $CASE $alg "..... ${GREEN}Pass${NOCOLOR}"
				else 
					echo -e $CASE $alg "..... ${RED}FAIL${NOCOLOR}"
				fi
			fi
		done
		echo "Test for ${alg} sort completed using ${threads} threads"
	done
done
SCORE=$(echo "scale=1; $SCORE/2" | bc -l)

echo -e "${YELLOW}Score:" $SCORE "${NOCOLOR}"

RET=1
if [ $SCORE == "35.0" ]
	then
	RET=0
fi

make clean

exit $RET
