/** @file
 * @brief Ponto de entrada dos testes unitarios.
 *
 * Inclui o cabecalho de cada arquivo de teste e invoca a funcao
 * run_unit_test_...() correspondente. Um assert que falhe interrompe o
 * programa e informa o arquivo, a linha e a condicao violada.
 */
#include <iostream>

#include "unit_tests.h"
#include "unit_Restaurante.h"
#include "unit_Usuario.h"
#include "unit_Sessao.h"
#include "unit_Oferta.h"
#include "unit_Reserva.h"
#include "unit_ResumoIndicadores.h"
#include "unit_SistemaSaveFood.h"
#include "unit_ServicoAutenticacao.h"
#include "unit_ServicoAcesso.h"
#include "unit_ServicoOfertas.h"
#include "unit_ServicoReservas.h"
#include "unit_ServicoRetiradas.h"
#include "unit_ServicoIndicadores.h"

#ifdef NDEBUG
#error "Os testes unitarios usam assert: compile sem -DNDEBUG."
#endif

/** @brief Executa todos os testes unitarios; retorna 0 se todos passarem. */
int main() {
    std::cout << "=== SaveFood - testes unitarios ===\n";

    run_unit_test_globals();               std::cout << "[OK] globals\n";
    run_unit_test_Restaurante();           std::cout << "[OK] Restaurante\n";
    run_unit_test_Usuario();               std::cout << "[OK] Usuario\n";
    run_unit_test_Sessao();                std::cout << "[OK] Sessao\n";
    run_unit_test_Oferta();                std::cout << "[OK] Oferta\n";
    run_unit_test_Reserva();               std::cout << "[OK] Reserva\n";
    run_unit_test_ResumoIndicadores();     std::cout << "[OK] ResumoIndicadores\n";
    run_unit_test_SistemaSaveFood();       std::cout << "[OK] SistemaSaveFood\n";
    run_unit_test_ServicoAutenticacao();   std::cout << "[OK] ServicoAutenticacao\n";
    run_unit_test_ServicoAcesso();         std::cout << "[OK] ServicoAcesso\n";
    run_unit_test_ServicoOfertas();        std::cout << "[OK] ServicoOfertas\n";
    run_unit_test_ServicoReservas();       std::cout << "[OK] ServicoReservas\n";
    run_unit_test_ServicoRetiradas();      std::cout << "[OK] ServicoRetiradas\n";
    run_unit_test_ServicoIndicadores();    std::cout << "[OK] ServicoIndicadores\n";

    std::cout << "Todos os testes unitarios passaram.\n";
    return 0;
}
