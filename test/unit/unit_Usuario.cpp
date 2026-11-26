/** @file unit_Usuario.cpp
 * @brief Testes unitarios da classe Usuario.
 */
#include <assert.h>
#include "unit_Usuario.h"
#include "../../src/dominio/model.h"

void unit_Usuario_constructor(void) {
    Usuario padrao;
    assert(padrao.id() == 0);
    assert(padrao.email().empty());
    assert(padrao.perfil() == Perfil::Consumidor);
    assert(padrao.idRestaurante() == 0);

    Usuario gerente(1, "gerente@savefood.demo", "gerente123", Perfil::Gerente, 1);
    assert(gerente.id() == 1);
    assert(gerente.email() == "gerente@savefood.demo");
    assert(gerente.perfil() == Perfil::Gerente);
    assert(gerente.idRestaurante() == 1);
}

void unit_Usuario_copyConstructor(void) {
    Usuario original(10, "teste@savefood.demo", "senha", Perfil::Consumidor, 0);
    Usuario copia(original);
    assert(copia.id() == 10);
    assert(copia.email() == "teste@savefood.demo");
    assert(copia.conferirSenha("senha"));
}

void unit_Usuario_assignmentOperator(void) {
    Usuario original(11, "outro@savefood.demo", "abc", Perfil::Gerente, 2);
    Usuario atribuido;
    atribuido = original;
    assert(atribuido.id() == 11);
    assert(atribuido.perfil() == Perfil::Gerente);
    assert(atribuido.idRestaurante() == 2);
    assert(atribuido.conferirSenha("abc"));
}

void unit_Usuario_destructor(void) {
    Usuario* original = new Usuario(12, "x@savefood.demo", "x", Perfil::Consumidor, 0);
    Usuario copia(*original);
    delete original;
    assert(copia.id() == 12);
    assert(copia.conferirSenha("x"));
}

void unit_Usuario_id(void) {
    Usuario u(7, "a@savefood.demo", "a", Perfil::Consumidor, 0);
    assert(u.id() == 7);
}

void unit_Usuario_email(void) {
    Usuario u(7, "a@savefood.demo", "a", Perfil::Consumidor, 0);
    assert(u.email() == "a@savefood.demo");
}

void unit_Usuario_perfil(void) {
    Usuario consumidor(1, "c@savefood.demo", "c", Perfil::Consumidor, 0);
    Usuario gerente(2, "g@savefood.demo", "g", Perfil::Gerente, 1);
    assert(consumidor.perfil() == Perfil::Consumidor);
    assert(gerente.perfil() == Perfil::Gerente);
}

void unit_Usuario_idRestaurante(void) {
    Usuario consumidor(1, "c@savefood.demo", "c", Perfil::Consumidor, 0);
    Usuario gerente(2, "g@savefood.demo", "g", Perfil::Gerente, 5);
    assert(consumidor.idRestaurante() == 0);
    assert(gerente.idRestaurante() == 5);
}

void unit_Usuario_conferirSenha(void) {
    Usuario u(1, "c@savefood.demo", "correta", Perfil::Consumidor, 0);
    assert(u.conferirSenha("correta"));
    assert(!u.conferirSenha("errada"));
    assert(!u.conferirSenha(""));
    assert(!u.conferirSenha("Correta")); // diferencia maiusculas
}

void run_unit_test_Usuario(void) {
    unit_Usuario_constructor();
    unit_Usuario_copyConstructor();
    unit_Usuario_assignmentOperator();
    unit_Usuario_destructor();
    unit_Usuario_id();
    unit_Usuario_email();
    unit_Usuario_perfil();
    unit_Usuario_idRestaurante();
    unit_Usuario_conferirSenha();
}
