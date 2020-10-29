CXX = g++
CXXFLAGS = -g -std=c++11 -Wall
LFLAGS = -pthread
TARGET = server

$(TARGET): main.o threadpool.o calculator.o machine.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o calculator.o machine.o $(LFLAGS)

main.o: main.cpp threadpool.h
	$(CXX) $(CXXFLAGS) -c main.cpp

threadpool.o: threadpool.h calculator.h
	$(CXX) $(CXXFLAGS) -c threadpool.h

calculator.o: calculator.h machine.h
	$(CXX) $(CXXFLAGS) -c calculator.cpp

machine.o: machine.h
	$(CXX) $(CXXFLAGS) -c machine.cpp

clean:
	rm -f *.o
	rm -f $(TARGET)  
