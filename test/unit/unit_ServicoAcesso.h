/** @file unit_ServicoAcesso.h
 * @brief Assinaturas dos testes unitarios da classe ServicoAcesso.
 */
#ifndef UNIT_SERVICOACESSO
#define UNIT_SERVICOACESSO

/** @brief Testa o construtor padrao (sem sistema) e o construtor com sistema. */
void unit_ServicoAcesso_constructor(void);
/** @brief Testa o construtor de copia (a copia compartilha o mesmo sistema). */
void unit_ServicoAcesso_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_ServicoAcesso_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o servico nao destroi o sistema. */
void unit_ServicoAcesso_destructor(void);
/** @brief Testa ServicoAcesso::permitir(). */
void unit_ServicoAcesso_permitir(void);
/** @brief Executa todos os testes unitarios de ServicoAcesso. */
void run_unit_test_ServicoAcesso(void);

#endif
