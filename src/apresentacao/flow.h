/** @file flow.h
 * @brief Declaracao do fluxo demonstrativo (camada de apresentacao).
 *
 * A apresentacao depende apenas das interfaces publicas (servicos_api.h e
 * sistema_api.h) e obtem os objetos pelas fabricas.
 */
#ifndef SAVEFOOD_FLOW_H
#define SAVEFOOD_FLOW_H

#include "../servicos/servicos_api.h"

#include <iosfwd>

// Executa uma demonstracao completa do MVP com dados ficticios.
/** @brief Executa o fluxo comercial com contas ficticias; lanca std::runtime_error se uma etapa for recusada.
 * @param sistema Sistema criado pela FabricaSistema.
 * @param saida Fluxo de exibicao da demonstracao.
 * @throws std::runtime_error Se uma etapa for recusada.
 */
void executarDemonstracao(ISistemaSaveFood& sistema, std::ostream& saida);

#endif
