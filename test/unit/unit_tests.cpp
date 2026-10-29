#include "unit_tests.h"

#include "../../src/model.h"
#include "../../src/system.h"

#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>

namespace {
void verificar(bool condicao, const char* mensagem) {
    if (!condicao) {
        throw std::runtime_error(mensagem);
    }
}
}

bool testeCopiaDeOferta() {
    try {
        Oferta original(1, 1, "Marmita", 5, 2.5, 12.0, "18h-19h");
        Oferta copia(original);           // construtor de copia
        Oferta atribuida;                // construtor padrao
        atribuida = original;            // operador de atribuicao
        verificar(copia.reservar(2), "Nao foi possivel alterar a copia");
        verificar(copia.quantidadeDisponivel() == 3, "Copia nao mudou");
        verificar(original.quantidadeDisponivel() == 5, "Copia alterou o original");
        verificar(atribuida.quantidadeDisponivel() == 5, "Atribuicao incorreta");

        Usuario usuario1(10, "teste@savefood.demo", "senha", Perfil::Consumidor, 0);
        Usuario usuario2(usuario1);
        verificar(usuario2.id() == 10 && usuario2.conferirSenha("senha"),
                  "Copia de usuario incorreta");
        Restaurante restaurante(1, "Restaurante A");
        Restaurante restauranteCopia(restaurante);
        Restaurante restauranteAtribuido;
        restauranteAtribuido = restaurante;
        verificar(restauranteCopia.id() == 1 &&
                  restauranteAtribuido.nome() == "Restaurante A",
                  "Forma canonica de Restaurante incorreta");
        std::cout << "[OK] 1. Forma canonica de Oferta e Usuario\n";
        return true;
    } catch (const std::exception& erro) {
        std::cerr << "[FALHOU] Unitario 1: " << erro.what() << '\n';
        return false;
    }
}

bool testeCopiaDeReserva() {
    try {
        Reserva original(1, 20, 3, 4);
        Reserva copia(original);
        Reserva atribuida;
        atribuida = original;
        verificar(copia.confirmarRetirada(2, 30.0), "Confirmacao da copia falhou");
        verificar(!original.retiradaConfirmada(), "Copia alterou original");
        verificar(original.situacao() == EstadoReserva::Pendente &&
                  copia.situacao() == EstadoReserva::Retirada,
                  "Situacao da reserva nao corresponde a retirada");
        verificar(!atribuida.retiradaConfirmada(), "Atribuicao incorreta");
        verificar(!copia.confirmarRetirada(2, 30.0), "Confirmacao duplicada permitida");
        verificar(!original.confirmarRetirada(5, 50.0), "Retirada acima da reserva aceita");
        std::cout << "[OK] 2. Forma canonica e validacao de Reserva\n";
        return true;
    } catch (const std::exception& erro) {
        std::cerr << "[FALHOU] Unitario 2: " << erro.what() << '\n';
        return false;
    }
}

bool testeCopiaDoSistema() {
    try {
        SistemaSaveFood original;
        Sessao gerente = original.entrar("gerente@savefood.demo", "gerente123");
        Sessao consumidor = original.entrar("consumidor@savefood.demo", "consumidor123");
        int oferta = original.publicarOferta(gerente, "Pao", 4, 2.0, 5.0, "17h-18h");
        verificar(oferta > 0, "Falha ao criar oferta original");

        SistemaSaveFood copia(original); // copia os dados, mutex independente
        int idReserva = 0;
        verificar(copia.reservar(consumidor, oferta, 2, idReserva), "Reserva na copia falhou");
        verificar(copia.saldoOferta(gerente, oferta) == 2, "Saldo da copia incorreto");
        verificar(original.saldoOferta(gerente, oferta) == 4, "Copia afetou original");

        SistemaSaveFood atribuida;
        atribuida = original;
        verificar(atribuida.reservar(consumidor, oferta, 3, idReserva),
                  "Reserva na atribuicao falhou");
        verificar(original.saldoOferta(gerente, oferta) == 4,
                  "Atribuicao afetou o original");
        std::cout << "[OK] 3. Construtor de copia e atribuicao do Sistema\n";
        return true;
    } catch (const std::exception& erro) {
        std::cerr << "[FALHOU] Unitario 3: " << erro.what() << '\n';
        return false;
    }
}
