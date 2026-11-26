/** @file unit_ServicoReservas.cpp
 * @brief Testes unitarios da classe ServicoReservas.
 */
#include <assert.h>
#include "unit_ServicoReservas.h"
#include "../../src/services.h"

namespace {
/** @brief Publica 5 marmitas pelo gerente A e retorna o ID da oferta. */
int prepararOferta(SistemaSaveFood& sistema) {
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    return sistema.publicarOferta(g, "Marmita", 5, 2.5, 12.0, "18h-19h");
}
}

void unit_ServicoReservas_constructor(void) {
    ServicoReservas semSistema;
    Reserva r;
    assert(!semSistema.reservar(1, 1, r));
    assert(!semSistema.cancelar(1));
    assert(semSistema.minhasReservas().empty());

    SistemaSaveFood sistema;
    int oferta = prepararOferta(sistema);
    ServicoReservas reservas(sistema, sistema.entrar("consumidor@savefood.demo", "consumidor123"));
    assert(reservas.reservar(oferta, 1, r));
}

void unit_ServicoReservas_copyConstructor(void) {
    SistemaSaveFood sistema;
    int oferta = prepararOferta(sistema);
    ServicoReservas original(sistema, sistema.entrar("consumidor@savefood.demo", "consumidor123"));
    Reserva r;
    assert(original.reservar(oferta, 1, r));
    ServicoReservas copia(original);
    assert(copia.minhasReservas().size() == 1); // mesma sessao e mesmo sistema
}

void unit_ServicoReservas_assignmentOperator(void) {
    SistemaSaveFood sistema;
    int oferta = prepararOferta(sistema);
    ServicoReservas atribuido;
    atribuido = ServicoReservas(sistema, sistema.entrar("consumidor@savefood.demo", "consumidor123"));
    Reserva r;
    assert(atribuido.reservar(oferta, 2, r));
    assert(atribuido.minhasReservas().size() == 1);
}

void unit_ServicoReservas_destructor(void) {
    SistemaSaveFood sistema;
    int oferta = prepararOferta(sistema);
    Sessao c = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    {
        ServicoReservas temporario(sistema, c);
        Reserva r;
        assert(temporario.reservar(oferta, 2, r));
    }
    assert(sistema.minhasReservas(c).size() == 1); // reserva continua registrada
}

void unit_ServicoReservas_reservar(void) {
    SistemaSaveFood sistema;
    int oferta = prepararOferta(sistema);
    Sessao c = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    ServicoReservas reservas(sistema, c);
    Reserva criada;
    assert(reservas.reservar(oferta, 2, criada));               // SB03 CT-01
    assert(criada.situacao() == EstadoReserva::Pendente);
    assert(sistema.saldoOferta(c, oferta) == 3);
    Reserva recusada;
    assert(!reservas.reservar(oferta, 4, recusada));            // acima do saldo
    assert(recusada.id() == 0 && sistema.saldoOferta(c, oferta) == 3);
}

void unit_ServicoReservas_cancelar(void) {
    SistemaSaveFood sistema;
    int oferta = prepararOferta(sistema);
    Sessao c1 = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    Sessao c2 = sistema.entrar("consumidor2@savefood.demo", "consumidor123");
    ServicoReservas dono(sistema, c1), terceiro(sistema, c2);
    Reserva r;
    assert(dono.reservar(oferta, 2, r));
    assert(!terceiro.cancelar(r.id()));
    assert(dono.cancelar(r.id()));                              // SB04 CT-01
    assert(sistema.saldoOferta(c1, oferta) == 5);
    assert(!dono.cancelar(r.id()));                             // SB04 CT-02
    assert(sistema.saldoOferta(c1, oferta) == 5);
}

void unit_ServicoReservas_minhasReservas(void) {
    SistemaSaveFood sistema;
    int oferta = prepararOferta(sistema);
    ServicoReservas c1(sistema, sistema.entrar("consumidor@savefood.demo", "consumidor123"));
    ServicoReservas c2(sistema, sistema.entrar("consumidor2@savefood.demo", "consumidor123"));
    Reserva r1, r2;
    assert(c1.reservar(oferta, 1, r1) && c2.reservar(oferta, 1, r2));
    auto lista = c1.minhasReservas();
    assert(lista.size() == 1 && lista.front().id() == r1.id()); // so as proprias
}

void run_unit_test_ServicoReservas(void) {
    unit_ServicoReservas_constructor();
    unit_ServicoReservas_copyConstructor();
    unit_ServicoReservas_assignmentOperator();
    unit_ServicoReservas_destructor();
    unit_ServicoReservas_reservar();
    unit_ServicoReservas_cancelar();
    unit_ServicoReservas_minhasReservas();
}
