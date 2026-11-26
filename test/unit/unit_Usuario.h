/** @file unit_Usuario.h
 * @brief Assinaturas dos testes unitarios da classe Usuario.
 */
#ifndef UNIT_USUARIO
#define UNIT_USUARIO

/** @brief Testa o construtor padrao e o construtor com parametros. */
void unit_Usuario_constructor(void);
/** @brief Testa o construtor de copia. */
void unit_Usuario_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_Usuario_assignmentOperator(void);
/** @brief Testa o destrutor: destruir o original nao afeta a copia. */
void unit_Usuario_destructor(void);
/** @brief Testa Usuario::id(). */
void unit_Usuario_id(void);
/** @brief Testa Usuario::email(). */
void unit_Usuario_email(void);
/** @brief Testa Usuario::perfil(). */
void unit_Usuario_perfil(void);
/** @brief Testa Usuario::idRestaurante(). */
void unit_Usuario_idRestaurante(void);
/** @brief Testa Usuario::conferirSenha(). */
void unit_Usuario_conferirSenha(void);
/** @brief Executa todos os testes unitarios de Usuario. */
void run_unit_test_Usuario(void);

#endif
