/** @file
 * @brief Ponto de entrada dos testes unitarios.
 */
#ifndef MAIN_UNIT_TESTS
#define MAIN_UNIT_TESTS

#include "unit_tests.h"
#include "model.h"
#include "system.h"

#include <iostream>

/** @brief Executa os tres testes e retorna 1 se algum falhar, ou 0 em sucesso. */
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
