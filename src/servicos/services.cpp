/** @file services.cpp
 * @brief Implementacao das interfaces de servico.
 */
#include "services.h"

ServicoAutenticacao::ServicoAutenticacao(ISistemaSaveFood& sistema)
    : sistema_(&sistema) {}

bool ServicoAutenticacao::entrar(const std::string& email,
                                const std::string& senha, Sessao& sessao) {
    if (!sistema_) {
        sessao = Sessao();
        return false;
    }
    sessao = sistema_->entrar(email, senha);
    return sessao.autenticada();
}

void ServicoAutenticacao::sair(Sessao& sessao) {
    if (sistema_) {
        sistema_->sair(sessao);
    }
}

ServicoAcesso::ServicoAcesso(const ISistemaSaveFood& sistema)
    : sistema_(&sistema) {}

bool ServicoAcesso::permitir(const Sessao& sessao, Papel papel,
                            int idRestaurante) const {
    return sistema_ && sistema_->permitir(sessao, papel, idRestaurante);
}

ServicoOfertas::ServicoOfertas(ISistemaSaveFood& sistema, const Sessao& sessao)
    : sistema_(&sistema), sessao_(sessao) {}

bool ServicoOfertas::publicar(const DadosOferta& dados, Oferta& oferta) {
    if (!sistema_) return false;
    int id = sistema_->publicarOferta(sessao_, dados.descricao, dados.quantidade,
                                    dados.pesoTotalKg, dados.precoUnitario,
                                    dados.janelaRetirada, &oferta);
    return id > 0;
}

Colecao<Oferta> ServicoOfertas::consultar(const FiltroOferta& filtro) const {
    return sistema_ ? sistema_->consultarOfertas(sessao_, filtro.termo) : Colecao<Oferta>{};
}

bool ServicoOfertas::detalhes(int idOferta, Oferta& oferta) const {
    return sistema_ && sistema_->detalhesOferta(sessao_, idOferta, oferta);
}

ServicoReservas::ServicoReservas(ISistemaSaveFood& sistema, const Sessao& sessao)
    : sistema_(&sistema), sessao_(sessao) {}

bool ServicoReservas::reservar(int idOferta, int quantidade, Reserva& reserva) {
    int idReserva = 0;
    return sistema_ && sistema_->reservar(sessao_, idOferta, quantidade, idReserva, &reserva);
}

bool ServicoReservas::cancelar(int idReserva) {
    return sistema_ && sistema_->cancelarReserva(sessao_, idReserva);
}

Colecao<Reserva> ServicoReservas::minhasReservas() const {
    return sistema_ ? sistema_->minhasReservas(sessao_) : Colecao<Reserva>{};
}

ServicoRetiradas::ServicoRetiradas(ISistemaSaveFood& sistema, const Sessao& sessao)
    : sistema_(&sistema), sessao_(sessao) {}

Colecao<Reserva> ServicoRetiradas::listar() const {
    return sistema_ ? sistema_->listarReservas(sessao_) : Colecao<Reserva>{};
}

bool ServicoRetiradas::confirmar(int idReserva, const DadosRetirada& dados) {
    return sistema_ && sistema_->confirmarRetirada(sessao_, idReserva,
                                      dados.quantidadeEntregue, dados.pesoAproveitadoKg,
                                      dados.valorRecebido);
}

ServicoIndicadores::ServicoIndicadores(ISistemaSaveFood& sistema, const Sessao& sessao)
    : sistema_(&sistema), sessao_(sessao) {}

ResumoIndicadores ServicoIndicadores::obterResumo() const {
    return sistema_ ? sistema_->obterResumo(sessao_) : ResumoIndicadores{};
}
