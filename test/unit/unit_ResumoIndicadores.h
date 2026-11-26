/** @file unit_ResumoIndicadores.h
 * @brief Assinaturas dos testes unitarios de ResumoIndicadores.
 */
#ifndef UNIT_RESUMOINDICADORES
#define UNIT_RESUMOINDICADORES

/** @brief Testa o construtor padrao (indicadores zerados). */
void unit_ResumoIndicadores_constructor(void);
/** @brief Testa o construtor de copia. */
void unit_ResumoIndicadores_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_ResumoIndicadores_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o original nao afeta a copia. */
void unit_ResumoIndicadores_destructor(void);
/** @brief Executa todos os testes unitarios de ResumoIndicadores. */
void run_unit_test_ResumoIndicadores(void);

#endif
