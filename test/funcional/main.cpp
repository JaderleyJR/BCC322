#ifndef MAIN_FUNCIONAL_TESTS
#define MAIN_FUNCIONAL_TESTS

#include "funcional_tests.h"
#include "../../src/model.h"
#include "../../src/system.h"
#include "../../src/flow.h"

#include <iostream>

int main() {
    std::cout << "=== SaveFood - testes funcionais ===\n";
    const bool teste1 = testePublicacaoEConsulta();
    const bool teste2 = testeReservasConcorrentes();
    const bool teste3 = testeRetiradaEIndicadores();

    if (teste1 && teste2 && teste3) {
        std::cout << "Todos os 3 testes funcionais passaram.\n";
        return 0;
    }
    return 1;
}

#endif
