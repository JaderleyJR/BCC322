/** @file flow.h
 * @brief Declaracao do fluxo demonstrativo.
 */
#ifndef SAVEFOOD_FLOW_H
#define SAVEFOOD_FLOW_H

#include "services.h"

#include <iosfwd>

// Executa uma demonstracao completa do MVP com dados ficticios.
/** @brief Executa o fluxo comercial com contas ficticias; lanca std::runtime_error se uma etapa for recusada.
 * @param sistema Sistema em memoria.
 * @param saida Fluxo de exibicao da demonstracao.
 * @throws std::runtime_error Se uma etapa for recusada.
 */
void executarDemonstracao(SistemaSaveFood& sistema, std::ostream& saida);

#endif
