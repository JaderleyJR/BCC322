/** @file funcional_tests.h
 * @brief Declaracoes dos tres testes funcionais da Sprint 3.
 */
#ifndef SAVEFOOD_FUNCIONAL_TESTS_H
#define SAVEFOOD_FUNCIONAL_TESTS_H

/** @brief SB03: reserva valida e recusa por saldo insuficiente (CT-01 e CT-02). */
bool testeReservarQuantidade();  // SB03
/** @brief SB04: cancelamento com reposicao do saldo e recusa de repeticao (CT-01 e CT-02). */
bool testeCancelarReserva();     // SB04
/** @brief SB05: retirada valida e acesso de gerente de outro restaurante negado (CT-01 e CT-02). */
bool testeConfirmarRetirada();   // SB05

#endif
