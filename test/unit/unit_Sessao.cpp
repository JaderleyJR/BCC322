/** @file unit_Sessao.cpp
 * @brief Testes unitarios da classe Sessao.
 *
 * O construtor com parametros e privado (somente o SistemaSaveFood cria
 * sessoes validas), por isso ele e exercitado por meio de entrar().
 */
#include <assert.h>
#include "unit_Sessao.h"
#include "../../src/nucleo/system.h"

void unit_Sessao_constructor(void) {
    Sessao padrao;
    assert(!padrao.ativa());
    assert(!padrao.autenticada());

    SistemaSaveFood sistema;
    Sessao valida = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    assert(valida.ativa());
}

void unit_Sessao_copyConstructor(void) {
    SistemaSaveFood sistema;
    Sessao original = sistema.entrar("gerente@savefood.demo", "gerente123");
    Sessao copia(original);
    assert(copia.ativa());
    // A copia identifica o mesmo usuario perante o sistema.
    assert(sistema.permitir(copia, Perfil::Gerente, 1));
}

void unit_Sessao_assignmentOperator(void) {
    SistemaSaveFood sistema;
    Sessao original = sistema.entrar("gerente@savefood.demo", "gerente123");
    Sessao atribuida;
    assert(!atribuida.ativa());
    atribuida = original;
    assert(atribuida.ativa());
    assert(sistema.permitir(atribuida, Perfil::Gerente));
}

void unit_Sessao_destructor(void) {
    SistemaSaveFood sistema;
    Sessao original = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    Sessao* copia = new Sessao(original);
    delete copia;
    assert(original.ativa());
    assert(sistema.permitir(original, Perfil::Consumidor));
}

void unit_Sessao_ativa(void) {
    SistemaSaveFood sistema;
    Sessao s = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    assert(s.ativa());
    assert(sistema.sair(s));
    assert(!s.ativa());
    Sessao recusada = sistema.entrar("consumidor@savefood.demo", "senhaErrada");
    assert(!recusada.ativa());
}

void unit_Sessao_autenticada(void) {
    SistemaSaveFood sistema;
    Sessao s = sistema.entrar("gerente@savefood.demo", "gerente123");
    assert(s.autenticada() == s.ativa());
    assert(s.autenticada());
    sistema.sair(s);
    assert(!s.autenticada());
}

void run_unit_test_Sessao(void) {
    unit_Sessao_constructor();
    unit_Sessao_copyConstructor();
    unit_Sessao_assignmentOperator();
    unit_Sessao_destructor();
    unit_Sessao_ativa();
    unit_Sessao_autenticada();
}
