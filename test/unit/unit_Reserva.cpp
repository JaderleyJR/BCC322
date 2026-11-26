/** @file unit_Reserva.cpp
 * @brief Testes unitarios da classe Reserva.
 */
#include <assert.h>
#include <cmath>
#include <limits>
#include "unit_Reserva.h"
#include "../../src/dominio/model.h"

namespace {
bool quase(double a, double b) { return std::fabs(a - b) < 1e-9; }
}

void unit_Reserva_constructor(void) {
    Reserva padrao;
    assert(padrao.id() == 0);
    assert(padrao.situacao() == EstadoReserva::Pendente);
    assert(!padrao.cancelar()); // reserva vazia nao muda de estado

    Reserva r(1, 20, 3, 4);
    assert(r.id() == 1 && r.idOferta() == 20 && r.idConsumidor() == 3);
    assert(r.quantidadeReservada() == 4 && r.quantidadeRetirada() == 0);
    assert(r.situacao() == EstadoReserva::Pendente);
}

void unit_Reserva_copyConstructor(void) {
    Reserva original(1, 20, 3, 4);
    Reserva copia(original);
    assert(copia.confirmarRetirada(4, 2.0, 30.0));
    assert(copia.situacao() == EstadoReserva::Retirada);
    assert(original.situacao() == EstadoReserva::Pendente);
}

void unit_Reserva_assignmentOperator(void) {
    Reserva original(1, 20, 3, 4);
    Reserva atribuida;
    atribuida = original;
    assert(atribuida.id() == 1 && atribuida.quantidadeReservada() == 4);
    assert(atribuida.cancelar());
    assert(original.situacao() == EstadoReserva::Pendente);
}

void unit_Reserva_destructor(void) {
    Reserva* original = new Reserva(1, 20, 3, 4);
    Reserva copia(*original);
    delete original;
    assert(copia.id() == 1);
    assert(copia.situacao() == EstadoReserva::Pendente);
}

void unit_Reserva_cancelar(void) {
    Reserva r(1, 20, 3, 4);
    assert(r.cancelar());
    assert(r.situacao() == EstadoReserva::Cancelada);
    assert(!r.cancelar());                        // cancelamento repetido
    assert(!r.confirmarRetirada(4, 2.0, 30.0));   // cancelada nao e retirada

    Reserva retirada(2, 20, 3, 4);
    assert(retirada.confirmarRetirada(4, 2.0, 30.0));
    assert(!retirada.cancelar());                 // retirada nao e cancelada
}

void unit_Reserva_confirmarRetirada(void) {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    Reserva r(1, 20, 3, 4);
    assert(!r.confirmarRetirada(2, 1.0, 20.0));   // retirada parcial
    assert(!r.confirmarRetirada(5, 2.5, 50.0));   // acima do reservado
    assert(!r.confirmarRetirada(4, 0.0, 30.0));   // peso invalido
    assert(!r.confirmarRetirada(4, nan, 30.0));
    assert(!r.confirmarRetirada(4, 2.0, 0.0));    // valor invalido
    assert(r.situacao() == EstadoReserva::Pendente);

    assert(r.confirmarRetirada(4, 2.0, 30.0));
    assert(r.situacao() == EstadoReserva::Retirada);
    assert(!r.confirmarRetirada(4, 2.0, 30.0));   // confirmacao duplicada
}

void unit_Reserva_id(void) {
    assert(Reserva(9, 1, 1, 1).id() == 9);
}

void unit_Reserva_idOferta(void) {
    assert(Reserva(1, 15, 1, 1).idOferta() == 15);
}

void unit_Reserva_idConsumidor(void) {
    assert(Reserva(1, 1, 3, 1).idConsumidor() == 3);
}

void unit_Reserva_quantidadeReservada(void) {
    assert(Reserva(1, 1, 1, 2).quantidadeReservada() == 2);
}

void unit_Reserva_quantidadeRetirada(void) {
    Reserva r(1, 1, 1, 2);
    assert(r.quantidadeRetirada() == 0);
    r.confirmarRetirada(2, 1.0, 24.0);
    assert(r.quantidadeRetirada() == 2);
}

void unit_Reserva_valorRecebido(void) {
    Reserva r(1, 1, 1, 2);
    assert(quase(r.valorRecebido(), 0.0));
    r.confirmarRetirada(2, 1.0, 24.0);
    assert(quase(r.valorRecebido(), 24.0));
}

void unit_Reserva_pesoAproveitadoKg(void) {
    Reserva r(1, 1, 1, 2);
    assert(quase(r.pesoAproveitadoKg(), 0.0));
    r.confirmarRetirada(2, 1.0, 24.0);
    assert(quase(r.pesoAproveitadoKg(), 1.0));
}

void unit_Reserva_retiradaConfirmada(void) {
    Reserva r(1, 1, 1, 2);
    assert(!r.retiradaConfirmada());
    r.confirmarRetirada(2, 1.0, 24.0);
    assert(r.retiradaConfirmada());
}

void unit_Reserva_situacao(void) {
    Reserva pendente(1, 1, 1, 2), cancelada(2, 1, 1, 2), retirada(3, 1, 1, 2);
    cancelada.cancelar();
    retirada.confirmarRetirada(2, 1.0, 24.0);
    assert(pendente.situacao() == EstadoReserva::Pendente);
    assert(cancelada.situacao() == EstadoReserva::Cancelada);
    assert(retirada.situacao() == EstadoReserva::Retirada);
}

void run_unit_test_Reserva(void) {
    unit_Reserva_constructor();
    unit_Reserva_copyConstructor();
    unit_Reserva_assignmentOperator();
    unit_Reserva_destructor();
    unit_Reserva_cancelar();
    unit_Reserva_confirmarRetirada();
    unit_Reserva_id();
    unit_Reserva_idOferta();
    unit_Reserva_idConsumidor();
    unit_Reserva_quantidadeReservada();
    unit_Reserva_quantidadeRetirada();
    unit_Reserva_valorRecebido();
    unit_Reserva_pesoAproveitadoKg();
    unit_Reserva_retiradaConfirmada();
    unit_Reserva_situacao();
}
