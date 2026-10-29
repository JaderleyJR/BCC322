#ifndef MAIN_UNIT_TESTS
#define MAIN_UNIT_TESTS

#include "unit_tests.h"
#include "../../src/model.h"
#include "../../src/system.h"

#include <iostream>

int main() {
    std::cout << "=== SaveFood - testes unitarios ===\n";
    const bool a = testeCopiaDeOferta();
    const bool b = testeCopiaDeReserva();
    const bool c = testeCopiaDoSistema();
    if (a && b && c) {
        std::cout << "Todos os 3 testes unitarios passaram.\n";
        return 0;
    }
    return 1;
}

#endif
