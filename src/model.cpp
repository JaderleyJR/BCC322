/** @file model.cpp
 * @brief Implementacao das classes do dominio.
 */
#include "model.h"

#include <cmath>
#include <functional>

namespace {
// Hash ilustrativo para contas ficticias. Nao usar em producao.
std::size_t resumirSenhaDemo(const std::string& senha) {
    return std::hash<std::string>{}("savefood-demo-" + senha);
}
}

Restaurante::Restaurante() : id_(0) {}
Restaurante::Restaurante(int id, const std::string& nome) : id_(id), nome_(nome) {}
Restaurante::Restaurante(const Restaurante& outro) = default;
Restaurante& Restaurante::operator=(const Restaurante& outro) = default;
Restaurante::~Restaurante() = default;
int Restaurante::id() const { return id_; }
const std::string& Restaurante::nome() const { return nome_; }

Usuario::Usuario()
    : id_(0), resumoSenha_(0), perfil_(Perfil::Consumidor), idRestaurante_(0) {}
Usuario::Usuario(int id, const std::string& email, const std::string& senha,
                 Perfil perfil, int idRestaurante)
    : id_(id), email_(email), resumoSenha_(resumirSenhaDemo(senha)),
      perfil_(perfil), idRestaurante_(idRestaurante) {}
Usuario::Usuario(const Usuario& outro) = default;
Usuario& Usuario::operator=(const Usuario& outro) = default;
Usuario::~Usuario() = default;
int Usuario::id() const { return id_; }
const std::string& Usuario::email() const { return email_; }
Perfil Usuario::perfil() const { return perfil_; }
int Usuario::idRestaurante() const { return idRestaurante_; }
bool Usuario::conferirSenha(const std::string& senha) const {
    return resumoSenha_ == resumirSenhaDemo(senha);
}

Sessao::Sessao() : codigo_(0), idUsuario_(0), ativa_(false) {}
Sessao::Sessao(int codigo, int idUsuario)
    : codigo_(codigo), idUsuario_(idUsuario), ativa_(true) {}
Sessao::Sessao(const Sessao& outra) = default;
Sessao& Sessao::operator=(const Sessao& outra) = default;
Sessao::~Sessao() = default;
bool Sessao::ativa() const { return ativa_; }
bool Sessao::autenticada() const { return ativa(); }

Oferta::Oferta()
    : id_(0), idRestaurante_(0), quantidadeTotal_(0),
      quantidadeDisponivel_(0), pesoTotalKg_(0.0), precoUnitario_(0.0), ativa_(false) {}
Oferta::Oferta(int id, int idRestaurante, const std::string& descricao,
               int quantidade, double pesoTotalKg, double precoUnitario,
               const std::string& janelaRetirada)
    : id_(id), idRestaurante_(idRestaurante), descricao_(descricao),
      quantidadeTotal_(quantidade), quantidadeDisponivel_(quantidade),
      pesoTotalKg_(pesoTotalKg), precoUnitario_(precoUnitario),
      janelaRetirada_(janelaRetirada), ativa_(true) {}
