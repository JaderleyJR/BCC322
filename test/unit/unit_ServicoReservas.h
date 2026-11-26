/** @file unit_ServicoReservas.h
 * @brief Assinaturas dos testes unitarios da classe ServicoReservas.
 */
#ifndef UNIT_SERVICORESERVAS
#define UNIT_SERVICORESERVAS

/** @brief Testa o construtor padrao (sem sistema) e o construtor com sistema. */
void unit_ServicoReservas_constructor(void);
/** @brief Testa o construtor de copia (a copia compartilha o mesmo sistema). */
void unit_ServicoReservas_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_ServicoReservas_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o servico nao destroi o sistema. */
void unit_ServicoReservas_destructor(void);
/** @brief Testa ServicoReservas::reservar() (SB03). */
void unit_ServicoReservas_reservar(void);
/** @brief Testa ServicoReservas::cancelar() (SB04). */
void unit_ServicoReservas_cancelar(void);
/** @brief Testa ServicoReservas::minhasReservas() (SB04). */
void unit_ServicoReservas_minhasReservas(void);
/** @brief Executa todos os testes unitarios de ServicoReservas. */
void run_unit_test_ServicoReservas(void);

#endif
