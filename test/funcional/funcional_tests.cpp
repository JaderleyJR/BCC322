/** @file funcional_tests.cpp
 * @brief Cenarios de aceitacao de SB03, SB04 e SB05.
 */
#include "funcional_tests.h"
#include "services.h"

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

// Consulta o estoque pela API, sem acessar diretamente os dados internos.
int saldo(ServicoOfertas& ofertas, int idOferta) {
    Oferta detalhes;
    verificar(ofertas.detalhes(idOferta, detalhes), "Oferta nao encontrada");
    return detalhes.quantidadeDisponivel();
}
}

// SB03 - Reservar quantidade disponivel (pagina 6 do projeto da API).
bool testeReservarQuantidade() {
    try {
        SistemaSaveFood sistema;
        ServicoAutenticacao autenticacao(sistema);
        Sessao gerente, consumidor;
        verificar(autenticacao.entrar("gerente@savefood.demo", "gerente123", gerente),
                  "Falha no login do gerente");
        verificar(autenticacao.entrar("consumidor@savefood.demo", "consumidor123", consumidor),
                  "Falha no login do consumidor");
        ServicoOfertas ofertas(sistema, gerente);
        ServicoReservas reservas(sistema, consumidor);
        Oferta lote;
        verificar(ofertas.publicar({"Marmita", 5, 2.5, 12.0, "18h-19h"}, lote),
                  "Falha ao preparar a oferta");
        verificar(saldo(ofertas, lote.id()) == 5, "Saldo inicial deveria ser 5");

        // CT-01: reservar 2 de 5 unidades. O saldo deve cair para 3.
        Reserva criada;
        verificar(reservas.reservar(lote.id(), 2, criada), "Reserva valida recusada");
        verificar(criada.id() > 0 && criada.idOferta() == lote.id() &&
                  criada.quantidadeReservada() == 2 &&
                  criada.situacao() == EstadoReserva::Pendente,
                  "Dados da reserva incorretos");
        verificar(saldo(ofertas, lote.id()) == 3, "Saldo deveria ser 3");
        auto lista = reservas.minhasReservas();
        verificar(lista.size() == 1 && lista.front().id() == criada.id() &&
                  lista.front().situacao() == EstadoReserva::Pendente,
                  "Reserva pendente nao registrada");

        // CT-02: preparar saldo 1 e tentar reservar 2 unidades.
        Reserva preparacao, recusada;
        verificar(reservas.reservar(lote.id(), 2, preparacao), "Falha na preparacao do CT-02");
        verificar(saldo(ofertas, lote.id()) == 1, "Saldo deveria ser 1");
        const auto antes = reservas.minhasReservas().size();
        verificar(!reservas.reservar(lote.id(), 2, recusada), "Reserva acima do saldo aceita");
        verificar(saldo(ofertas, lote.id()) == 1 && reservas.minhasReservas().size() == antes &&
                  recusada.id() == 0, "Reserva recusada alterou o saldo ou criou registro");
        std::cout << "[OK] SB03 - CT-01: saldo 5 -> 3; CT-02: saldo insuficiente, permanece 1\n";
        return true;
    } catch (const std::exception& erro) {
        std::cerr << "[FALHOU] SB03: " << erro.what() << '\n';
        return false;
    }
}

// SB04 - Cancelar reserva e restaurar disponibilidade (pagina 7).
bool testeCancelarReserva() {
    try {
        SistemaSaveFood sistema;
        ServicoAutenticacao autenticacao(sistema);
        Sessao gerente, consumidor, outroConsumidor;
        verificar(autenticacao.entrar("gerente@savefood.demo", "gerente123", gerente),
                  "Falha no login do gerente");
        verificar(autenticacao.entrar("consumidor@savefood.demo", "consumidor123", consumidor),
                  "Falha no login do consumidor");
        verificar(autenticacao.entrar("consumidor2@savefood.demo", "consumidor123", outroConsumidor),
                  "Falha no login do segundo consumidor");
        ServicoOfertas ofertas(sistema, gerente);
        ServicoReservas reservas(sistema, consumidor), reservasOutro(sistema, outroConsumidor);
        Oferta lote;
        Reserva criada;
        verificar(ofertas.publicar({"Marmita", 5, 2.5, 12.0, "18h-19h"}, lote) &&
                  reservas.reservar(lote.id(), 2, criada), "Falha ao preparar a reserva");
        verificar(saldo(ofertas, lote.id()) == 3, "Saldo antes do cancelamento deveria ser 3");
        auto lista = reservas.minhasReservas();
        verificar(lista.size() == 1 && lista.front().id() == criada.id() &&
                  lista.front().situacao() == EstadoReserva::Pendente,
                  "Reserva propria pendente nao encontrada");
        verificar(reservasOutro.minhasReservas().empty(), "Reserva de terceiro exposta");
        verificar(!reservasOutro.cancelar(criada.id()) && saldo(ofertas, lote.id()) == 3,
                  "Terceiro conseguiu cancelar a reserva");

        // CT-01: cancelar a propria reserva pendente e restaurar 2 unidades.
        verificar(reservas.cancelar(criada.id()), "Cancelamento valido recusado");
        lista = reservas.minhasReservas();
        verificar(lista.size() == 1 && lista.front().id() == criada.id() &&
                  lista.front().situacao() == EstadoReserva::Cancelada,
                  "Situacao CANCELADA nao registrada");
        verificar(saldo(ofertas, lote.id()) == 5, "Estoque nao retornou a 5");

        // CT-02: repetir o cancelamento, sem uma segunda devolucao.
        verificar(!reservas.cancelar(criada.id()), "Cancelamento repetido aceito");
        lista = reservas.minhasReservas();
        verificar(saldo(ofertas, lote.id()) == 5 && lista.size() == 1 &&
                  lista.front().situacao() == EstadoReserva::Cancelada,
                  "Cancelamento repetido alterou os dados");
        std::cout << "[OK] SB04 - CT-01: saldo 3 -> 5; CT-02: cancelamento repetido recusado\n";
        return true;
    } catch (const std::exception& erro) {
        std::cerr << "[FALHOU] SB04: " << erro.what() << '\n';
        return false;
    }
}

