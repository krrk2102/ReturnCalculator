CFLAGS=--std=c++11

returnCalc: main.o MonthlyData.o
	g++ -o returnCalc $(CFLAGS) main.o MonthlyData.o
	rm main.o MonthlyData.o
	./returnCalc

%.o: %.cpp %.h
	g++ -c $(CFLAGS) $<

clean:
	rm returnCalc
