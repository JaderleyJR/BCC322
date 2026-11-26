/** @file unit_FabricaServicos.cpp
 * @brief Testes unitarios da classe FabricaServicos.
 *
 * Cada teste verifica que a fabrica entrega a implementacao concreta
 * esperada e que o servico criado opera sobre o sistema e a sessao recebidos.
 */
#include <assert.h>
#include <cmath>
#include <memory>
#include <type_traits>
#include "unit_FabricaServicos.h"
#include "../../src/servicos/services.h"

namespace {
const DadosOferta MARMITA{"Marmita", 5, 2.5, 12.0, "18h-19h"};
}

void unit_FabricaServicos_constructor(void) {
    static_assert(!std::is_default_constructible<FabricaServicos>::value,
                  "FabricaServicos nao deve ser instanciavel");
}

void unit_FabricaServicos_criarAutenticacao(void) {
    auto sistema = FabricaSistema::criarSistema();
    std::unique_ptr<IServicoAutenticacao> servico = FabricaServicos::criarAutenticacao(*sistema);
    assert(dynamic_cast<ServicoAutenticacao*>(servico.get()) != nullptr);
    Sessao s;
    assert(servico->entrar("consumidor@savefood.demo", "consumidor123", s));
    assert(sistema->permitir(s, Perfil::Consumidor)); // sessao aberta no sistema informado
    servico->sair(s);
    assert(!s.ativa());
}

void unit_FabricaServicos_criarAcesso(void) {
    auto sistema = FabricaSistema::criarSistema();
    Sessao g = sistema->entrar("gerente@savefood.demo", "gerente123");
    std::unique_ptr<IServicoAcesso> servico = FabricaServicos::criarAcesso(*sistema);
    assert(dynamic_cast<ServicoAcesso*>(servico.get()) != nullptr);
    assert(servico->permitir(g, Papel::Gerente, 1));
    assert(!servico->permitir(g, Papel::Consumidor));
}

void unit_FabricaServicos_criarOfertas(void) {
    auto sistema = FabricaSistema::criarSistema();
    Sessao g = sistema->entrar("gerente@savefood.demo", "gerente123");
    Sessao c = sistema->entrar("consumidor@savefood.demo", "consumidor123");
    auto doGerente = FabricaServicos::criarOfertas(*sistema, g);
    auto doConsumidor = FabricaServicos::criarOfertas(*sistema, c);
    assert(dynamic_cast<ServicoOfertas*>(doGerente.get()) != nullptr);
    Oferta publicada;
    assert(doGerente->publicar(MARMITA, publicada));     // usa a sessao do gerente
    assert(!doConsumidor->publicar(MARMITA, publicada)); // usa a sessao do consumidor
    assert(doConsumidor->consultar(FiltroOferta{"marmita"}).size() == 1);
}

void unit_FabricaServicos_criarReservas(void) {
    auto sistema = FabricaSistema::criarSistema();
    Sessao g = sistema->entrar("gerente@savefood.demo", "gerente123");
    Sessao c = sistema->entrar("consumidor@savefood.demo", "consumidor123");
    int oferta = sistema->publicarOferta(g, "Marmita", 5, 2.5, 12.0, "18h-19h");
    auto servico = FabricaServicos::criarReservas(*sistema, c);
    assert(dynamic_cast<ServicoReservas*>(servico.get()) != nullptr);
    Reserva r;
    assert(servico->reservar(oferta, 2, r));
    assert(servico->minhasReservas().size() == 1);
    assert(sistema->saldoOferta(c, oferta) == 3);
}

void unit_FabricaServicos_criarRetiradas(void) {
    auto sistema = FabricaSistema::criarSistema();
    Sessao g = sistema->entrar("gerente@savefood.demo", "gerente123");
    Sessao c = sistema->entrar("consumidor@savefood.demo", "consumidor123");
    int oferta = sistema->publicarOferta(g, "Marmita", 5, 2.5, 12.0, "18h-19h");
    int id = 0;
    assert(sistema->reservar(c, oferta, 2, id));
    auto servico = FabricaServicos::criarRetiradas(*sistema, g);
    assert(dynamic_cast<ServicoRetiradas*>(servico.get()) != nullptr);
    assert(servico->listar().size() == 1);
    assert(servico->confirmar(id, DadosRetirada{2, 1.0, 24.0}));
}

void unit_FabricaServicos_criarIndicadores(void) {
    auto sistema = FabricaSistema::criarSistema();
    Sessao g = sistema->entrar("gerente@savefood.demo", "gerente123");
    Sessao c = sistema->entrar("consumidor@savefood.demo", "consumidor123");
    int oferta = sistema->publicarOferta(g, "Marmita", 5, 2.5, 12.0, "18h-19h");
    int id = 0;
    assert(sistema->reservar(c, oferta, 2, id));
    assert(sistema->confirmarRetirada(g, id, 2, 1.0, 24.0));
    auto servico = FabricaServicos::criarIndicadores(*sistema, g);
    assert(dynamic_cast<ServicoIndicadores*>(servico.get()) != nullptr);
    ResumoIndicadores resumo = servico->obterResumo();
    assert(std::fabs(resumo.receitaBrutaReais - 24.0) < 1e-9);
}

void run_unit_test_FabricaServicos(void) {
    unit_FabricaServicos_constructor();
    unit_FabricaServicos_criarAutenticacao();
    unit_FabricaServicos_criarAcesso();
    unit_FabricaServicos_criarOfertas();
    unit_FabricaServicos_criarReservas();
    unit_FabricaServicos_criarRetiradas();
    unit_FabricaServicos_criarIndicadores();
}
