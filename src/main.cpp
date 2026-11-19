/** @file
 * @brief Ponto de entrada da demonstracao do SaveFood.
 */
#include "flow.h"
#include <exception>
#include <iostream>

/** @brief Executa a demonstracao e trata falhas inesperadas.
 * @return 0 em sucesso, 1 em erro.
 */
int main() {
    try {
        std::cout << "=== SaveFood - demonstracao da Sprint 4 ===\n";
        SistemaSaveFood sistema;
        executarDemonstracao(sistema, std::cout);
        return 0;
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
    } catch (...) {
        std::cerr << "Erro inesperado na demonstracao.\n";
    }
    return 1;
}
