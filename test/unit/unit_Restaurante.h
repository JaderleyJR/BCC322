/** @file unit_Restaurante.h
 * @brief Assinaturas dos testes unitarios da classe Restaurante.
 */
#ifndef UNIT_RESTAURANTE
#define UNIT_RESTAURANTE

/** @brief Testa o construtor padrao e o construtor com parametros. */
void unit_Restaurante_constructor(void);
/** @brief Testa o construtor de copia. */
void unit_Restaurante_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_Restaurante_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o original nao afeta a copia. */
void unit_Restaurante_destructor(void);
/** @brief Testa Restaurante::id(). */
void unit_Restaurante_id(void);
/** @brief Testa Restaurante::nome(). */
void unit_Restaurante_nome(void);
/** @brief Executa todos os testes unitarios de Restaurante. */
void run_unit_test_Restaurante(void);

#endif
