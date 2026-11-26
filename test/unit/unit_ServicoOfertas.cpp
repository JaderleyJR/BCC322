/** @file unit_ServicoOfertas.cpp
 * @brief Testes unitarios da classe ServicoOfertas.
 */
#include <assert.h>
#include "unit_ServicoOfertas.h"
#include "../../src/servicos/services.h"
#include "../../src/nucleo/system.h"  // teste de caixa-branca: usa a classe concreta

namespace {
const DadosOferta MARMITA{"Marmita", 5, 2.5, 12.0, "18h-19h"};
}

void unit_ServicoOfertas_constructor(void) {
    ServicoOfertas semSistema;
    Oferta o;
    assert(!semSistema.publicar(MARMITA, o));
    assert(semSistema.consultar(FiltroOferta{}).empty());

    SistemaSaveFood sistema;
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    ServicoOfertas ofertas(sistema, g);
    assert(ofertas.publicar(MARMITA, o));
}

void unit_ServicoOfertas_copyConstructor(void) {
    SistemaSaveFood sistema;
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    ServicoOfertas original(sistema, g);
    Oferta publicada;
    assert(original.publicar(MARMITA, publicada));
    ServicoOfertas copia(original);
    Oferta vista;
    assert(copia.detalhes(publicada.id(), vista)); // mesmo sistema e mesma sessao
}

void unit_ServicoOfertas_assignmentOperator(void) {
    SistemaSaveFood sistema;
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    ServicoOfertas atribuido;
    Oferta o;
    assert(!atribuido.publicar(MARMITA, o));
    atribuido = ServicoOfertas(sistema, g);
    assert(atribuido.publicar(MARMITA, o));
}

void unit_ServicoOfertas_destructor(void) {
    SistemaSaveFood sistema;
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    Oferta publicada;
    {
        ServicoOfertas temporario(sistema, g);
        assert(temporario.publicar(MARMITA, publicada));
    }
    assert(sistema.saldoOferta(g, publicada.id()) == 5); // oferta permanece no sistema
}

void unit_ServicoOfertas_publicar(void) {
    SistemaSaveFood sistema;
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    Sessao c = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    ServicoOfertas doGerente(sistema, g), doConsumidor(sistema, c);

    Oferta criada;
    assert(doGerente.publicar(MARMITA, criada));            // SB01 CT-01
    assert(criada.quantidadeDisponivel() == 5 && criada.idRestaurante() == 1);
    Oferta naoCriada;
    DadosOferta semJanela{"Marmita", 5, 2.5, 12.0, ""};
    assert(!doGerente.publicar(semJanela, naoCriada));      // SB01 CT-02
    assert(naoCriada.id() == 0);
    assert(!doConsumidor.publicar(MARMITA, naoCriada));     // consumidor nao publica
}

void unit_ServicoOfertas_consultar(void) {
    SistemaSaveFood sistema;
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    Sessao c = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    ServicoOfertas doGerente(sistema, g), doConsumidor(sistema, c);
    Oferta o;
    assert(doGerente.publicar(MARMITA, o));

    assert(doConsumidor.consultar(FiltroOferta{"marmita"}).size() == 1);
    assert(doConsumidor.consultar(FiltroOferta{""}).size() == 1);
    assert(doConsumidor.consultar(FiltroOferta{"sopa"}).empty()); // SB02 CT-02
}

void unit_ServicoOfertas_detalhes(void) {
    SistemaSaveFood sistema;
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    Sessao c = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    ServicoOfertas doGerente(sistema, g), doConsumidor(sistema, c);
    Oferta publicada, vista;
    assert(doGerente.publicar(MARMITA, publicada));
    assert(doConsumidor.detalhes(publicada.id(), vista));
    assert(vista.janelaRetirada() == "18h-19h");
    assert(vista.quantidadeDisponivel() == 5);        // leitura nao altera saldo
    assert(!doConsumidor.detalhes(999, vista));
}

void run_unit_test_ServicoOfertas(void) {
    unit_ServicoOfertas_constructor();
    unit_ServicoOfertas_copyConstructor();
    unit_ServicoOfertas_assignmentOperator();
    unit_ServicoOfertas_destructor();
    unit_ServicoOfertas_publicar();
    unit_ServicoOfertas_consultar();
    unit_ServicoOfertas_detalhes();
}
