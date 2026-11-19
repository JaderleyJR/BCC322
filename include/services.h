/** @file services.h
 * @brief Interfaces da API e tipos de entrada.
 */
#ifndef SAVEFOOD_SERVICES_H
#define SAVEFOOD_SERVICES_H

#include "system.h"

#include <string>
#include <vector>

// Tipos de entrada e saida do contrato apresentado na Sprint 2.
/** @brief Alias para colecoes retornadas pelos servicos. */
template <typename T>
using Colecao = std::vector<T>;

/** @brief Parametros para publicacao de uma oferta comercial. */
struct DadosOferta {
    /** @brief Descricao do lote. */
    std::string descricao;
    /** @brief Quantidade de unidades. */
    int quantidade = 0;
    /** @brief Peso total em kg. */
    double pesoTotalKg = 0.0;
    /** @brief Preco por unidade em reais. */
    double precoUnitario = 0.0;
    /** @brief Horario de retirada. */
    std::string janelaRetirada;
};

/** @brief Termo opcional para pesquisa no catalogo. */
struct FiltroOferta {
    /** @brief Texto de busca; vazio para todo o catalogo. */
    std::string termo;
};

/** @brief Quantidade, peso e valor efetivamente registrados na retirada. */
struct DadosRetirada {
    /** @brief Unidades efetivamente entregues. */
    int quantidadeEntregue = 0;
    /** @brief Peso aproveitado em kg. */
    double pesoAproveitadoKg = 0.0;
    /** @brief Valor recebido em reais. */
    double valorRecebido = 0.0;
};

// Os servicos recebem o mesmo SistemaSaveFood para compartilhar o dominio.
// As verificacoes de permissao tambem ocorrem dentro do sistema.
// O ponteiro nao possui o sistema: ele deve viver mais que os servicos.
// Um servico construido sem sistema recusa operacoes ate receber um vinculo.
/** @brief Interface para abrir e encerrar sessoes. */
class ServicoAutenticacao {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    SistemaSaveFood* sistema_ = nullptr;
public:
    /** @brief Construtor padrao. */
    ServicoAutenticacao() = default;
    /** @brief Construtor de copia. */
    ServicoAutenticacao(const ServicoAutenticacao& outro) = default;
    /** @brief Operador de atribuicao por copia. */
    ServicoAutenticacao& operator=(const ServicoAutenticacao& outro) = default;
    /** @brief Destrutor. */
    ~ServicoAutenticacao() = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    explicit ServicoAutenticacao(SistemaSaveFood& sistema);
    /** @brief Valida credenciais e abre uma sessao; credenciais invalidas nao autenticam. */
    bool entrar(const std::string& email, const std::string& senha,
                Sessao& sessao);
    /** @brief Encerra uma sessao de acesso ao sistema. */
    void sair(Sessao& sessao);
};

/** @brief Interface auxiliar para verificar autorizacao. */
class ServicoAcesso {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    const SistemaSaveFood* sistema_ = nullptr;
public:
    /** @brief Construtor padrao. */
    ServicoAcesso() = default;
    /** @brief Construtor de copia. */
    ServicoAcesso(const ServicoAcesso& outro) = default;
    /** @brief Operador de atribuicao por copia. */
    ServicoAcesso& operator=(const ServicoAcesso& outro) = default;
    /** @brief Destrutor. */
    ~ServicoAcesso() = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    explicit ServicoAcesso(const SistemaSaveFood& sistema);
    /** @brief Verifica sessao, perfil e, quando informado, restaurante de vinculo. */
    bool permitir(const Sessao& sessao, Papel papel,
                  int idRestaurante = 0) const;
};

/** @brief Interface de publicacao, pesquisa e detalhamento de ofertas (SB01 e SB02). */
class ServicoOfertas {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    SistemaSaveFood* sistema_ = nullptr;
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
    ~ServicoOfertas() = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    ServicoOfertas(SistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Publica oferta comercial com os dados informados pelo gerente. */
    bool publicar(const DadosOferta& dados, Oferta& oferta);
    /** @brief Pesquisa ofertas disponiveis pelo termo informado. */
    Colecao<Oferta> consultar(const FiltroOferta& filtro) const;
    /** @brief Retorna detalhes de uma oferta acessivel sem alterar dados. */
    bool detalhes(int idOferta, Oferta& oferta) const;
};

/** @brief Interface de reserva e cancelamento de unidades (SB03 e SB04). */
class ServicoReservas {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    SistemaSaveFood* sistema_ = nullptr;
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
    ~ServicoReservas() = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    ServicoReservas(SistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Solicita reserva de unidades; retorna false se a operacao for recusada.
     * @param idOferta ID da oferta.
     * @param quantidade Unidades solicitadas.
     * @param[out] reserva Reserva criada em caso de sucesso.
     * @return true se criada; false em caso de recusa.
     */
    bool reservar(int idOferta, int quantidade, Reserva& reserva);
    /** @brief Cancela uma reserva pendente; retorna false se a operacao for recusada. */
    bool cancelar(int idReserva);
    /** @brief Lista apenas reservas pertencentes ao consumidor autenticado. */
    Colecao<Reserva> minhasReservas() const;
};

/** @brief Interface de consulta e confirmacao de retirada (SB05). */
class ServicoRetiradas {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    SistemaSaveFood* sistema_ = nullptr;
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
    ~ServicoRetiradas() = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    ServicoRetiradas(SistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Lista reservas do estabelecimento vinculado ao gerente. */
    Colecao<Reserva> listar() const;
    /** @brief Confirma a retirada de uma reserva do restaurante autorizado.
     * @param idReserva ID da reserva.
     * @param dados Dados efetivos da retirada.
     * @return true se confirmada; false se recusada.
     */
    bool confirmar(int idReserva, const DadosRetirada& dados);
};

/** @brief Interface de consulta de indicadores do restaurante (SB06). */
class ServicoIndicadores {
    /** @brief Ponteiro nao proprietario para o sistema compartilhado. */
    SistemaSaveFood* sistema_ = nullptr;
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
    ~ServicoIndicadores() = default;

    /** @brief Vincula o servico ao sistema compartilhado, sem assumir sua propriedade. */
    ServicoIndicadores(SistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Soma peso e receita de retiradas confirmadas do restaurante autorizado. */
    ResumoIndicadores obterResumo() const;
};

#endif
