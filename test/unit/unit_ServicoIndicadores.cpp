/** @file unit_ServicoIndicadores.cpp
 * @brief Testes unitarios da classe ServicoIndicadores.
 */
#include <assert.h>
#include <cmath>
#include "unit_ServicoIndicadores.h"
#include "../../src/servicos/services.h"
#include "../../src/nucleo/system.h"  // teste de caixa-branca: usa a classe concreta

namespace {
bool quase(double a, double b) { return std::fabs(a - b) < 1e-9; }

/** @brief Registra uma retirada confirmada de 2 unidades, 1 kg e R$ 24 no restaurante A. */
void prepararRetirada(SistemaSaveFood& sistema) {
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    Sessao c = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    int oferta = sistema.publicarOferta(g, "Marmita", 5, 2.5, 12.0, "18h-19h");
    int id = 0;
    sistema.reservar(c, oferta, 2, id);
    sistema.confirmarRetirada(g, id, 2, 1.0, 24.0);
}
}

void unit_ServicoIndicadores_constructor(void) {
    ServicoIndicadores semSistema;
    ResumoIndicadores vazio = semSistema.obterResumo();
    assert(quase(vazio.pesoAproveitadoKg, 0.0) && quase(vazio.receitaBrutaReais, 0.0));

    SistemaSaveFood sistema;
    prepararRetirada(sistema);
    ServicoIndicadores indicadores(sistema, sistema.entrar("gerente@savefood.demo", "gerente123"));
    assert(quase(indicadores.obterResumo().receitaBrutaReais, 24.0));
}

void unit_ServicoIndicadores_copyConstructor(void) {
    SistemaSaveFood sistema;
    prepararRetirada(sistema);
    ServicoIndicadores original(sistema, sistema.entrar("gerente@savefood.demo", "gerente123"));
    ServicoIndicadores copia(original);
    assert(quase(copia.obterResumo().pesoAproveitadoKg, 1.0));
}

void unit_ServicoIndicadores_assignmentOperator(void) {
    SistemaSaveFood sistema;
    prepararRetirada(sistema);
    ServicoIndicadores atribuido;
    atribuido = ServicoIndicadores(sistema, sistema.entrar("gerente@savefood.demo", "gerente123"));
    assert(quase(atribuido.obterResumo().pesoAproveitadoKg, 1.0));
}

void unit_ServicoIndicadores_destructor(void) {
    SistemaSaveFood sistema;
    prepararRetirada(sistema);
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    ServicoIndicadores* temporario = new ServicoIndicadores(sistema, g);
    delete temporario;
    assert(quase(sistema.obterResumo(g).receitaBrutaReais, 24.0));
}

void unit_ServicoIndicadores_obterResumo(void) {
    SistemaSaveFood sistema;
    prepararRetirada(sistema);
    ServicoIndicadores doA(sistema, sistema.entrar("gerente@savefood.demo", "gerente123"));
    ServicoIndicadores doB(sistema, sistema.entrar("gerente2@savefood.demo", "gerente123"));
    ServicoIndicadores doConsumidor(sistema, sistema.entrar("consumidor@savefood.demo", "consumidor123"));
    ResumoIndicadores a = doA.obterResumo();
    assert(quase(a.pesoAproveitadoKg, 1.0) && quase(a.receitaBrutaReais, 24.0));
    assert(quase(doB.obterResumo().receitaBrutaReais, 0.0));          // outro restaurante
    assert(quase(doConsumidor.obterResumo().receitaBrutaReais, 0.0)); // sem permissao
}

void run_unit_test_ServicoIndicadores(void) {
    unit_ServicoIndicadores_constructor();
    unit_ServicoIndicadores_copyConstructor();
    unit_ServicoIndicadores_assignmentOperator();
    unit_ServicoIndicadores_destructor();
    unit_ServicoIndicadores_obterResumo();
}
