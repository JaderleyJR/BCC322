#ifndef SAVEFOOD_SERVICES_H
#define SAVEFOOD_SERVICES_H

#include "system.h"

#include <string>
#include <vector>

// Tipos de entrada e saida do contrato apresentado na Sprint 2.
template <typename T>
using Colecao = std::vector<T>;

struct DadosOferta {
    std::string descricao;
    int quantidade = 0;
    double pesoKgInformado = 0.0;
    double preco = 0.0;
    std::string janelaRetirada;
};

struct FiltroOferta {
    std::string pesquisa;
};

struct ConfirmacaoRetirada {
    int quantidadeRetirada = 0;
    double valorRecebido = 0.0;
};

// Os servicos recebem o mesmo SistemaSaveFood para compartilhar o dominio.
// As verificacoes de permissao tambem ocorrem dentro do sistema.
class ServicoAutenticacao {
    SistemaSaveFood& sistema_;
public:
    explicit ServicoAutenticacao(SistemaSaveFood& sistema);
    bool entrar(const std::string& email, const std::string& senha,
                Sessao& sessao);
    void sair(Sessao& sessao);
};

class ServicoAcesso {
    const SistemaSaveFood& sistema_;
public:
    explicit ServicoAcesso(const SistemaSaveFood& sistema);
    bool permitir(const Sessao& sessao, Papel papel,
                  int idRestaurante = 0) const;
};

class ServicoOfertas {
    SistemaSaveFood& sistema_;
    Sessao sessao_;
public:
    ServicoOfertas(SistemaSaveFood& sistema, const Sessao& sessao);
    bool publicar(const DadosOferta& dados, Oferta& oferta);
    Colecao<Oferta> consultar(const FiltroOferta& filtro) const;
    bool detalhes(int idOferta, Oferta& oferta) const;
};

class ServicoReservas {
    SistemaSaveFood& sistema_;
    Sessao sessao_;
public:
    ServicoReservas(SistemaSaveFood& sistema, const Sessao& sessao);
    bool reservar(int idOferta, int quantidade, Reserva& reserva);
};

class ServicoRetiradas {
    SistemaSaveFood& sistema_;
    Sessao sessao_;
public:
    ServicoRetiradas(SistemaSaveFood& sistema, const Sessao& sessao);
    Colecao<Reserva> listar() const;
    bool confirmar(int idReserva, const ConfirmacaoRetirada& dados);
};

class ServicoIndicadores {
    SistemaSaveFood& sistema_;
    Sessao sessao_;
public:
    ServicoIndicadores(SistemaSaveFood& sistema, const Sessao& sessao);
    ResumoIndicadores obterResumo() const;
};

#endif
