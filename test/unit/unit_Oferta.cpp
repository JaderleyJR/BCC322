/** @file unit_Oferta.cpp
 * @brief Testes unitarios da classe Oferta.
 */
#include <assert.h>
#include <cmath>
#include <limits>
#include "unit_Oferta.h"
#include "../../src/dominio/model.h"

namespace {
/** @brief Oferta valida usada como base: 5 marmitas, 2,5 kg, R$ 12, 18h-19h. */
Oferta ofertaPadrao() {
    return Oferta(1, 1, "Marmita", 5, 2.5, 12.0, "18h-19h");
}
bool quase(double a, double b) { return std::fabs(a - b) < 1e-9; }
}

void unit_Oferta_constructor(void) {
    Oferta padrao;
    assert(padrao.id() == 0);
    assert(padrao.quantidadeDisponivel() == 0);
    assert(!padrao.ativa());
    assert(!padrao.valida());

    Oferta o = ofertaPadrao();
    assert(o.id() == 1 && o.idRestaurante() == 1);
    assert(o.quantidadeTotal() == 5 && o.quantidadeDisponivel() == 5);
    assert(o.ativa() && o.valida());
}

void unit_Oferta_copyConstructor(void) {
    Oferta original = ofertaPadrao();
    Oferta copia(original);
    assert(copia.quantidadeDisponivel() == 5);
    assert(copia.reservar(2));
    assert(copia.quantidadeDisponivel() == 3);
    assert(original.quantidadeDisponivel() == 5); // original intacto
}

void unit_Oferta_assignmentOperator(void) {
    Oferta original = ofertaPadrao();
    Oferta atribuida;
    atribuida = original;
    assert(atribuida.id() == 1 && atribuida.descricao() == "Marmita");
    assert(atribuida.reservar(1));
    assert(original.quantidadeDisponivel() == 5);
}

void unit_Oferta_destructor(void) {
    Oferta* original = new Oferta(ofertaPadrao());
    Oferta copia(*original);
    delete original;
    assert(copia.valida());
    assert(copia.quantidadeDisponivel() == 5);
}

void unit_Oferta_valida(void) {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    assert(ofertaPadrao().valida());
    assert(!Oferta(0, 1, "Marmita", 5, 2.5, 12.0, "18h-19h").valida());  // id
    assert(!Oferta(1, 0, "Marmita", 5, 2.5, 12.0, "18h-19h").valida());  // restaurante
    assert(!Oferta(1, 1, "   ", 5, 2.5, 12.0, "18h-19h").valida());      // descricao em branco
    assert(!Oferta(1, 1, "Marmita", 0, 2.5, 12.0, "18h-19h").valida());  // quantidade
    assert(!Oferta(1, 1, "Marmita", 5, 0.0, 12.0, "18h-19h").valida());  // peso
    assert(!Oferta(1, 1, "Marmita", 5, nan, 12.0, "18h-19h").valida());  // peso NaN
    assert(!Oferta(1, 1, "Marmita", 5, 2.5, -1.0, "18h-19h").valida());  // preco
    assert(!Oferta(1, 1, "Marmita", 5, 2.5, 12.0, " \t").valida());      // janela em branco
}

void unit_Oferta_reservar(void) {
    Oferta o = ofertaPadrao();
    assert(o.reservar(2) && o.quantidadeDisponivel() == 3);
    assert(!o.reservar(0));
    assert(!o.reservar(-1));
    assert(!o.reservar(4));            // acima do saldo
    assert(o.quantidadeDisponivel() == 3);
    assert(o.reservar(3) && o.quantidadeDisponivel() == 0);
    assert(!o.reservar(1));            // esgotada

    Oferta invalida(1, 1, "", 5, 2.5, 12.0, "18h-19h");
    assert(!invalida.reservar(1));
}

void unit_Oferta_restaurarDisponibilidade(void) {
    Oferta o = ofertaPadrao();
    assert(!o.restaurarDisponibilidade(1)); // nada reservado ainda
    assert(o.reservar(3));
    assert(!o.restaurarDisponibilidade(4)); // mais do que foi reservado
    assert(!o.restaurarDisponibilidade(0));
    assert(o.restaurarDisponibilidade(3));
    assert(o.quantidadeDisponivel() == 5);
    assert(!o.restaurarDisponibilidade(1)); // nao passa do total
}

void unit_Oferta_id(void) {
    assert(Oferta(42, 1, "Pao", 1, 0.1, 1.0, "8h").id() == 42);
}

void unit_Oferta_idRestaurante(void) {
    assert(Oferta(1, 7, "Pao", 1, 0.1, 1.0, "8h").idRestaurante() == 7);
}

void unit_Oferta_descricao(void) {
    assert(ofertaPadrao().descricao() == "Marmita");
}

void unit_Oferta_quantidadeTotal(void) {
    Oferta o = ofertaPadrao();
    assert(o.quantidadeTotal() == 5);
    o.reservar(2);
    assert(o.quantidadeTotal() == 5); // reserva nao altera o total
}

void unit_Oferta_quantidadeDisponivel(void) {
    Oferta o = ofertaPadrao();
    assert(o.quantidadeDisponivel() == 5);
    o.reservar(2);
    assert(o.quantidadeDisponivel() == 3);
}

void unit_Oferta_pesoPorUnidadeKg(void) {
    assert(quase(ofertaPadrao().pesoPorUnidadeKg(), 0.5)); // 2,5 kg / 5
    assert(quase(Oferta().pesoPorUnidadeKg(), 0.0));       // sem divisao por zero
}

void unit_Oferta_precoUnitario(void) {
    assert(quase(ofertaPadrao().precoUnitario(), 12.0));
}

void unit_Oferta_janelaRetirada(void) {
    assert(ofertaPadrao().janelaRetirada() == "18h-19h");
}

void unit_Oferta_ativa(void) {
    Oferta o = ofertaPadrao();
    assert(o.ativa());
    o.reservar(5);
    assert(o.ativa());       // continua ativa mesmo esgotada
    assert(!Oferta().ativa());
}

void unit_Oferta_estaAtiva(void) {
    Oferta o = ofertaPadrao();
    assert(o.estaAtiva());
    o.reservar(5);
    assert(!o.estaAtiva());  // esgotada nao e reservavel
    o.restaurarDisponibilidade(1);
    assert(o.estaAtiva());
}

void run_unit_test_Oferta(void) {
    unit_Oferta_constructor();
    unit_Oferta_copyConstructor();
    unit_Oferta_assignmentOperator();
    unit_Oferta_destructor();
    unit_Oferta_valida();
    unit_Oferta_reservar();
    unit_Oferta_restaurarDisponibilidade();
    unit_Oferta_id();
    unit_Oferta_idRestaurante();
    unit_Oferta_descricao();
    unit_Oferta_quantidadeTotal();
    unit_Oferta_quantidadeDisponivel();
    unit_Oferta_pesoPorUnidadeKg();
    unit_Oferta_precoUnitario();
    unit_Oferta_janelaRetirada();
    unit_Oferta_ativa();
    unit_Oferta_estaAtiva();
}
