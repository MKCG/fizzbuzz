compile:
	gcc -O3 ./fizzbuzz.c -o fizzbuzz

run: compile
	./fizzbuzz

perf: compile
	 ./fizzbuzz | pv > /dev/null

stat: compile
	sudo perf stat ./fizzbuzz
