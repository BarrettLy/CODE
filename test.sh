while true
do
	time1=$(date +%s)
	./make.sh
	time2=$(date +%s)
	((difftime=$time2-$time1))
	echo $difftime
	echo "运算时间 : $difftime" >> log/testresult.txt
done
