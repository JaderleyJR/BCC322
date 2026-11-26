/** @file unit_ResumoIndicadores.cpp
 * @brief Testes unitarios de ResumoIndicadores.
 */
#include <assert.h>
#include "unit_ResumoIndicadores.h"
#include "../../src/dominio/model.h"

void unit_ResumoIndicadores_constructor(void) {
    ResumoIndicadores r;
    assert(r.pesoAproveitadoKg == 0.0);
    assert(r.receitaBrutaReais == 0.0);
}

void unit_ResumoIndicadores_copyConstructor(void) {
    ResumoIndicadores original;
    original.pesoAproveitadoKg = 1.5;
    original.receitaBrutaReais = 36.0;
    ResumoIndicadores copia(original);
    assert(copia.pesoAproveitadoKg == 1.5);
    assert(copia.receitaBrutaReais == 36.0);
}

void unit_ResumoIndicadores_assignmentOperator(void) {
    ResumoIndicadores original;
    original.pesoAproveitadoKg = 2.0;
    original.receitaBrutaReais = 48.0;
    ResumoIndicadores atribuido;
    atribuido = original;
    assert(atribuido.pesoAproveitadoKg == 2.0);
    assert(atribuido.receitaBrutaReais == 48.0);
}

void unit_ResumoIndicadores_destructor(void) {
    ResumoIndicadores* original = new ResumoIndicadores();
    original->receitaBrutaReais = 10.0;
    ResumoIndicadores copia(*original);
    delete original;
    assert(copia.receitaBrutaReais == 10.0);
}

void run_unit_test_ResumoIndicadores(void) {
    unit_ResumoIndicadores_constructor();
    unit_ResumoIndicadores_copyConstructor();
    unit_ResumoIndicadores_assignmentOperator();
    unit_ResumoIndicadores_destructor();
}
