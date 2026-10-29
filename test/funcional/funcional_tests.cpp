#include "funcional_tests.h"

#include "../../src/model.h"
#include "../../src/services.h"
#include "../../src/system.h"

#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>

namespace {
void verificar(bool condicao, const char* mensagem) {
    if (!condicao) {
        throw std::runtime_error(mensagem);
    }
}

bool quaseIgual(double a, double b) {
    return std::fabs(a - b) < 0.000001;
}
}

// Fluxo 1: gerente publica, consumidor encontra o lote e acessos sao separados.
bool testePublicacaoEConsulta() {
    try {
        SistemaSaveFood sistema;
        ServicoAutenticacao autenticacao(sistema);
        Sessao gerente;
        Sessao consumidor;
        Sessao senhaErrada;

        verificar(autenticacao.entrar("gerente@savefood.demo", "gerente123", gerente) &&
                  autenticacao.entrar("consumidor@savefood.demo", "consumidor123", consumidor),
                  "Falha no login valido");
        verificar(!autenticacao.entrar("consumidor@savefood.demo", "errada", senhaErrada),
                  "Senha incorreta foi aceita");

        ServicoAcesso acesso(sistema);
        verificar(acesso.permitir(gerente, Papel::Gerente, 1),
                  "Gerente nao recebeu permissao do restaurante");
        verificar(!acesso.permitir(gerente, Papel::Gerente, 2) &&
                  !acesso.permitir(consumidor, Papel::Gerente),
                  "Controle de acesso permitiu perfil ou restaurante incorreto");

        ServicoOfertas ofertasGerente(sistema, gerente);
        ServicoOfertas ofertasConsumidor(sistema, consumidor);
        Oferta publicada;
        DadosOferta dados{"Marmita caseira", 5, 2.5, 12.0, "18h-19h"};
        verificar(ofertasGerente.publicar(dados, publicada),
                  "Gerente nao conseguiu publicar");
        Oferta naoPermitida;
        verificar(!ofertasConsumidor.publicar(
                      DadosOferta{"Nao permitido", 2, 1.0, 9.0, "18h"}, naoPermitida),
                  "Consumidor conseguiu publicar oferta");
        verificar(!ofertasGerente.publicar(
                      DadosOferta{"Incompleta", 0, 1.0, 9.0, "18h"}, naoPermitida),
                  "Oferta invalida foi aceita");

        Colecao<Oferta> catalogo = ofertasConsumidor.consultar(FiltroOferta{"marmita"});
        verificar(catalogo.size() == 1, "Catalogo nao retornou a oferta correta");
        verificar(catalogo.front().quantidadeDisponivel() == 5,
                  "Quantidade inicial incorreta");
        Oferta detalhes;
        verificar(ofertasConsumidor.detalhes(publicada.id(), detalhes) &&
                  detalhes.descricao() == "Marmita caseira" &&
                  detalhes.precoUnitario() == 12.0,
                  "Detalhes da oferta nao correspondem ao lote publicado");
        verificar(!ofertasConsumidor.detalhes(9999, detalhes),
                  "Consulta por ID inexistente foi aceita");
        Sessao gerente2;
        verificar(autenticacao.entrar("gerente2@savefood.demo", "gerente123", gerente2),
                  "Falha no login do segundo gerente");
        ServicoOfertas ofertasOutroRestaurante(sistema, gerente2);
        verificar(!ofertasOutroRestaurante.detalhes(publicada.id(), detalhes),
                  "Gerente de outro restaurante consultou detalhes protegidos");
        autenticacao.sair(consumidor);
        verificar(ofertasConsumidor.consultar(FiltroOferta{""}).empty() &&
                  !ofertasConsumidor.detalhes(publicada.id(), detalhes),
                  "Sessao encerrada ainda acessa o catalogo protegido");
        std::cout << "[OK] 1. Publicacao, consulta e controle de acesso\n";
        return true;
    } catch (const std::exception& erro) {
        std::cerr << "[FALHOU] Teste 1: " << erro.what() << '\n';
        return false;
    }
}

