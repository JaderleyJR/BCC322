/** @file fabrica_sistema.cpp
 * @brief Implementacao da FabricaSistema e do destrutor da interface do nucleo.
 */
#include "system.h"

// Definido aqui (e nao inline) para que a tabela virtual e as informacoes de
// tipo de ISistemaSaveFood fiquem dentro do componente savefood_nucleo.
ISistemaSaveFood::~ISistemaSaveFood() = default;

std::unique_ptr<ISistemaSaveFood> FabricaSistema::criarSistema() {
    return std::make_unique<SistemaSaveFood>();
}
