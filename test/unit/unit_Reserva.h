/** @file unit_Reserva.h
 * @brief Assinaturas dos testes unitarios da classe Reserva.
 */
#ifndef UNIT_RESERVA
#define UNIT_RESERVA

/** @brief Testa o construtor padrao e o construtor com parametros. */
void unit_Reserva_constructor(void);
/** @brief Testa o construtor de copia (copia independente do original). */
void unit_Reserva_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_Reserva_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o original nao afeta a copia. */
void unit_Reserva_destructor(void);
/** @brief Testa Reserva::cancelar(). */
void unit_Reserva_cancelar(void);
/** @brief Testa Reserva::confirmarRetirada(). */
void unit_Reserva_confirmarRetirada(void);
/** @brief Testa Reserva::id(). */
void unit_Reserva_id(void);
/** @brief Testa Reserva::idOferta(). */
void unit_Reserva_idOferta(void);
/** @brief Testa Reserva::idConsumidor(). */
void unit_Reserva_idConsumidor(void);
/** @brief Testa Reserva::quantidadeReservada(). */
void unit_Reserva_quantidadeReservada(void);
/** @brief Testa Reserva::quantidadeRetirada(). */
void unit_Reserva_quantidadeRetirada(void);
/** @brief Testa Reserva::valorRecebido(). */
void unit_Reserva_valorRecebido(void);
/** @brief Testa Reserva::pesoAproveitadoKg(). */
void unit_Reserva_pesoAproveitadoKg(void);
/** @brief Testa Reserva::retiradaConfirmada(). */
void unit_Reserva_retiradaConfirmada(void);
/** @brief Testa Reserva::situacao(). */
void unit_Reserva_situacao(void);
/** @brief Executa todos os testes unitarios de Reserva. */
void run_unit_test_Reserva(void);

#endif
