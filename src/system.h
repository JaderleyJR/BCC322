#ifndef SAVEFOOD_SYSTEM_H
#define SAVEFOOD_SYSTEM_H

#include "model.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

// Servicos centrais do MVP. Os dados ficam em memoria nesta sprint.
class SistemaSaveFood {
private:
    mutable std::mutex mutex_;
    std::map<int, Restaurante> restaurantes_;
    std::map<int, Usuario> usuarios_;
    std::map<int, Oferta> ofertas_;
    std::map<int, Reserva> reservas_;
    std::map<int, int> sessoesAtivas_;
    int proximoIdSessao_;
    int proximoIdOferta_;
    int proximoIdReserva_;

    void prepararContasDemo();
    const Usuario* usuarioDaSessaoSemLock(const Sessao& sessao) const;

public:
    SistemaSaveFood();
    SistemaSaveFood(const SistemaSaveFood& outro);
    SistemaSaveFood& operator=(const SistemaSaveFood& outro);
    ~SistemaSaveFood();

    Sessao entrar(const std::string& email, const std::string& senha);
    bool sair(Sessao& sessao);

    // Retorna zero quando a publicacao nao e permitida ou possui dados invalidos.
    int publicarOferta(const Sessao& gerente, const std::string& descricao,
                       int quantidade, double pesoTotalKg, double precoUnitario,
                       const std::string& janelaRetirada,
                       Oferta* ofertaPublicada = nullptr);

    std::vector<Oferta> consultarOfertas(const Sessao& consumidor,
                                        const std::string& pesquisa = "") const;
    // Consulta de uma oferta por ID, conforme previsto na API da Sprint 2.
    bool detalhesOferta(const Sessao& usuario, int idOferta, Oferta& oferta) const;
    bool reservar(const Sessao& consumidor, int idOferta, int quantidade,
                  int& idReservaCriada, Reserva* reservaCriada = nullptr);

    // Verificacao interna de perfil e vinculo com o restaurante.
    bool permitir(const Sessao& sessao, Perfil perfil,
                  int idRestaurante = 0) const;

    std::vector<Reserva> listarReservas(const Sessao& gerente) const;
    bool confirmarRetirada(const Sessao& gerente, int idReserva,
                           int quantidadeRetirada, double valorRecebido);
    ResumoIndicadores obterResumo(const Sessao& gerente) const;

    // Consulta auxiliar utilizada nas demonstracoes e nos testes.
    int saldoOferta(const Sessao& usuario, int idOferta) const;
};

#endif