// SB05 - Consultar reservas e confirmar retirada (pagina 8).
bool testeConfirmarRetirada() {
    try {
        SistemaSaveFood sistema;
        ServicoAutenticacao autenticacao(sistema);
        Sessao gerenteA, gerenteB, consumidor;
        verificar(autenticacao.entrar("gerente@savefood.demo", "gerente123", gerenteA),
                  "Falha no login do gerente A");
        verificar(autenticacao.entrar("gerente2@savefood.demo", "gerente123", gerenteB),
                  "Falha no login do gerente B");
        verificar(autenticacao.entrar("consumidor@savefood.demo", "consumidor123", consumidor),
                  "Falha no login do consumidor");
        ServicoOfertas ofertasA(sistema, gerenteA), ofertasB(sistema, gerenteB);
        ServicoReservas reservas(sistema, consumidor);
        ServicoRetiradas retiradasA(sistema, gerenteA), retiradasB(sistema, gerenteB);
        Oferta loteA, loteB;
        Reserva reservaA, reservaB;
        // Peso anunciado diferente do peso efetivo para testar o dado informado.
        verificar(ofertasA.publicar({"Marmita A", 5, 3.0, 12.0, "18h-19h"}, loteA) &&
                  ofertasB.publicar({"Marmita B", 5, 2.5, 12.0, "18h-19h"}, loteB) &&
                  reservas.reservar(loteA.id(), 2, reservaA) &&
                  reservas.reservar(loteB.id(), 2, reservaB), "Falha ao preparar as reservas");
        auto listaA = retiradasA.listar();
        auto listaB = retiradasB.listar();
        verificar(listaA.size() == 1 && listaA.front().id() == reservaA.id() &&
                  listaA.front().situacao() == EstadoReserva::Pendente &&
                  listaB.size() == 1 && listaB.front().id() == reservaB.id() &&
                  listaB.front().situacao() == EstadoReserva::Pendente,
                  "Listagem inclui reservas incorretas");

        // CT-01: confirmar 2 unidades, 1 kg e R$ 24 no restaurante A.
        DadosRetirada dados{2, 1.0, 24.0};
        verificar(retiradasA.confirmar(reservaA.id(), dados), "Retirada valida recusada");
        listaA = retiradasA.listar();
        verificar(listaA.size() == 1 && listaA.front().id() == reservaA.id() &&
                  listaA.front().situacao() == EstadoReserva::Retirada &&
                  listaA.front().quantidadeRetirada() == 2 &&
                  std::fabs(listaA.front().pesoAproveitadoKg() - 1.0) < 1e-6 &&
                  std::fabs(listaA.front().valorRecebido() - 24.0) < 1e-6,
                  "Dados efetivos da retirada nao registrados");
        verificar(!retiradasA.confirmar(reservaA.id(), dados), "Confirmacao duplicada aceita");
        verificar(!reservas.cancelar(reservaA.id()), "Reserva retirada foi cancelada");
        verificar(saldo(ofertasA, loteA.id()) == 3, "Retirada alterou indevidamente o saldo");

        // CT-02: gerente A tenta confirmar reserva do restaurante B.
        verificar(!retiradasA.confirmar(reservaB.id(), dados), "Retirada de outro restaurante aceita");
        listaB = retiradasB.listar();
        verificar(listaB.size() == 1 && listaB.front().id() == reservaB.id() &&
                  listaB.front().situacao() == EstadoReserva::Pendente &&
                  listaB.front().quantidadeRetirada() == 0 &&
                  listaB.front().pesoAproveitadoKg() == 0.0 && listaB.front().valorRecebido() == 0.0 &&
                  saldo(ofertasB, loteB.id()) == 3, "Tentativa indevida alterou a reserva B");
        // A entrega confirmada fica disponivel para os indicadores (efeito do SB05).
        const auto resumoA = ServicoIndicadores(sistema, gerenteA).obterResumo();
        const auto resumoB = ServicoIndicadores(sistema, gerenteB).obterResumo();
        verificar(std::fabs(resumoA.pesoAproveitadoKg - 1.0) < 1e-6 &&
                  std::fabs(resumoA.receitaBrutaReais - 24.0) < 1e-6 &&
                  resumoB.pesoAproveitadoKg == 0.0 && resumoB.receitaBrutaReais == 0.0,
                  "Indicadores nao refletem a retirada confirmada uma unica vez");
        std::cout << "[OK] SB05 - CT-01: retirada de 2 unidades, 1 kg e R$ 24; CT-02: acesso negado\n";
        return true;
    } catch (const std::exception& erro) {
        std::cerr << "[FALHOU] SB05: " << erro.what() << '\n';
        return false;
    }
}
