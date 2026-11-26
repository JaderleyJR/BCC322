/** @file unit_Restaurante.cpp
 * @brief Testes unitarios da classe Restaurante.
 */
#include <assert.h>
#include "unit_Restaurante.h"
#include "../../src/model.h"

void unit_Restaurante_constructor(void) {
    Restaurante padrao;
    assert(padrao.id() == 0);
    assert(padrao.nome().empty());

    Restaurante r(1, "Restaurante A");
    assert(r.id() == 1);
    assert(r.nome() == "Restaurante A");
}

void unit_Restaurante_copyConstructor(void) {
    Restaurante original(2, "Restaurante B");
    Restaurante copia(original);
    assert(copia.id() == 2);
    assert(copia.nome() == "Restaurante B");
}

void unit_Restaurante_assignmentOperator(void) {
    Restaurante original(3, "Restaurante C");
    Restaurante atribuido;
    atribuido = original;
    assert(atribuido.id() == 3);
    assert(atribuido.nome() == "Restaurante C");

    atribuido = atribuido; // autoatribuicao preserva os dados
    assert(atribuido.id() == 3);
}

void unit_Restaurante_destructor(void) {
    Restaurante* original = new Restaurante(4, "Restaurante D");
    Restaurante copia(*original);
    delete original;
    assert(copia.id() == 4);
    assert(copia.nome() == "Restaurante D");
}

void unit_Restaurante_id(void) {
    Restaurante r(10, "Qualquer");
    assert(r.id() == 10);
}

void unit_Restaurante_nome(void) {
    Restaurante r(1, "Cantina da UFOP");
    assert(r.nome() == "Cantina da UFOP");
}

void run_unit_test_Restaurante(void) {
    unit_Restaurante_constructor();
    unit_Restaurante_copyConstructor();
    unit_Restaurante_assignmentOperator();
    unit_Restaurante_destructor();
    unit_Restaurante_id();
    unit_Restaurante_nome();
}
