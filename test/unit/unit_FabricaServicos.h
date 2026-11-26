/** @file unit_FabricaServicos.h
 * @brief Assinaturas dos testes unitarios da classe FabricaServicos.
 */
#ifndef UNIT_FABRICASERVICOS
#define UNIT_FABRICASERVICOS

/** @brief Verifica que a fabrica nao pode ser instanciada (construtor removido). */
void unit_FabricaServicos_constructor(void);
/** @brief Testa FabricaServicos::criarAutenticacao(). */
void unit_FabricaServicos_criarAutenticacao(void);
/** @brief Testa FabricaServicos::criarAcesso(). */
void unit_FabricaServicos_criarAcesso(void);
/** @brief Testa FabricaServicos::criarOfertas(). */
void unit_FabricaServicos_criarOfertas(void);
/** @brief Testa FabricaServicos::criarReservas(). */
void unit_FabricaServicos_criarReservas(void);
/** @brief Testa FabricaServicos::criarRetiradas(). */
void unit_FabricaServicos_criarRetiradas(void);
/** @brief Testa FabricaServicos::criarIndicadores(). */
void unit_FabricaServicos_criarIndicadores(void);
/** @brief Executa todos os testes unitarios de FabricaServicos. */
void run_unit_test_FabricaServicos(void);

#endif
