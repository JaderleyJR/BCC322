/** @file unit_ServicoIndicadores.h
 * @brief Assinaturas dos testes unitarios da classe ServicoIndicadores.
 */
#ifndef UNIT_SERVICOINDICADORES
#define UNIT_SERVICOINDICADORES

/** @brief Testa o construtor padrao (sem sistema) e o construtor com sistema. */
void unit_ServicoIndicadores_constructor(void);
/** @brief Testa o construtor de copia (a copia compartilha o mesmo sistema). */
void unit_ServicoIndicadores_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_ServicoIndicadores_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o servico nao destroi o sistema. */
void unit_ServicoIndicadores_destructor(void);
/** @brief Testa ServicoIndicadores::obterResumo() (SB06). */
void unit_ServicoIndicadores_obterResumo(void);
/** @brief Executa todos os testes unitarios de ServicoIndicadores. */
void run_unit_test_ServicoIndicadores(void);

#endif
