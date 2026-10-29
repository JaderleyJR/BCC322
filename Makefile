CXX ?= g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
FONTES = src/model.cpp src/system.cpp src/services.cpp src/flow.cpp

.PHONY: all test clean

all: bin/savefood bin/testes_funcionais bin/testes_unitarios

bin/savefood: src/main.cpp $(FONTES) src/model.h src/system.h src/services.h src/flow.h
	$(CXX) $(CXXFLAGS) src/main.cpp $(FONTES) -o bin/savefood

bin/testes_funcionais: test/funcional/main.cpp test/funcional/funcional_tests.cpp $(FONTES) src/services.h
	$(CXX) $(CXXFLAGS) test/funcional/main.cpp test/funcional/funcional_tests.cpp $(FONTES) -o bin/testes_funcionais

bin/testes_unitarios: test/unit/main.cpp test/unit/unit_tests.cpp $(FONTES) src/services.h
	$(CXX) $(CXXFLAGS) test/unit/main.cpp test/unit/unit_tests.cpp $(FONTES) -o bin/testes_unitarios

test: all
	./bin/testes_unitarios
	./bin/testes_funcionais

clean:
	rm -f bin/savefood bin/testes_funcionais bin/testes_unitarios
