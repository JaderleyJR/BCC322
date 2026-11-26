/** @file services.h
 * @brief Implementacoes concretas dos servicos da API.
 *
 * Camadas superiores devem criar os servicos pela FabricaServicos e
 * usa-los pelas interfaces de servicos_api.h. As classes permanecem
 * exportadas para que os testes unitarios as exercitem diretamente.
 */
#ifndef SAVEFOOD_SERVICES_H
#define SAVEFOOD_SERVICES_H

#include "servicos_api.h"

#include <string>

// Os servicos recebem o mesmo ISistemaSaveFood para compartilhar o dominio.
// As verificacoes de permissao tambem ocorrem dentro do sistema.
// O ponteiro nao possui o sistema: ele deve viver mais que os servicos.
// Um servico construido sem sistema recusa operacoes ate receber um vinculo.
/** @brief Interface para abrir e encerrar sessoes. */
class SAVEFOOD_SERVICOS_API ServicoAutenticacao : public IServicoAutenticacao {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    ISistemaSaveFood* sistema_ = nullptr;
public:
    /** @brief Construtor padrao. */
    ServicoAutenticacao() = default;
    /** @brief Construtor de copia. */
    ServicoAutenticacao(const ServicoAutenticacao& outro) = default;
    /** @brief Operador de atribuicao por copia. */
    ServicoAutenticacao& operator=(const ServicoAutenticacao& outro) = default;
    /** @brief Destrutor. */
    ~ServicoAutenticacao() override = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    explicit ServicoAutenticacao(ISistemaSaveFood& sistema);
    /** @brief Valida credenciais e abre uma sessao; credenciais invalidas nao autenticam. */
    bool entrar(const std::string& email, const std::string& senha,
                Sessao& sessao) override;
    /** @brief Encerra uma sessao de acesso ao sistema. */
    void sair(Sessao& sessao) override;
};

/** @brief Interface auxiliar para verificar autorizacao. */
class SAVEFOOD_SERVICOS_API ServicoAcesso : public IServicoAcesso {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    const ISistemaSaveFood* sistema_ = nullptr;
public:
    /** @brief Construtor padrao. */
    ServicoAcesso() = default;
    /** @brief Construtor de copia. */
    ServicoAcesso(const ServicoAcesso& outro) = default;
    /** @brief Operador de atribuicao por copia. */
    ServicoAcesso& operator=(const ServicoAcesso& outro) = default;
    /** @brief Destrutor. */
    ~ServicoAcesso() override = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    explicit ServicoAcesso(const ISistemaSaveFood& sistema);
    /** @brief Verifica sessao, perfil e, quando informado, restaurante de vinculo. */
    bool permitir(const Sessao& sessao, Papel papel,
                  int idRestaurante = 0) const override;
};

/** @brief Interface de publicacao, pesquisa e detalhamento de ofertas (SB01 e SB02). */
class SAVEFOOD_SERVICOS_API ServicoOfertas : public IServicoOfertas {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    ISistemaSaveFood* sistema_ = nullptr;
    /** @brief Copia da sessao usada nas chamadas. */
    Sessao sessao_;
public:
    /** @brief Construtor padrao. */
    ServicoOfertas() = default;
    /** @brief Construtor de copia. */
    ServicoOfertas(const ServicoOfertas& outro) = default;
    /** @brief Operador de atribuicao por copia. */
    ServicoOfertas& operator=(const ServicoOfertas& outro) = default;
    /** @brief Destrutor. */
    ~ServicoOfertas() override = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    ServicoOfertas(ISistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Publica oferta comercial com os dados informados pelo gerente. */
    bool publicar(const DadosOferta& dados, Oferta& oferta) override;
    /** @brief Pesquisa ofertas disponiveis pelo termo informado. */
    Colecao<Oferta> consultar(const FiltroOferta& filtro) const override;
    /** @brief Retorna detalhes de uma oferta acessivel sem alterar dados. */
    bool detalhes(int idOferta, Oferta& oferta) const override;
};

/** @brief Interface de reserva e cancelamento de unidades (SB03 e SB04). */
class SAVEFOOD_SERVICOS_API ServicoReservas : public IServicoReservas {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    ISistemaSaveFood* sistema_ = nullptr;
    /** @brief Copia da sessao usada nas chamadas. */
    Sessao sessao_;
public:
    /** @brief Construtor padrao. */
    ServicoReservas() = default;
    /** @brief Construtor de copia. */
    ServicoReservas(const ServicoReservas& outro) = default;
    /** @brief Operador de atribuicao por copia. */
    ServicoReservas& operator=(const ServicoReservas& outro) = default;
    /** @brief Destrutor. */
    ~ServicoReservas() override = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    ServicoReservas(ISistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Solicita reserva de unidades; retorna false se a operacao for recusada.
     * @param idOferta ID da oferta.
     * @param quantidade Unidades solicitadas.
     * @param[out] reserva Reserva criada em caso de sucesso.
     * @return true se criada; false em caso de recusa.
     */
    bool reservar(int idOferta, int quantidade, Reserva& reserva) override;
    /** @brief Cancela uma reserva pendente; retorna false se a operacao for recusada. */
    bool cancelar(int idReserva) override;
    /** @brief Lista apenas reservas pertencentes ao consumidor autenticado. */
    Colecao<Reserva> minhasReservas() const override;
};

/** @brief Interface de consulta e confirmacao de retirada (SB05). */
class SAVEFOOD_SERVICOS_API ServicoRetiradas : public IServicoRetiradas {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    ISistemaSaveFood* sistema_ = nullptr;
    /** @brief Copia da sessao usada nas chamadas. */
    Sessao sessao_;
public:
    /** @brief Construtor padrao. */
    ServicoRetiradas() = default;
    /** @brief Construtor de copia. */
    ServicoRetiradas(const ServicoRetiradas& outro) = default;
    /** @brief Operador de atribuicao por copia. */
    ServicoRetiradas& operator=(const ServicoRetiradas& outro) = default;
    /** @brief Destrutor. */
    ~ServicoRetiradas() override = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    ServicoRetiradas(ISistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Lista reservas do estabelecimento vinculado ao gerente. */
    Colecao<Reserva> listar() const override;
    /** @brief Confirma a retirada de uma reserva do restaurante autorizado.
     * @param idReserva ID da reserva.
     * @param dados Dados efetivos da retirada.
     * @return true se confirmada; false se recusada.
     */
    bool confirmar(int idReserva, const DadosRetirada& dados) override;
};

/** @brief Interface de consulta de indicadores do restaurante (SB06). */
class SAVEFOOD_SERVICOS_API ServicoIndicadores : public IServicoIndicadores {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    ISistemaSaveFood* sistema_ = nullptr;
    /** @brief Copia da sessao usada nas chamadas. */
    Sessao sessao_;
public:
    /** @brief Construtor padrao. */
    ServicoIndicadores() = default;
    /** @brief Construtor de copia. */
    ServicoIndicadores(const ServicoIndicadores& outro) = default;
    /** @brief Operador de atribuicao por copia. */
    ServicoIndicadores& operator=(const ServicoIndicadores& outro) = default;
    /** @brief Destrutor. */
    ~ServicoIndicadores() override = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    ServicoIndicadores(ISistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Soma peso e receita de retiradas confirmadas do restaurante autorizado. */
    ResumoIndicadores obterResumo() const override;
};

#endif
