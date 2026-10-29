#include "flow.h"

#include <iomanip>
#include <ostream>

void executarDemonstracao(SistemaSaveFood& sistema, std::ostream& saida) {
    ServicoAutenticacao autenticacao(sistema);
    Sessao gerente;
    Sessao consumidor;
    if (!autenticacao.entrar("gerente@savefood.demo", "gerente123", gerente) ||
        !autenticacao.entrar("consumidor@savefood.demo", "consumidor123", consumidor)) {
        saida << "Falha ao iniciar as sessoes da demonstracao.\n";
        return;
    }

    ServicoOfertas ofertasGerente(sistema, gerente);
    ServicoOfertas ofertasConsumidor(sistema, consumidor);
    DadosOferta dados{"Marmita do dia", 5, 2.5, 12.0, "18h-19h"};
    Oferta publicada;
    if (!ofertasGerente.publicar(dados, publicada)) {
        saida << "Nao foi possivel publicar a oferta.\n";
        return;
    }

    saida << "Oferta publicada: codigo " << publicada.id() << "\n";
    for (const Oferta& oferta : ofertasConsumidor.consultar(FiltroOferta{"marmita"})) {
        saida << "Catalogo: " << oferta.descricao() << " - "
              << oferta.quantidadeDisponivel() << " unidades\n";
    }

    Oferta detalhes;
    if (!ofertasConsumidor.detalhes(publicada.id(), detalhes)) {
        saida << "Nao foi possivel consultar os detalhes da oferta.\n";
        return;
    }
    saida << "Detalhes: " << detalhes.descricao() << " - R$ "
          << detalhes.precoUnitario() << " por unidade\n";

    ServicoReservas reservas(sistema, consumidor);
    Reserva criada;
    if (!reservas.reservar(publicada.id(), 2, criada)) {
        saida << "Nao foi possivel efetuar a reserva.\n";
        return;
    }
    saida << "Reserva criada: codigo " << criada.id() << "\n";

    ServicoRetiradas retiradas(sistema, gerente);
    ConfirmacaoRetirada entrega{2, 24.0};
    if (!retiradas.confirmar(criada.id(), entrega)) {
        saida << "Nao foi possivel confirmar a retirada.\n";
        return;
    }

    ServicoIndicadores indicadores(sistema, gerente);
    ResumoIndicadores resumo = indicadores.obterResumo();
    saida << std::fixed << std::setprecision(2);
    saida << "Alimentos aproveitados: " << resumo.pesoAproveitadoKg << " kg\n";
    saida << "Receita bruta recuperada: R$ " << resumo.receitaBrutaReais << "\n";

    autenticacao.sair(consumidor);
    autenticacao.sair(gerente);
}
