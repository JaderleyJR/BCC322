/** @file flow.cpp
 * @brief Implementacao da demonstracao e tratamento de falhas (camada de apresentacao).
 */
#include "flow.h"

#include <iomanip>
#include <ostream>
#include <stdexcept>

void executarDemonstracao(ISistemaSaveFood& sistema, std::ostream& saida) {
    // Todos os servicos sao obtidos pela fabrica e usados pelas interfaces.
    std::unique_ptr<IServicoAutenticacao> autenticacao = FabricaServicos::criarAutenticacao(sistema);
    Sessao gerente;
    Sessao consumidor;
    try {
        if (!autenticacao->entrar("gerente@savefood.demo", "gerente123", gerente) ||
            !autenticacao->entrar("consumidor@savefood.demo", "consumidor123", consumidor)) {
            throw std::runtime_error("Falha ao iniciar as sessoes da demonstracao.");
        }

        auto ofertasGerente = FabricaServicos::criarOfertas(sistema, gerente);
        auto ofertasConsumidor = FabricaServicos::criarOfertas(sistema, consumidor);
        DadosOferta dados{"Marmita do dia", 5, 2.5, 12.0, "18h-19h"};
        Oferta publicada;
        if (!ofertasGerente->publicar(dados, publicada)) {
            throw std::runtime_error("Nao foi possivel publicar a oferta.");
        }

        saida << "Oferta publicada: codigo " << publicada.id() << "\n";
        for (const Oferta& oferta : ofertasConsumidor->consultar(FiltroOferta{"marmita"})) {
            saida << "Catalogo: " << oferta.descricao() << " - "
                  << oferta.quantidadeDisponivel() << " unidades\n";
        }

        Oferta detalhes;
        if (!ofertasConsumidor->detalhes(publicada.id(), detalhes)) {
            throw std::runtime_error("Nao foi possivel consultar os detalhes da oferta.");
        }
        saida << "Detalhes: " << detalhes.descricao() << " - R$ "
              << detalhes.precoUnitario() << " por unidade\n";

        auto reservas = FabricaServicos::criarReservas(sistema, consumidor);
        Reserva criada;
        if (!reservas->reservar(publicada.id(), 2, criada)) {
            throw std::runtime_error("Nao foi possivel efetuar a reserva.");
        }
        saida << "Reserva criada: codigo " << criada.id() << "\n";

        auto retiradas = FabricaServicos::criarRetiradas(sistema, gerente);
        DadosRetirada entrega{2, 1.0, 24.0};
        if (!retiradas->confirmar(criada.id(), entrega)) {
            throw std::runtime_error("Nao foi possivel confirmar a retirada.");
        }

        auto indicadores = FabricaServicos::criarIndicadores(sistema, gerente);
        ResumoIndicadores resumo = indicadores->obterResumo();
        saida << std::fixed << std::setprecision(2);
        saida << "Alimentos aproveitados: " << resumo.pesoAproveitadoKg << " kg\n";
        saida << "Receita bruta recuperada: R$ " << resumo.receitaBrutaReais << "\n";

    } catch (...) {
        // Encerra as sessoes mesmo quando uma etapa da demonstracao falha.
        autenticacao->sair(consumidor);
        autenticacao->sair(gerente);
        throw;
    }
    autenticacao->sair(consumidor);
    autenticacao->sair(gerente);
}