// Fluxo 2: duas pessoas tentam reservar o mesmo saldo ao mesmo tempo.
bool testeReservasConcorrentes() {
    try {
        SistemaSaveFood sistema;
        ServicoAutenticacao autenticacao(sistema);
        Sessao gerente;
        Sessao consumidor1;
        Sessao consumidor2;
        verificar(autenticacao.entrar("gerente@savefood.demo", "gerente123", gerente) &&
                  autenticacao.entrar("consumidor@savefood.demo", "consumidor123", consumidor1) &&
                  autenticacao.entrar("consumidor2@savefood.demo", "consumidor123", consumidor2),
                  "Falha ao preparar as sessoes");
        ServicoOfertas ofertas(sistema, gerente);
        Oferta publicada;
        verificar(ofertas.publicar(DadosOferta{"Pao do dia", 4, 2.0, 5.0, "17h-18h"},
                                 publicada), "Oferta da concorrencia nao foi criada");

        bool resultado1 = false;
        bool resultado2 = false;
        Reserva reserva1;
        Reserva reserva2;
        ServicoReservas reservas1(sistema, consumidor1);
        ServicoReservas reservas2(sistema, consumidor2);
        std::thread primeira([&] { resultado1 = reservas1.reservar(publicada.id(), 3, reserva1); });
        std::thread segunda([&] { resultado2 = reservas2.reservar(publicada.id(), 3, reserva2); });
        primeira.join();
        segunda.join();

        verificar(static_cast<int>(resultado1) + static_cast<int>(resultado2) == 1,
                  "Duas reservas excederam o saldo ou ambas falharam");
        verificar(sistema.saldoOferta(gerente, publicada.id()) == 1,
                  "Saldo incorreto apos reservas concorrentes");
        verificar((resultado1 ? reserva1.id() : reserva2.id()) > 0,
                  "Reserva aprovada nao retornou os dados ao consumidor");
        ServicoRetiradas retiradas(sistema, gerente);
        verificar(retiradas.listar().size() == 1,
                  "Apenas uma reserva deveria ter sido gravada");

        Sessao gerente2;
        autenticacao.entrar("gerente2@savefood.demo", "gerente123", gerente2);
        ServicoRetiradas retiradasOutro(sistema, gerente2);
        verificar(retiradasOutro.listar().empty(),
                  "Gerente de outro restaurante visualizou reservas");
        std::cout << "[OK] 2. Reserva simultanea sem ultrapassar quantidade\n";
        return true;
    } catch (const std::exception& erro) {
        std::cerr << "[FALHOU] Teste 2: " << erro.what() << '\n';
        return false;
    }
}

// Fluxo 3: retirada confirmada soma os indicadores; pendencias nao somam.
bool testeRetiradaEIndicadores() {
    try {
        SistemaSaveFood sistema;
        ServicoAutenticacao autenticacao(sistema);
        Sessao gerente;
        Sessao gerente2;
        Sessao consumidor;
        verificar(autenticacao.entrar("gerente@savefood.demo", "gerente123", gerente) &&
                  autenticacao.entrar("gerente2@savefood.demo", "gerente123", gerente2) &&
                  autenticacao.entrar("consumidor@savefood.demo", "consumidor123", consumidor),
                  "Falha ao preparar as sessoes");
        ServicoOfertas ofertas(sistema, gerente);
        Oferta publicada;
        verificar(ofertas.publicar(DadosOferta{"Almoco", 6, 3.0, 12.0, "18h-19h"},
                                 publicada), "Falha ao publicar almoco");

        ServicoReservas reservas(sistema, consumidor);
        Reserva retirada;
        Reserva pendente;
        verificar(reservas.reservar(publicada.id(), 2, retirada), "Primeira reserva falhou");
        verificar(reservas.reservar(publicada.id(), 1, pendente), "Reserva pendente falhou");
        verificar(pendente.situacao() == EstadoReserva::Pendente,
                  "Reserva nova nao foi marcada como pendente");

        ServicoIndicadores indicadores(sistema, gerente);
        ServicoIndicadores indicadoresOutro(sistema, gerente2);
        ResumoIndicadores antes = indicadores.obterResumo();
        verificar(quaseIgual(antes.pesoAproveitadoKg, 0.0) &&
                      quaseIgual(antes.receitaBrutaReais, 0.0),
                  "Reservas pendentes alteraram o resumo");
        ServicoRetiradas retiradas(sistema, gerente);
        ServicoRetiradas retiradasOutro(sistema, gerente2);
        ConfirmacaoRetirada entrega{2, 24.0};
        verificar(!retiradasOutro.confirmar(retirada.id(), entrega),
                  "Outro gerente confirmou a retirada");
        verificar(retiradas.confirmar(retirada.id(), entrega),
                  "Gerente nao confirmou a retirada");
        verificar(!retiradas.confirmar(retirada.id(), entrega),
                  "A retirada foi confirmada duas vezes");

        ResumoIndicadores depois = indicadores.obterResumo();
        verificar(quaseIgual(depois.pesoAproveitadoKg, 1.0) &&
                      quaseIgual(depois.receitaBrutaReais, 24.0),
                  "Indicadores nao correspondem a retirada confirmada");
        ResumoIndicadores outro = indicadoresOutro.obterResumo();
        verificar(quaseIgual(outro.pesoAproveitadoKg, 0.0) &&
                      quaseIgual(outro.receitaBrutaReais, 0.0),
                  "Indicadores de outro restaurante foram expostos");
        std::cout << "[OK] 3. Confirmacao de retirada e indicadores\n";
        return true;
    } catch (const std::exception& erro) {
        std::cerr << "[FALHOU] Teste 3: " << erro.what() << '\n';
        return false;
    }
}
