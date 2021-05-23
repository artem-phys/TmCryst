for i in 0 1 2 3 4 5 6 7 8 9
do
	echo "nohup ./TmCryst macros/run_source_${i}.mac &>test${i}.log &" >> sim.sh
	echo "nohup ./TmCryst macros/specsource_${i}.mac &>spectest${i}.log &" >> sim.sh
done
