/** @file unit_Oferta.h
 * @brief Assinaturas dos testes unitarios da classe Oferta.
 */
#ifndef UNIT_OFERTA
#define UNIT_OFERTA

/** @brief Testa o construtor padrao e o construtor com parametros. */
void unit_Oferta_constructor(void);
/** @brief Testa o construtor de copia (copia independente do original). */
void unit_Oferta_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_Oferta_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o original nao afeta a copia. */
void unit_Oferta_destructor(void);
/** @brief Testa Oferta::valida(). */
void unit_Oferta_valida(void);
/** @brief Testa Oferta::reservar(). */
void unit_Oferta_reservar(void);
/** @brief Testa Oferta::restaurarDisponibilidade(). */
void unit_Oferta_restaurarDisponibilidade(void);
/** @brief Testa Oferta::id(). */
void unit_Oferta_id(void);
/** @brief Testa Oferta::idRestaurante(). */
void unit_Oferta_idRestaurante(void);
/** @brief Testa Oferta::descricao(). */
void unit_Oferta_descricao(void);
/** @brief Testa Oferta::quantidadeTotal(). */
void unit_Oferta_quantidadeTotal(void);
/** @brief Testa Oferta::quantidadeDisponivel(). */
void unit_Oferta_quantidadeDisponivel(void);
/** @brief Testa Oferta::pesoPorUnidadeKg(). */
void unit_Oferta_pesoPorUnidadeKg(void);
/** @brief Testa Oferta::precoUnitario(). */
void unit_Oferta_precoUnitario(void);
/** @brief Testa Oferta::janelaRetirada(). */
void unit_Oferta_janelaRetirada(void);
/** @brief Testa Oferta::ativa(). */
void unit_Oferta_ativa(void);
/** @brief Testa Oferta::estaAtiva(). */
void unit_Oferta_estaAtiva(void);
/** @brief Executa todos os testes unitarios de Oferta. */
void run_unit_test_Oferta(void);

#endif
