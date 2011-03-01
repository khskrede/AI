
# comment line

C++          = g++ 
C++-FLAGS    = -c -Wall -std=c++0x -g
LINKER       = g++
LINKER-FLAGS = -o
RM           = rm -f
MAKE         = make
OBJS         = main.o basicea.o strategies.o population.o individual.o
TARGET       = main

$(TARGET): $(OBJS) 
	$(LINKER) $(LINKER-FLAGS) $(TARGET) $(OBJS)  

main.o : main.cpp
	$(C++) $(C++-FLAGS) main.cpp

basicea.o : basicea.cpp
	$(C++) $(C++-FLAGS) basicea.cpp

population.o : population.cpp
	$(C++) $(C++-FLAGS) population.cpp

individual.o : individual.cpp
	$(C++) $(C++-FLAGS) individual.cpp

strategies.o : strategies.cpp
	$(C++) $(C++-FLAGS) strategies.cpp

clean : 
	rm *.o main data.ea

cleandata : 
	rm ./data/*
