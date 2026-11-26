/** @file unit_ServicoAutenticacao.h
 * @brief Assinaturas dos testes unitarios da classe ServicoAutenticacao.
 */
#ifndef UNIT_SERVICOAUTENTICACAO
#define UNIT_SERVICOAUTENTICACAO

/** @brief Testa o construtor padrao (sem sistema) e o construtor com sistema. */
void unit_ServicoAutenticacao_constructor(void);
/** @brief Testa o construtor de copia (a copia compartilha o mesmo sistema). */
void unit_ServicoAutenticacao_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_ServicoAutenticacao_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o servico nao destroi o sistema. */
void unit_ServicoAutenticacao_destructor(void);
/** @brief Testa ServicoAutenticacao::entrar(). */
void unit_ServicoAutenticacao_entrar(void);
/** @brief Testa ServicoAutenticacao::sair(). */
void unit_ServicoAutenticacao_sair(void);
/** @brief Executa todos os testes unitarios de ServicoAutenticacao. */
void run_unit_test_ServicoAutenticacao(void);

#endif
