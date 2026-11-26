/** @file unit_ServicoAcesso.cpp
 * @brief Testes unitarios da classe ServicoAcesso.
 */
#include <assert.h>
#include "unit_ServicoAcesso.h"
#include "../../src/services.h"

void unit_ServicoAcesso_constructor(void) {
    SistemaSaveFood sistema;
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    ServicoAcesso semSistema;
    assert(!semSistema.permitir(g, Papel::Gerente));
    ServicoAcesso acesso(sistema);
    assert(acesso.permitir(g, Papel::Gerente));
}

void unit_ServicoAcesso_copyConstructor(void) {
    SistemaSaveFood sistema;
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    ServicoAcesso original(sistema);
    ServicoAcesso copia(original);
    assert(copia.permitir(g, Papel::Gerente, 1));
}

void unit_ServicoAcesso_assignmentOperator(void) {
    SistemaSaveFood sistema;
    Sessao c = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    ServicoAcesso atribuido;
    atribuido = ServicoAcesso(sistema);
    assert(atribuido.permitir(c, Papel::Consumidor));
}

void unit_ServicoAcesso_destructor(void) {
    SistemaSaveFood sistema;
    Sessao c = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    ServicoAcesso* acesso = new ServicoAcesso(sistema);
    delete acesso;
    assert(sistema.permitir(c, Perfil::Consumidor));
}

void unit_ServicoAcesso_permitir(void) {
    SistemaSaveFood sistema;
    Sessao g = sistema.entrar("gerente@savefood.demo", "gerente123");
    Sessao c = sistema.entrar("consumidor@savefood.demo", "consumidor123");
    ServicoAcesso acesso(sistema);
    assert(acesso.permitir(g, Papel::Gerente, 1));
    assert(!acesso.permitir(g, Papel::Gerente, 2));  // restaurante de outro gerente
    assert(!acesso.permitir(c, Papel::Gerente));
    assert(acesso.permitir(c, Papel::Consumidor));
    sistema.sair(c);
    assert(!acesso.permitir(c, Papel::Consumidor));  // sessao encerrada
}

void run_unit_test_ServicoAcesso(void) {
    unit_ServicoAcesso_constructor();
    unit_ServicoAcesso_copyConstructor();
    unit_ServicoAcesso_assignmentOperator();
    unit_ServicoAcesso_destructor();
    unit_ServicoAcesso_permitir();
}
