#include "services.h"

ServicoAutenticacao::ServicoAutenticacao(SistemaSaveFood& sistema)
    : sistema_(sistema) {}

bool ServicoAutenticacao::entrar(const std::string& email,
                                const std::string& senha, Sessao& sessao) {
    sessao = sistema_.entrar(email, senha);
    return sessao.autenticada();
}

void ServicoAutenticacao::sair(Sessao& sessao) {
    sistema_.sair(sessao);
}

ServicoAcesso::ServicoAcesso(const SistemaSaveFood& sistema)
    : sistema_(sistema) {}

bool ServicoAcesso::permitir(const Sessao& sessao, Papel papel,
                            int idRestaurante) const {
    return sistema_.permitir(sessao, papel, idRestaurante);
}

ServicoOfertas::ServicoOfertas(SistemaSaveFood& sistema, const Sessao& sessao)
    : sistema_(sistema), sessao_(sessao) {}

bool ServicoOfertas::publicar(const DadosOferta& dados, Oferta& oferta) {
    int id = sistema_.publicarOferta(sessao_, dados.descricao, dados.quantidade,
                                    dados.pesoKgInformado, dados.preco,
                                    dados.janelaRetirada, &oferta);
    return id > 0;
}

Colecao<Oferta> ServicoOfertas::consultar(const FiltroOferta& filtro) const {
    return sistema_.consultarOfertas(sessao_, filtro.pesquisa);
}

bool ServicoOfertas::detalhes(int idOferta, Oferta& oferta) const {
    return sistema_.detalhesOferta(sessao_, idOferta, oferta);
}

ServicoReservas::ServicoReservas(SistemaSaveFood& sistema, const Sessao& sessao)
    : sistema_(sistema), sessao_(sessao) {}

bool ServicoReservas::reservar(int idOferta, int quantidade, Reserva& reserva) {
    int idReserva = 0;
    return sistema_.reservar(sessao_, idOferta, quantidade, idReserva, &reserva);
}

ServicoRetiradas::ServicoRetiradas(SistemaSaveFood& sistema, const Sessao& sessao)
    : sistema_(sistema), sessao_(sessao) {}

Colecao<Reserva> ServicoRetiradas::listar() const {
    return sistema_.listarReservas(sessao_);
}

bool ServicoRetiradas::confirmar(int idReserva, const ConfirmacaoRetirada& dados) {
    return sistema_.confirmarRetirada(sessao_, idReserva,
                                      dados.quantidadeRetirada, dados.valorRecebido);
}

ServicoIndicadores::ServicoIndicadores(SistemaSaveFood& sistema, const Sessao& sessao)
    : sistema_(sistema), sessao_(sessao) {}

ResumoIndicadores ServicoIndicadores::obterResumo() const {
    return sistema_.obterResumo(sessao_);
}
