/** @file unit_SistemaSaveFood.h
 * @brief Assinaturas dos testes unitarios da classe SistemaSaveFood.
 */
#ifndef UNIT_SISTEMASAVEFOOD
#define UNIT_SISTEMASAVEFOOD

/** @brief Testa o construtor padrao (contas de demonstracao disponiveis). */
void unit_SistemaSaveFood_constructor(void);
/** @brief Testa o construtor de copia (dados copiados, estados independentes). */
void unit_SistemaSaveFood_copyConstructor(void);
/** @brief Testa o operador de atribuicao por copia. */
void unit_SistemaSaveFood_assignmentOperator(void);
/** @brief Testa o destrutor: destruir uma copia nao afeta o original. */
void unit_SistemaSaveFood_destructor(void);
/** @brief Testa SistemaSaveFood::entrar(). */
void unit_SistemaSaveFood_entrar(void);
/** @brief Testa SistemaSaveFood::sair(). */
void unit_SistemaSaveFood_sair(void);
/** @brief Testa SistemaSaveFood::publicarOferta() (SB01). */
void unit_SistemaSaveFood_publicarOferta(void);
/** @brief Testa SistemaSaveFood::consultarOfertas() (SB02). */
void unit_SistemaSaveFood_consultarOfertas(void);
/** @brief Testa SistemaSaveFood::detalhesOferta() (SB02). */
void unit_SistemaSaveFood_detalhesOferta(void);
/** @brief Testa SistemaSaveFood::reservar() (SB03), inclusive a disputa concorrente da secao 3.3. */
void unit_SistemaSaveFood_reservar(void);
/** @brief Testa SistemaSaveFood::cancelarReserva() (SB04). */
void unit_SistemaSaveFood_cancelarReserva(void);
/** @brief Testa SistemaSaveFood::minhasReservas() (SB04). */
void unit_SistemaSaveFood_minhasReservas(void);
/** @brief Testa SistemaSaveFood::permitir(). */
void unit_SistemaSaveFood_permitir(void);
/** @brief Testa SistemaSaveFood::listarReservas() (SB05). */
void unit_SistemaSaveFood_listarReservas(void);
/** @brief Testa SistemaSaveFood::confirmarRetirada() (SB05). */
void unit_SistemaSaveFood_confirmarRetirada(void);
/** @brief Testa SistemaSaveFood::obterResumo() (SB06). */
void unit_SistemaSaveFood_obterResumo(void);
/** @brief Testa SistemaSaveFood::saldoOferta(). */
void unit_SistemaSaveFood_saldoOferta(void);
/** @brief Executa todos os testes unitarios de SistemaSaveFood. */
void run_unit_test_SistemaSaveFood(void);

#endif
