/** @file system.h
 * @brief Operacoes centrais e armazenamento em memoria.
 */
#ifndef SAVEFOOD_SYSTEM_H
#define SAVEFOOD_SYSTEM_H

#include "model.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

// Servicos centrais do MVP. Os dados ficam em memoria nesta sprint.
/** @brief Centraliza o dominio em memoria e protege as operacoes com mutex. */
class SistemaSaveFood {
private:
    /** @brief Protege os dados compartilhados. */
    mutable std::mutex mutex_;
    /** @brief Restaurantes de demonstracao por ID. */
    std::map<int, Restaurante> restaurantes_;
    /** @brief Contas por ID. */
    std::map<int, Usuario> usuarios_;
    /** @brief Ofertas por ID. */
    std::map<int, Oferta> ofertas_;
    /** @brief Reservas por ID. */
    std::map<int, Reserva> reservas_;
    /** @brief Sessoes validas e IDs dos usuarios associados. */
    std::map<int, int> sessoesAtivas_;
    /** @brief Proximo codigo de sessao. */
    int proximoIdSessao_;
    /** @brief Proximo ID de oferta. */
    int proximoIdOferta_;
    /** @brief Proximo ID de reserva. */
    int proximoIdReserva_;

    /** @brief Cria as contas e restaurantes ficticios do exercicio. */
    void prepararContasDemo();
    /** @brief Localiza o usuario de uma sessao valida. Requer o mutex adquirido pelo chamador. */
    const Usuario* usuarioDaSessaoSemLock(const Sessao& sessao) const;

public:
    /** @brief Construtor padrao. */
    SistemaSaveFood();
    /** @brief Construtor de copia. */
    SistemaSaveFood(const SistemaSaveFood& outro);
    /** @brief Operador de atribuicao por copia. */
    SistemaSaveFood& operator=(const SistemaSaveFood& outro);
    /** @brief Destrutor. */
    ~SistemaSaveFood();

    /** @brief Valida credenciais e abre uma sessao; credenciais invalidas nao autenticam. */
    Sessao entrar(const std::string& email, const std::string& senha);
    /** @brief Encerra uma sessao de acesso ao sistema. */
    bool sair(Sessao& sessao);

    // Retorna zero quando a publicacao nao e permitida ou possui dados invalidos.
    /** @brief Publica oferta de gerente autorizado; retorna ID positivo ou zero em caso de recusa.
     * @param gerente Sessao do gerente.
     * @param descricao Descricao do lote.
     * @param quantidade Quantidade inicial.
     * @param pesoTotalKg Peso total em kg.
     * @param precoUnitario Preco em reais.
     * @param janelaRetirada Horario de retirada.
     * @param[out] ofertaPublicada Saida opcional com a oferta criada.
     * @return ID da oferta ou 0 se recusada.
     * @note Se a copia do resultado lancar uma excecao, a insercao e desfeita.
     */
    int publicarOferta(const Sessao& gerente, const std::string& descricao,
                       int quantidade, double pesoTotalKg, double precoUnitario,
                       const std::string& janelaRetirada,
                       Oferta* ofertaPublicada = nullptr);

    /** @brief Lista ofertas ativas e disponiveis que correspondem ao filtro do consumidor. */
    std::vector<Oferta> consultarOfertas(const Sessao& consumidor,
                                        const std::string& pesquisa = "") const;
    // Consulta de uma oferta por ID, conforme previsto na API da Sprint 2.
    /** @brief Consulta uma oferta acessivel ao usuario, sem alterar o estoque. */
    bool detalhesOferta(const Sessao& usuario, int idOferta, Oferta& oferta) const;
    /** @brief Solicita reserva de unidades; retorna false se a operacao for recusada.
     * @param consumidor Sessao do consumidor.
     * @param idOferta ID da oferta.
     * @param quantidade Unidades solicitadas.
     * @param[out] idReservaCriada ID atribuido em caso de sucesso.
     * @param[out] reservaCriada Saida opcional com a reserva.
     * @return true se criada; false sem baixar o estoque.
     * @note A gravacao e a baixa de saldo usam a mesma regiao critica.
     */
    bool reservar(const Sessao& consumidor, int idOferta, int quantidade,
                  int& idReservaCriada, Reserva* reservaCriada = nullptr);

    /** @brief Cancela reserva do titular e restaura o saldo uma unica vez.
     * @param consumidor Sessao do titular.
     * @param idReserva Reserva pendente.
     * @return true se cancelada; false sem devolver unidades.
     */
    bool cancelarReserva(const Sessao& consumidor, int idReserva);
    /** @brief Lista apenas reservas pertencentes ao consumidor autenticado. */
    std::vector<Reserva> minhasReservas(const Sessao& consumidor) const;

    // Verificacao interna de perfil e vinculo com o restaurante.
    /** @brief Verifica sessao, perfil e, quando informado, restaurante de vinculo. */
    bool permitir(const Sessao& sessao, Perfil perfil,
                  int idRestaurante = 0) const;

    /** @brief Lista reservas do restaurante do gerente autenticado. */
    std::vector<Reserva> listarReservas(const Sessao& gerente) const;
    /** @brief Registra uma retirada integral, com peso e valor positivos e finitos. */
    bool confirmarRetirada(const Sessao& gerente, int idReserva,
                           int quantidadeRetirada, double pesoAproveitadoKg,
                           double valorRecebido);
    /** @brief Soma peso e receita de retiradas confirmadas do restaurante autorizado. */
    ResumoIndicadores obterResumo(const Sessao& gerente) const;

    // Consulta auxiliar utilizada nas demonstracoes e nos testes.
    /** @brief Retorna o saldo da oferta ou -1 se inexistente ou inacessivel. */
    int saldoOferta(const Sessao& usuario, int idOferta) const;
};

#endif
