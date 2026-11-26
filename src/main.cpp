/** @file
 * @brief Ponto de entrada do SaveFood (camada de apresentacao).
 *
 * O programa principal nao conhece nenhuma classe concreta: o sistema e
 * criado pela FabricaSistema e os servicos pela FabricaServicos, cada um
 * implementado em sua propria biblioteca compartilhada.
 */
#include "apresentacao/flow.h"

#include <exception>
#include <iostream>
#include <memory>

/** @brief Executa a demonstracao e trata falhas inesperadas.
 * @return 0 em sucesso, 1 em erro.
 */
int main() {
    try {
        std::cout << "=== SaveFood - demonstracao da Sprint 7 (arquitetura em camadas) ===\n";
        std::unique_ptr<ISistemaSaveFood> sistema = FabricaSistema::criarSistema();
        executarDemonstracao(*sistema, std::cout);
        return 0;
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
    } catch (...) {
        std::cerr << "Erro inesperado na demonstracao.\n";
    }
    return 1;
}
