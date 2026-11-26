/** @file unit_ServicoAutenticacao.cpp
 * @brief Testes unitarios da classe ServicoAutenticacao.
 */
#include <assert.h>
#include "unit_ServicoAutenticacao.h"
#include "../../src/services.h"

void unit_ServicoAutenticacao_constructor(void) {
    ServicoAutenticacao semSistema;
    Sessao s;
    assert(!semSistema.entrar("consumidor@savefood.demo", "consumidor123", s));
    assert(!s.ativa());

    SistemaSaveFood sistema;
    ServicoAutenticacao autenticacao(sistema);
    assert(autenticacao.entrar("consumidor@savefood.demo", "consumidor123", s));
}

void unit_ServicoAutenticacao_copyConstructor(void) {
    SistemaSaveFood sistema;
    ServicoAutenticacao original(sistema);
    ServicoAutenticacao copia(original);
    Sessao s;
    assert(copia.entrar("gerente@savefood.demo", "gerente123", s));
    assert(sistema.permitir(s, Perfil::Gerente)); // sessao aberta no mesmo sistema
}

void unit_ServicoAutenticacao_assignmentOperator(void) {
    SistemaSaveFood sistema;
    ServicoAutenticacao original(sistema);
    ServicoAutenticacao atribuido;
    atribuido = original;
    Sessao s;
    assert(atribuido.entrar("gerente@savefood.demo", "gerente123", s));
    assert(sistema.permitir(s, Perfil::Gerente));
}

void unit_ServicoAutenticacao_destructor(void) {
    SistemaSaveFood sistema;
    Sessao s;
    {
        ServicoAutenticacao temporario(sistema);
        assert(temporario.entrar("consumidor@savefood.demo", "consumidor123", s));
    }
    assert(sistema.permitir(s, Perfil::Consumidor)); // sistema e sessao sobrevivem
}

void unit_ServicoAutenticacao_entrar(void) {
    SistemaSaveFood sistema;
    ServicoAutenticacao autenticacao(sistema);
    Sessao s;
    assert(autenticacao.entrar("consumidor@savefood.demo", "consumidor123", s));
    assert(s.autenticada());
    assert(!autenticacao.entrar("consumidor@savefood.demo", "errada", s));
    assert(!s.autenticada()); // falha substitui a sessao por uma inativa
}

void unit_ServicoAutenticacao_sair(void) {
    SistemaSaveFood sistema;
    ServicoAutenticacao autenticacao(sistema);
    Sessao s;
    assert(autenticacao.entrar("gerente@savefood.demo", "gerente123", s));
    autenticacao.sair(s);
    assert(!s.ativa());
    assert(!sistema.permitir(s, Perfil::Gerente));
    autenticacao.sair(s); // sair de novo nao causa erro

    ServicoAutenticacao semSistema;
    Sessao vazia;
    semSistema.sair(vazia); // servico sem sistema ignora a chamada
    assert(!vazia.ativa());
}

void run_unit_test_ServicoAutenticacao(void) {
    unit_ServicoAutenticacao_constructor();
    unit_ServicoAutenticacao_copyConstructor();
    unit_ServicoAutenticacao_assignmentOperator();
    unit_ServicoAutenticacao_destructor();
    unit_ServicoAutenticacao_entrar();
    unit_ServicoAutenticacao_sair();
}
