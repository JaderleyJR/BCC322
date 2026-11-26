/** @file
 * @brief Ponto de entrada dos testes funcionais.
 */
#include "funcional_tests.h"
#include "../../src/model.h"
#include "../../src/system.h"
#include "../../src/flow.h"

#include <iostream>

/** @brief Executa os tres testes e retorna 1 se algum falhar, ou 0 em sucesso. */
int main() {
    std::cout << "=== SaveFood - testes funcionais ===\n";
    const bool teste1 = testeReservarQuantidade();
    const bool teste2 = testeCancelarReserva();
    const bool teste3 = testeConfirmarRetirada();

    if (teste1 && teste2 && teste3) {
        std::cout << "Todos os 3 testes funcionais passaram.\n";
        return 0;
    }
    return 1;
}
