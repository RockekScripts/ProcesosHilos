echo Cuantas CPUs mariquita
read nCPUs
while read -r sched
do
    echo "Text read from file: $sched"
cd schedsim/
./schedsim -i ./examples/exampleLOTT.txt -s $sched -n $nCPUs
for ((cpu=0; cpu<nCPUs;cpu++))
do
	cp CPU_$cpu.log ../gantt-gplot/$sched-CPU_$cpu.log
done
cd ../gantt-gplot/
for (( cpu=0; cpu<nCPUs;cpu++))
do
	./generate_gantt_chart ./$sched-CPU_$cpu.log
done
cd ../
done < "scheds.txt"

