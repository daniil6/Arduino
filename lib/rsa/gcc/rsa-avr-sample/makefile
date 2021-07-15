all:
	gcc  -c  main.cpp -O2 -Wall -DGCC -DNDEBUG  -o build/main.cpp.o -I../.. -I.
	gcc -o build/rsa-avr-sample build/main.cpp.o -L../librsa-avr/build -lrsa-avr