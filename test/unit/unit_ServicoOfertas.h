/** @file unit_ServicoOfertas.h
 * @brief Assinaturas dos testes unitarios da classe ServicoOfertas.
 */
#ifndef UNIT_SERVICOOFERTAS
#define UNIT_SERVICOOFERTAS

/** @brief Testa o construtor padrao (sem sistema) e o construtor com sistema. */
void unit_ServicoOfertas_constructor(void);
/** @brief Testa o construtor de copia (a copia compartilha o mesmo sistema). */
void unit_ServicoOfertas_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_ServicoOfertas_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o servico nao destroi o sistema. */
void unit_ServicoOfertas_destructor(void);
/** @brief Testa ServicoOfertas::publicar() (SB01). */
void unit_ServicoOfertas_publicar(void);
/** @brief Testa ServicoOfertas::consultar() (SB02). */
void unit_ServicoOfertas_consultar(void);
/** @brief Testa ServicoOfertas::detalhes() (SB02). */
void unit_ServicoOfertas_detalhes(void);
/** @brief Executa todos os testes unitarios de ServicoOfertas. */
void run_unit_test_ServicoOfertas(void);

#endif
