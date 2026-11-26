/** @file unit_ServicoRetiradas.h
 * @brief Assinaturas dos testes unitarios da classe ServicoRetiradas.
 */
#ifndef UNIT_SERVICORETIRADAS
#define UNIT_SERVICORETIRADAS

/** @brief Testa o construtor padrao (sem sistema) e o construtor com sistema. */
void unit_ServicoRetiradas_constructor(void);
/** @brief Testa o construtor de copia (a copia compartilha o mesmo sistema). */
void unit_ServicoRetiradas_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_ServicoRetiradas_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o servico nao destroi o sistema. */
void unit_ServicoRetiradas_destructor(void);
/** @brief Testa ServicoRetiradas::listar() (SB05). */
void unit_ServicoRetiradas_listar(void);
/** @brief Testa ServicoRetiradas::confirmar() (SB05). */
void unit_ServicoRetiradas_confirmar(void);
/** @brief Executa todos os testes unitarios de ServicoRetiradas. */
void run_unit_test_ServicoRetiradas(void);

#endif
