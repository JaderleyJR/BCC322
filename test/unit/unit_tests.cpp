/** @file unit_tests.cpp
 * @brief Testes unitarios das funcoes de escopo global.
 *
 * A unica funcao global do produto e executarDemonstracao(). A funcao
 * paraMinusculas() de system.cpp fica em namespace anonimo (nao e visivel
 * fora daquele arquivo) e e testada indiretamente pela pesquisa sem
 * diferenciar maiusculas em unit_SistemaSaveFood_consultarOfertas().
 */
#include <assert.h>
#include <sstream>
#include <string>
#include "unit_tests.h"
#include "../../src/apresentacao/flow.h"
#include "../../src/nucleo/system.h"  // teste de caixa-branca: usa a classe concreta

namespace {
bool contem(const std::string& texto, const std::string& trecho) {
    return texto.find(trecho) != std::string::npos;
}
}

void unit_executarDemonstracao(void) {
    SistemaSaveFood sistema;
    std::ostringstream saida;
    executarDemonstracao(sistema, saida);
    const std::string texto = saida.str();
    assert(contem(texto, "Oferta publicada: codigo 1"));
    assert(contem(texto, "Catalogo: Marmita do dia - 5 unidades"));
    assert(contem(texto, "Reserva criada: codigo 1"));
    assert(contem(texto, "Alimentos aproveitados: 1.00 kg"));
    assert(contem(texto, "Receita bruta recuperada: R$ 24.00"));

    // A demonstracao encerra as sessoes; uma segunda execucao no mesmo
    // sistema funciona e os indicadores acumulam as duas retiradas.
    std::ostringstream segunda;
    executarDemonstracao(sistema, segunda);
    assert(contem(segunda.str(), "Oferta publicada: codigo 2"));
    assert(contem(segunda.str(), "Alimentos aproveitados: 2.00 kg"));
    assert(contem(segunda.str(), "Receita bruta recuperada: R$ 48.00"));
}

void run_unit_test_globals(void) {
    unit_executarDemonstracao();
}
