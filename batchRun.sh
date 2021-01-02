#!/usr/bin/env bash

exeName=example

nap()
{
        exeProc=`ps -U $USER | grep ${exeName} | wc -l`

        while [ $exeProc -ge 6 ]; do
		sleep 5
		exeProc=`ps -U $USER | grep ${exeName} | wc -l`
        done
}

Ei=10

while [ ${Ei} -le 1000 ]; do
	sed -e "s;\ PARTICLEENERGY;\ ${Ei};"  template.in > run_example${Ei}.in
	#./${exeName} run_example${Ei}.in > run_example${Ei}.log 2>&1 &
	./${exeName} run_example${Ei}.in > run_example${Ei}.log
	mv output.root run_example${Ei}.root

	#nap
	let Ei+=10
done



