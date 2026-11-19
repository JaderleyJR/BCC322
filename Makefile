CXX ?= g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
FONTES = src/model.cpp src/system.cpp src/services.cpp src/flow.cpp
CPPFLAGS += -Iinclude
CABECALHOS = $(wildcard include/*.h)

.PHONY: all test clean docs

all: bin/savefood bin/testes_funcionais bin/testes_unitarios

bin/savefood: src/main.cpp $(FONTES) $(CABECALHOS) | bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) src/main.cpp $(FONTES) -o bin/savefood

bin/testes_funcionais: test/funcional/main.cpp test/funcional/funcional_tests.cpp $(FONTES) $(CABECALHOS) test/funcional/funcional_tests.h | bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) test/funcional/main.cpp test/funcional/funcional_tests.cpp $(FONTES) -o bin/testes_funcionais

bin/testes_unitarios: test/unit/main.cpp test/unit/unit_tests.cpp $(FONTES) $(CABECALHOS) test/unit/unit_tests.h | bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) test/unit/main.cpp test/unit/unit_tests.cpp $(FONTES) -o bin/testes_unitarios

bin:
	mkdir -p bin

test: all
	./bin/testes_unitarios
	./bin/testes_funcionais

clean:
	rm -f bin/savefood bin/testes_funcionais bin/testes_unitarios

# Requer Doxygen e Graphviz no PATH.
docs:
	doxygen Doxyfile