Oferta::Oferta(const Oferta& outra) = default;
Oferta& Oferta::operator=(const Oferta& outra) = default;
Oferta::~Oferta() = default;
bool Oferta::valida() const {
    return id_ > 0 && idRestaurante_ > 0 && descricao_.find_first_not_of(" \t\r\n") != std::string::npos &&
           quantidadeTotal_ > 0 && std::isfinite(pesoTotalKg_) &&
           pesoTotalKg_ > 0.0 && std::isfinite(precoUnitario_) &&
           precoUnitario_ > 0.0 && janelaRetirada_.find_first_not_of(" \t\r\n") != std::string::npos;
}
bool Oferta::reservar(int quantidade) {
    if (!valida() || !ativa_ || quantidade <= 0 || quantidade > quantidadeDisponivel_) {
        return false;
    }
    quantidadeDisponivel_ -= quantidade;
    return true;
}
// Restaura somente unidades que estavam reservadas.
bool Oferta::restaurarDisponibilidade(int quantidade) {
    if (!valida() || quantidadeDisponivel_ < 0 ||
        quantidadeDisponivel_ > quantidadeTotal_ || quantidade <= 0 ||
        quantidade > quantidadeTotal_ - quantidadeDisponivel_) {
        return false;
    }
    quantidadeDisponivel_ += quantidade;
    return true;
}
int Oferta::id() const { return id_; }
int Oferta::idRestaurante() const { return idRestaurante_; }
const std::string& Oferta::descricao() const { return descricao_; }
int Oferta::quantidadeTotal() const { return quantidadeTotal_; }
int Oferta::quantidadeDisponivel() const { return quantidadeDisponivel_; }
double Oferta::pesoPorUnidadeKg() const {
    return quantidadeTotal_ > 0 ? pesoTotalKg_ / quantidadeTotal_ : 0.0;
}
double Oferta::precoUnitario() const { return precoUnitario_; }
const std::string& Oferta::janelaRetirada() const { return janelaRetirada_; }
bool Oferta::ativa() const { return ativa_; }
bool Oferta::estaAtiva() const { return ativa_ && quantidadeDisponivel_ > 0; }

Reserva::Reserva()
    : id_(0), idOferta_(0), idConsumidor_(0), quantidadeReservada_(0),
      quantidadeRetirada_(0), valorRecebido_(0.0), pesoAproveitadoKg_(0.0),
      situacao_(EstadoReserva::Pendente) {}
Reserva::Reserva(int id, int idOferta, int idConsumidor, int quantidade)
    : id_(id), idOferta_(idOferta), idConsumidor_(idConsumidor),
      quantidadeReservada_(quantidade), quantidadeRetirada_(0),
      valorRecebido_(0.0), pesoAproveitadoKg_(0.0),
      situacao_(EstadoReserva::Pendente) {}
Reserva::Reserva(const Reserva& outra) = default;
Reserva& Reserva::operator=(const Reserva& outra) = default;
Reserva::~Reserva() = default;
bool Reserva::cancelar() {
    if (id_ <= 0 || idOferta_ <= 0 || idConsumidor_ <= 0 ||
        quantidadeReservada_ <= 0 || situacao_ != EstadoReserva::Pendente) {
        return false;
    }
    situacao_ = EstadoReserva::Cancelada;
    return true;
}

bool Reserva::confirmarRetirada(int quantidadeRetirada, double pesoAproveitadoKg,
                                double valorRecebido) {
    // Retirada parcial fica fora do escopo ate que sua regra seja definida.
    if (id_ <= 0 || idOferta_ <= 0 || idConsumidor_ <= 0 ||
        situacao_ != EstadoReserva::Pendente || quantidadeRetirada <= 0 ||
        quantidadeRetirada != quantidadeReservada_ ||
        !std::isfinite(pesoAproveitadoKg) || pesoAproveitadoKg <= 0.0 ||
        !std::isfinite(valorRecebido) || valorRecebido <= 0.0) {
        return false;
    }
    quantidadeRetirada_ = quantidadeRetirada;
    pesoAproveitadoKg_ = pesoAproveitadoKg;
    valorRecebido_ = valorRecebido;
    situacao_ = EstadoReserva::Retirada;
    return true;
}
int Reserva::id() const { return id_; }
int Reserva::idOferta() const { return idOferta_; }
int Reserva::idConsumidor() const { return idConsumidor_; }
int Reserva::quantidadeReservada() const { return quantidadeReservada_; }
int Reserva::quantidadeRetirada() const { return quantidadeRetirada_; }
double Reserva::valorRecebido() const { return valorRecebido_; }
double Reserva::pesoAproveitadoKg() const { return pesoAproveitadoKg_; }
bool Reserva::retiradaConfirmada() const { return situacao_ == EstadoReserva::Retirada; }
EstadoReserva Reserva::situacao() const { return situacao_; }
