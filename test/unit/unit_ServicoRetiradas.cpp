/** @file unit_ServicoRetiradas.cpp
 * @brief Testes unitarios da classe ServicoRetiradas.
 */
#include <assert.h>
#include "unit_ServicoRetiradas.h"
#include "../../src/services.h"

namespace {
const DadosRetirada ENTREGA{2, 1.0, 24.0};

/** @brief Cria oferta do gerente A e reserva de 2 unidades; retorna o ID da reserva. */
int prepararReserva(SistemaSaveFood& sistema) {
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    Sessao c = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    int oferta = sistema.publicarOferta(g, "Marmita", 5, 2.5, 12.0, "18h-19h");
    int idReserva = 0;
    sistema.reservar(c, oferta, 2, idReserva);
    return idReserva;
}
}

void unit_ServicoRetiradas_constructor(void) {
    ServicoRetiradas semSistema;
    assert(semSistema.listar().empty());
    assert(!semSistema.confirmar(1, ENTREGA));

    SistemaSaveFood sistema;
    prepararReserva(sistema);
    ServicoRetiradas retiradas(sistema, sistema.entrar("gerente@savefood.demo", "gerente123"));
    assert(retiradas.listar().size() == 1);
}

void unit_ServicoRetiradas_copyConstructor(void) {
    SistemaSaveFood sistema;
    int id = prepararReserva(sistema);
    ServicoRetiradas original(sistema, sistema.entrar("gerente@savefood.demo", "gerente123"));
    ServicoRetiradas copia(original);
    assert(copia.confirmar(id, ENTREGA));
    assert(original.listar().front().situacao() == EstadoReserva::Retirada);
}

void unit_ServicoRetiradas_assignmentOperator(void) {
    SistemaSaveFood sistema;
    int id = prepararReserva(sistema);
    ServicoRetiradas atribuido;
    atribuido = ServicoRetiradas(sistema, sistema.entrar("gerente@savefood.demo", "gerente123"));
    assert(atribuido.confirmar(id, ENTREGA));
}

void unit_ServicoRetiradas_destructor(void) {
    SistemaSaveFood sistema;
    int id = prepararReserva(sistema);
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    {
        ServicoRetiradas temporario(sistema, g);
        assert(temporario.confirmar(id, ENTREGA));
    }
    assert(sistema.listarReservas(g).front().retiradaConfirmada());
}

void unit_ServicoRetiradas_listar(void) {
    SistemaSaveFood sistema;
    int id = prepararReserva(sistema);
    ServicoRetiradas doA(sistema, sistema.entrar("gerente@savefood.demo", "gerente123"));
    ServicoRetiradas doB(sistema, sistema.entrar("gerente2@savefood.demo", "gerente123"));
    ServicoRetiradas doConsumidor(sistema, sistema.entrar("consumidor@savefood.demo", "consumidor123"));
    auto lista = doA.listar();
    assert(lista.size() == 1 && lista.front().id() == id);
    assert(doB.listar().empty());        // so reservas do proprio restaurante
    assert(doConsumidor.listar().empty());
}

void unit_ServicoRetiradas_confirmar(void) {
    SistemaSaveFood sistema;
    int id = prepararReserva(sistema);
    ServicoRetiradas doA(sistema, sistema.entrar("gerente@savefood.demo", "gerente123"));
    ServicoRetiradas doB(sistema, sistema.entrar("gerente2@savefood.demo", "gerente123"));
    assert(!doB.confirmar(id, ENTREGA));                       // SB05 CT-02
    assert(!doA.confirmar(id, DadosRetirada{1, 0.5, 12.0}));   // parcial
    assert(doA.confirmar(id, ENTREGA));                        // SB05 CT-01
    assert(!doA.confirmar(id, ENTREGA));                       // duplicada
    Reserva r = doA.listar().front();
    assert(r.quantidadeRetirada() == 2 && r.valorRecebido() == 24.0);
}

void run_unit_test_ServicoRetiradas(void) {
    unit_ServicoRetiradas_constructor();
    unit_ServicoRetiradas_copyConstructor();
    unit_ServicoRetiradas_assignmentOperator();
    unit_ServicoRetiradas_destructor();
    unit_ServicoRetiradas_listar();
    unit_ServicoRetiradas_confirmar();
}
