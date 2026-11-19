/** @file flow.cpp
 * @brief Implementacao da demonstracao e tratamento de falhas.
 */
#include "flow.h"

#include <iomanip>
#include <ostream>
#include <stdexcept>

void executarDemonstracao(SistemaSaveFood& sistema, std::ostream& saida) {
    ServicoAutenticacao autenticacao(sistema);
    Sessao gerente;
    Sessao consumidor;
    try {
        if (!autenticacao.entrar("gerente@savefood.demo", "gerente123", gerente) ||
            !autenticacao.entrar("consumidor@savefood.demo", "consumidor123", consumidor)) {
            throw std::runtime_error("Falha ao iniciar as sessoes da demonstracao.");
        }

        ServicoOfertas ofertasGerente(sistema, gerente);
        ServicoOfertas ofertasConsumidor(sistema, consumidor);
        DadosOferta dados{"Marmita do dia", 5, 2.5, 12.0, "18h-19h"};
        Oferta publicada;
        if (!ofertasGerente.publicar(dados, publicada)) {
            throw std::runtime_error("Nao foi possivel publicar a oferta.");
        }

        saida << "Oferta publicada: codigo " << publicada.id() << "\n";
        for (const Oferta& oferta : ofertasConsumidor.consultar(FiltroOferta{"marmita"})) {
            saida << "Catalogo: " << oferta.descricao() << " - "
                  << oferta.quantidadeDisponivel() << " unidades\n";
        }

        Oferta detalhes;
        if (!ofertasConsumidor.detalhes(publicada.id(), detalhes)) {
            throw std::runtime_error("Nao foi possivel consultar os detalhes da oferta.");
        }
        saida << "Detalhes: " << detalhes.descricao() << " - R$ "
              << detalhes.precoUnitario() << " por unidade\n";

        ServicoReservas reservas(sistema, consumidor);
        Reserva criada;
        if (!reservas.reservar(publicada.id(), 2, criada)) {
            throw std::runtime_error("Nao foi possivel efetuar a reserva.");
        }
        saida << "Reserva criada: codigo " << criada.id() << "\n";

        ServicoRetiradas retiradas(sistema, gerente);
        DadosRetirada entrega{2, 1.0, 24.0};
        if (!retiradas.confirmar(criada.id(), entrega)) {
            throw std::runtime_error("Nao foi possivel confirmar a retirada.");
        }

        ServicoIndicadores indicadores(sistema, gerente);
        ResumoIndicadores resumo = indicadores.obterResumo();
        saida << std::fixed << std::setprecision(2);
        saida << "Alimentos aproveitados: " << resumo.pesoAproveitadoKg << " kg\n";
        saida << "Receita bruta recuperada: R$ " << resumo.receitaBrutaReais << "\n";

    } catch (...) {
        // Encerra as sessoes mesmo quando uma etapa da demonstracao falha.
        autenticacao.sair(consumidor);
        autenticacao.sair(gerente);
        throw;
    }
    autenticacao.sair(consumidor);
    autenticacao.sair(gerente);
}
