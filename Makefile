CXX ?= g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread

# Produto: todos os arquivos de src/ (cabecalhos e fontes).
FONTES      = src/model.cpp src/system.cpp src/services.cpp src/flow.cpp
CABECALHOS  = $(wildcard src/*.h)

# Testes unitarios: um par unit_<Classe>.h/.cpp por classe + unit_tests (globais).
UNIT_FONTES     = test/unit/main.cpp $(filter-out test/unit/main.cpp,$(wildcard test/unit/*.cpp))
UNIT_CABECALHOS = $(wildcard test/unit/*.h)

FUNC_FONTES     = test/funcional/main.cpp test/funcional/funcional_tests.cpp
FUNC_CABECALHOS = test/funcional/funcional_tests.h

.PHONY: all test clean docs

all: bin/savefood bin/testes_funcionais bin/testes_unitarios

bin/savefood: src/main.cpp $(FONTES) $(CABECALHOS) | bin
	$(CXX) $(CXXFLAGS) src/main.cpp $(FONTES) -o $@

bin/testes_funcionais: $(FUNC_FONTES) $(FUNC_CABECALHOS) $(FONTES) $(CABECALHOS) | bin
	$(CXX) $(CXXFLAGS) $(FUNC_FONTES) $(FONTES) -o $@

# -UNDEBUG garante que os assert dos testes unitarios nunca sejam desligados.
bin/testes_unitarios: $(UNIT_FONTES) $(UNIT_CABECALHOS) $(FONTES) $(CABECALHOS) | bin
	$(CXX) $(CXXFLAGS) -UNDEBUG $(UNIT_FONTES) $(FONTES) -o $@

bin:
	mkdir -p bin

test: all
	./bin/testes_unitarios
	./bin/testes_funcionais

clean:
	rm -f bin/savefood bin/testes_funcionais bin/testes_unitarios
	rm -f bin/savefood.exe bin/testes_funcionais.exe bin/testes_unitarios.exe

# Requer Doxygen e Graphviz no PATH.
docs:
	doxygen Doxyfile
