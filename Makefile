# SaveFood - Sprint 7: arquitetura em camadas com componentes (.so)
#
# Camada 1  Dominio      -> bin/libsavefood_dominio.so
# Camada 2  Nucleo       -> bin/libsavefood_nucleo.so    (depende de dominio)
# Camada 3  Servicos     -> bin/libsavefood_servicos.so  (depende de nucleo e dominio)
# Camada 4  Apresentacao -> bin/savefood                 (usa as tres bibliotecas)
#
# As bibliotecas sao compiladas com -fvisibility=hidden: so o que esta
# marcado com SAVEFOOD_<CAMADA>_API (src/exportacao.h) fica visivel.
# Os executaveis procuram as bibliotecas na propria pasta bin/ (rpath $ORIGIN).

CXX      ?= g++
CXXFLAGS  = -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
LIBFLAGS  = -fPIC -fvisibility=hidden -shared
RPATH     = -Wl,-rpath,'$$ORIGIN'

LIB_DOMINIO  = bin/libsavefood_dominio.so
LIB_NUCLEO   = bin/libsavefood_nucleo.so
LIB_SERVICOS = bin/libsavefood_servicos.so
LIBS         = $(LIB_DOMINIO) $(LIB_NUCLEO) $(LIB_SERVICOS)
LINK_LIBS    = -Lbin -lsavefood_servicos -lsavefood_nucleo -lsavefood_dominio $(RPATH)

EXPORTACAO       = src/exportacao.h
FONTES_DOMINIO   = src/dominio/model.cpp
FONTES_NUCLEO    = src/nucleo/system.cpp src/nucleo/fabrica_sistema.cpp
FONTES_SERVICOS  = src/servicos/services.cpp src/servicos/fabrica_servicos.cpp
FONTES_APRESENT  = src/apresentacao/flow.cpp
CAB_DOMINIO      = $(EXPORTACAO) $(wildcard src/dominio/*.h)
CAB_NUCLEO       = $(CAB_DOMINIO) $(wildcard src/nucleo/*.h)
CAB_SERVICOS     = $(CAB_NUCLEO) $(wildcard src/servicos/*.h)
CAB_APRESENT     = $(CAB_SERVICOS) $(wildcard src/apresentacao/*.h)

UNIT_FONTES     = test/unit/main.cpp $(filter-out test/unit/main.cpp,$(wildcard test/unit/*.cpp))
UNIT_CABECALHOS = $(wildcard test/unit/*.h)
FUNC_FONTES     = test/funcional/main.cpp test/funcional/funcional_tests.cpp
FUNC_CABECALHOS = test/funcional/funcional_tests.h

.PHONY: all libs test clean docs simbolos

all: libs bin/savefood bin/testes_funcionais bin/testes_unitarios

libs: $(LIBS)

# ---- Componentes (bibliotecas compartilhadas) ----
$(LIB_DOMINIO): $(FONTES_DOMINIO) $(CAB_DOMINIO) | bin
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) -DSAVEFOOD_DOMINIO_EXPORTA $(FONTES_DOMINIO) -o $@

$(LIB_NUCLEO): $(FONTES_NUCLEO) $(CAB_NUCLEO) $(LIB_DOMINIO) | bin
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) -DSAVEFOOD_NUCLEO_EXPORTA $(FONTES_NUCLEO) \
	    -Lbin -lsavefood_dominio $(RPATH) -o $@

$(LIB_SERVICOS): $(FONTES_SERVICOS) $(CAB_SERVICOS) $(LIB_NUCLEO) | bin
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) -DSAVEFOOD_SERVICOS_EXPORTA $(FONTES_SERVICOS) \
	    -Lbin -lsavefood_nucleo -lsavefood_dominio $(RPATH) -o $@

# ---- Apresentacao (executavel) ----
bin/savefood: src/main.cpp $(FONTES_APRESENT) $(CAB_APRESENT) $(LIBS) | bin
	$(CXX) $(CXXFLAGS) src/main.cpp $(FONTES_APRESENT) $(LINK_LIBS) -o $@

# ---- Testes (usam os mesmos componentes) ----
bin/testes_funcionais: $(FUNC_FONTES) $(FUNC_CABECALHOS) $(CAB_SERVICOS) $(LIBS) | bin
	$(CXX) $(CXXFLAGS) $(FUNC_FONTES) $(LINK_LIBS) -o $@

# -UNDEBUG garante que os assert dos testes unitarios nunca sejam desligados.
bin/testes_unitarios: $(UNIT_FONTES) $(UNIT_CABECALHOS) $(FONTES_APRESENT) $(CAB_APRESENT) $(LIBS) | bin
	$(CXX) $(CXXFLAGS) -UNDEBUG $(UNIT_FONTES) $(FONTES_APRESENT) $(LINK_LIBS) -o $@

bin:
	mkdir -p bin

test: all
	./bin/testes_unitarios
	./bin/testes_funcionais

# Lista os simbolos que cada componente exporta.
simbolos: libs
	@for lib in $(LIBS); do echo "== $$lib"; nm -DC --defined-only $$lib | grep -c ' [TWV] ' ; done

clean:
	rm -f bin/savefood bin/testes_funcionais bin/testes_unitarios bin/*.so
	rm -f bin/*.exe bin/*.dll bin/*.dll.a

# Requer Doxygen e Graphviz no PATH.
docs:
	doxygen Doxyfile
