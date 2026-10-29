#include "system.h"

#include <algorithm>
#include <cctype>
#include <utility>

namespace {
std::string paraMinusculas(std::string texto) {
    std::transform(texto.begin(), texto.end(), texto.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return texto;
}
}

SistemaSaveFood::SistemaSaveFood()
    : proximoIdSessao_(1), proximoIdOferta_(1), proximoIdReserva_(1) {
    prepararContasDemo();
}

SistemaSaveFood::SistemaSaveFood(const SistemaSaveFood& outro) {
    std::lock_guard<std::mutex> trava(outro.mutex_);
    restaurantes_ = outro.restaurantes_;
    usuarios_ = outro.usuarios_;
    ofertas_ = outro.ofertas_;
    reservas_ = outro.reservas_;
    sessoesAtivas_ = outro.sessoesAtivas_;
    proximoIdSessao_ = outro.proximoIdSessao_;
    proximoIdOferta_ = outro.proximoIdOferta_;
    proximoIdReserva_ = outro.proximoIdReserva_;
}

SistemaSaveFood& SistemaSaveFood::operator=(const SistemaSaveFood& outro) {
    if (this == &outro) {
        return *this;
    }
    std::scoped_lock trava(mutex_, outro.mutex_);
    restaurantes_ = outro.restaurantes_;
    usuarios_ = outro.usuarios_;
    ofertas_ = outro.ofertas_;
    reservas_ = outro.reservas_;
    sessoesAtivas_ = outro.sessoesAtivas_;
    proximoIdSessao_ = outro.proximoIdSessao_;
    proximoIdOferta_ = outro.proximoIdOferta_;
    proximoIdReserva_ = outro.proximoIdReserva_;
    return *this;
}

SistemaSaveFood::~SistemaSaveFood() = default;

void SistemaSaveFood::prepararContasDemo() {
    // Nao ha cadastro aberto na primeira sprint.
    restaurantes_.emplace(1, Restaurante(1, "Restaurante A"));
    restaurantes_.emplace(2, Restaurante(2, "Restaurante B"));
    usuarios_.emplace(1, Usuario(1, "gerente@savefood.demo", "gerente123", Perfil::Gerente, 1));
    usuarios_.emplace(2, Usuario(2, "consumidor@savefood.demo", "consumidor123", Perfil::Consumidor, 0));
    usuarios_.emplace(3, Usuario(3, "consumidor2@savefood.demo", "consumidor123", Perfil::Consumidor, 0));
    usuarios_.emplace(4, Usuario(4, "gerente2@savefood.demo", "gerente123", Perfil::Gerente, 2));
}

const Usuario* SistemaSaveFood::usuarioDaSessaoSemLock(const Sessao& sessao) const {
    if (!sessao.ativa_) {
        return nullptr;
    }
    auto itSessao = sessoesAtivas_.find(sessao.codigo_);
    if (itSessao == sessoesAtivas_.end() || itSessao->second != sessao.idUsuario_) {
        return nullptr;
    }
    auto itUsuario = usuarios_.find(sessao.idUsuario_);
    return itUsuario != usuarios_.end() ? &itUsuario->second : nullptr;
}

Sessao SistemaSaveFood::entrar(const std::string& email, const std::string& senha) {
    std::lock_guard<std::mutex> trava(mutex_);
    for (const auto& item : usuarios_) {
        if (item.second.email() == email && item.second.conferirSenha(senha)) {
            const int codigo = proximoIdSessao_++;
            sessoesAtivas_.emplace(codigo, item.first);
            return Sessao(codigo, item.first);
        }
    }
    return Sessao();
}

bool SistemaSaveFood::sair(Sessao& sessao) {
    std::lock_guard<std::mutex> trava(mutex_);
    if (!usuarioDaSessaoSemLock(sessao)) {
        return false;
    }
    sessoesAtivas_.erase(sessao.codigo_);
    sessao.ativa_ = false;
    return true;
}

int SistemaSaveFood::publicarOferta(const Sessao& gerente, const std::string& descricao,
                                   int quantidade, double pesoTotalKg, double precoUnitario,
                                   const std::string& janelaRetirada,
                                   Oferta* ofertaPublicada) {
    std::lock_guard<std::mutex> trava(mutex_);
    const Usuario* usuario = usuarioDaSessaoSemLock(gerente);
    if (!usuario || usuario->perfil() != Perfil::Gerente ||
        restaurantes_.find(usuario->idRestaurante()) == restaurantes_.end()) {
        return 0;
    }
    const int id = proximoIdOferta_;
    Oferta oferta(id, usuario->idRestaurante(), descricao, quantidade,
                  pesoTotalKg, precoUnitario, janelaRetirada);
    if (!oferta.valida()) {
        return 0;
    }
    ofertas_.emplace(id, oferta);
    if (ofertaPublicada) {
        *ofertaPublicada = oferta;
    }
    ++proximoIdOferta_;
    return id;
}

std::vector<Oferta> SistemaSaveFood::consultarOfertas(const Sessao& consumidor,
                                                      const std::string& pesquisa) const {
    std::lock_guard<std::mutex> trava(mutex_);
    const Usuario* usuario = usuarioDaSessaoSemLock(consumidor);
    if (!usuario || usuario->perfil() != Perfil::Consumidor) {
        return {};
    }
    std::vector<Oferta> encontradas;
    const std::string filtro = paraMinusculas(pesquisa);
    for (const auto& item : ofertas_) {
        const Oferta& oferta = item.second;
        if (oferta.ativa() && oferta.quantidadeDisponivel() > 0 &&
            paraMinusculas(oferta.descricao()).find(filtro) != std::string::npos) {
            encontradas.push_back(oferta);
        }
    }
    return encontradas;
}

bool SistemaSaveFood::detalhesOferta(const Sessao& sessao, int idOferta,
                                    Oferta& oferta) const {
    std::lock_guard<std::mutex> trava(mutex_);
    const Usuario* usuario = usuarioDaSessaoSemLock(sessao);
    auto it = ofertas_.find(idOferta);
    if (!usuario || it == ofertas_.end()) {
        return false;
    }
    const Oferta& encontrada = it->second;
    const bool gerenteDono = usuario->perfil() == Perfil::Gerente &&
                              usuario->idRestaurante() == encontrada.idRestaurante();
    const bool consumidor = usuario->perfil() == Perfil::Consumidor &&
                           encontrada.estaAtiva();
    if (!gerenteDono && !consumidor) {
        return false;
    }
    oferta = encontrada;
    return true;
}

bool SistemaSaveFood::reservar(const Sessao& consumidor, int idOferta, int quantidade,
                               int& idReservaCriada, Reserva* reservaCriada) {
    std::lock_guard<std::mutex> trava(mutex_);
    const Usuario* usuario = usuarioDaSessaoSemLock(consumidor);
    auto itOferta = ofertas_.find(idOferta);
    if (!usuario || usuario->perfil() != Perfil::Consumidor ||
        itOferta == ofertas_.end() || quantidade <= 0 ||
        quantidade > itOferta->second.quantidadeDisponivel()) {
        return false;
    }

    // A gravacao e a baixa do saldo ficam na mesma regiao critica.
    const int id = proximoIdReserva_;
    reservas_.emplace(id, Reserva(id, idOferta, usuario->id(), quantidade));
    if (!itOferta->second.reservar(quantidade)) {
        reservas_.erase(id);
        return false;
    }
    ++proximoIdReserva_;
    idReservaCriada = id;
    if (reservaCriada) {
        *reservaCriada = reservas_.at(id);
    }
    return true;
}

bool SistemaSaveFood::permitir(const Sessao& sessao, Perfil perfil,
                               int idRestaurante) const {
    std::lock_guard<std::mutex> trava(mutex_);
    const Usuario* usuario = usuarioDaSessaoSemLock(sessao);
    return usuario && usuario->perfil() == perfil &&
           (idRestaurante == 0 ||
            (perfil == Perfil::Gerente && usuario->idRestaurante() == idRestaurante));
}

std::vector<Reserva> SistemaSaveFood::listarReservas(const Sessao& gerente) const {
    std::lock_guard<std::mutex> trava(mutex_);
    const Usuario* usuario = usuarioDaSessaoSemLock(gerente);
    if (!usuario || usuario->perfil() != Perfil::Gerente) {
        return {};
    }
    std::vector<Reserva> encontradas;
    for (const auto& item : reservas_) {
        auto itOferta = ofertas_.find(item.second.idOferta());
        if (itOferta != ofertas_.end() &&
            itOferta->second.idRestaurante() == usuario->idRestaurante()) {
            encontradas.push_back(item.second);
        }
    }
    return encontradas;
}

bool SistemaSaveFood::confirmarRetirada(const Sessao& gerente, int idReserva,
                                        int quantidadeRetirada, double valorRecebido) {
    std::lock_guard<std::mutex> trava(mutex_);
    const Usuario* usuario = usuarioDaSessaoSemLock(gerente);
    auto itReserva = reservas_.find(idReserva);
    if (!usuario || usuario->perfil() != Perfil::Gerente ||
        itReserva == reservas_.end()) {
        return false;
    }
    auto itOferta = ofertas_.find(itReserva->second.idOferta());
    if (itOferta == ofertas_.end() ||
        itOferta->second.idRestaurante() != usuario->idRestaurante()) {
        return false;
    }
    return itReserva->second.confirmarRetirada(quantidadeRetirada, valorRecebido);
}

ResumoIndicadores SistemaSaveFood::obterResumo(const Sessao& gerente) const {
    std::lock_guard<std::mutex> trava(mutex_);
    ResumoIndicadores resumo;
    const Usuario* usuario = usuarioDaSessaoSemLock(gerente);
    if (!usuario || usuario->perfil() != Perfil::Gerente) {
        return resumo;
    }
    for (const auto& item : reservas_) {
        const Reserva& reserva = item.second;
        if (!reserva.retiradaConfirmada()) {
            continue;
        }
        auto itOferta = ofertas_.find(reserva.idOferta());
        if (itOferta != ofertas_.end() &&
            itOferta->second.idRestaurante() == usuario->idRestaurante()) {
            resumo.pesoAproveitadoKg +=
                reserva.quantidadeRetirada() * itOferta->second.pesoPorUnidadeKg();
            resumo.receitaBrutaReais += reserva.valorRecebido();
        }
    }
    return resumo;
}

int SistemaSaveFood::saldoOferta(const Sessao& sessao, int idOferta) const {
    std::lock_guard<std::mutex> trava(mutex_);
    const Usuario* usuario = usuarioDaSessaoSemLock(sessao);
    auto itOferta = ofertas_.find(idOferta);
    if (!usuario || itOferta == ofertas_.end()) {
        return -1;
    }
    if (usuario->perfil() == Perfil::Gerente &&
        usuario->idRestaurante() != itOferta->second.idRestaurante()) {
        return -1;
    }
    return itOferta->second.quantidadeDisponivel();
}
