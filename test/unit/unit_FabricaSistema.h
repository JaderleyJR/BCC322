/** @file unit_FabricaSistema.h
 * @brief Assinaturas dos testes unitarios da classe FabricaSistema.
 */
#ifndef UNIT_FABRICASISTEMA
#define UNIT_FABRICASISTEMA

/** @brief Verifica que a fabrica nao pode ser instanciada (construtor removido). */
void unit_FabricaSistema_constructor(void);
/** @brief Testa FabricaSistema::criarSistema(). */
void unit_FabricaSistema_criarSistema(void);
/** @brief Executa todos os testes unitarios de FabricaSistema. */
void run_unit_test_FabricaSistema(void);

#endif
