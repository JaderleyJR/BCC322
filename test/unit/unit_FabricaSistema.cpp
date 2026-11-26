/** @file unit_FabricaSistema.cpp
 * @brief Testes unitarios da classe FabricaSistema.
 */
#include <assert.h>
#include <memory>
#include <type_traits>
#include "unit_FabricaSistema.h"
#include "../../src/nucleo/system.h"

void unit_FabricaSistema_constructor(void) {
    // Fabrica so com metodos estaticos: nao ha objetos FabricaSistema.
    static_assert(!std::is_default_constructible<FabricaSistema>::value,
                  "FabricaSistema nao deve ser instanciavel");
}

void unit_FabricaSistema_criarSistema(void) {
    std::unique_ptr<ISistemaSaveFood> sistema = FabricaSistema::criarSistema();
    assert(sistema != nullptr);
    // A implementacao entregue pelo componente nucleo e o SistemaSaveFood.
    assert(dynamic_cast<SistemaSaveFood*>(sistema.get()) != nullptr);

    // O sistema criado ja possui as contas de demonstracao.
    Sessao g = sistema->entrar("gerente@savefood.demo", "gerente123");
    assert(g.ativa());
    assert(sistema->publicarOferta(g, "Marmita", 5, 2.5, 12.0, "18h-19h") == 1);

    // Cada chamada cria uma instancia nova e independente.
    std::unique_ptr<ISistemaSaveFood> outro = FabricaSistema::criarSistema();
    assert(outro.get() != sistema.get());
    Sessao gOutro = outro->entrar("gerente@savefood.demo", "gerente123");
    assert(outro->saldoOferta(gOutro, 1) == -1);  // oferta so existe no primeiro
    assert(sistema->saldoOferta(g, 1) == 5);
}   // os unique_ptr destroem as implementacoes pela interface (destrutor virtual)

void run_unit_test_FabricaSistema(void) {
    unit_FabricaSistema_constructor();
    unit_FabricaSistema_criarSistema();
}
