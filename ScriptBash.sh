echo Cuantas CPUs?
read nCPUs
mkdir -p results
while read -r sched
do
	echo
	echo "Simulando planificador $sched"
	echo	
	cd results/
	mkdir -p $sched
	cd ../schedsim/
	./schedsim -i ./examples/example$sched.txt -s $sched -n $nCPUs
	for ((cpu=0; cpu<nCPUs;cpu++))
	do
		cp CPU_$cpu.log ../gantt-gplot/$sched.CPU_$cpu.log
		cp CPU_$cpu.log ../results/$sched/$sched.CPU_$cpu.log
		rm CPU_$cpu.log
	done
	cd ../gantt-gplot/
	for (( cpu=0; cpu<nCPUs;cpu++))
	do
		./generate_gantt_chart ./$sched.CPU_$cpu.log
		cp $sched.CPU_$cpu.eps ../results/$sched/$sched.CPU_$cpu.eps
		rm $sched.CPU_$cpu.log
		rm $sched.CPU_$cpu.eps
	done
	cd ../
	echo
	echo "~~~~~~~~~~~~~~~~~~~~~~~" 
done < "scheds.txt"
echo "Final"

