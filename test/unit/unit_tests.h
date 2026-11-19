/** @file unit_tests.h
 * @brief Declaracoes dos tres testes unitarios.
 */
#ifndef SAVEFOOD_UNIT_TESTS_H
#define SAVEFOOD_UNIT_TESTS_H

/** @brief Verifica forma canonica de Oferta e Usuario e rejeicao de dados invalidos. */
bool testeCopiaDeOferta();
/** @brief Verifica copia, atribuicao e regras de estado de Reserva. */
bool testeCopiaDeReserva();
/** @brief Verifica copia e atribuicao de SistemaSaveFood. */
bool testeCopiaDoSistema();

#endif
