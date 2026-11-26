/** @file unit_Sessao.h
 * @brief Assinaturas dos testes unitarios da classe Sessao.
 */
#ifndef UNIT_SESSAO
#define UNIT_SESSAO

/** @brief Testa o construtor padrao e o construtor privado (via SistemaSaveFood::entrar). */
void unit_Sessao_constructor(void);
/** @brief Testa o construtor de copia. */
void unit_Sessao_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_Sessao_assignmentOperator(void);
/** @brief Testa o destrutor: destruir uma copia nao encerra a sessao no sistema. */
void unit_Sessao_destructor(void);
/** @brief Testa Sessao::ativa(). */
void unit_Sessao_ativa(void);
/** @brief Testa Sessao::autenticada(). */
void unit_Sessao_autenticada(void);
/** @brief Executa todos os testes unitarios de Sessao. */
void run_unit_test_Sessao(void);

#endif
