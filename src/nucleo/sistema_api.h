/** @file sistema_api.h
 * @brief Interface publica da camada de nucleo: ISistemaSaveFood e FabricaSistema.
 *
 * As camadas superiores (servicos e apresentacao) conhecem o nucleo apenas
 * por esta interface abstrata. A classe concreta SistemaSaveFood fica em
 * system.h e so e criada pela FabricaSistema.
 */
#ifndef SAVEFOOD_SISTEMA_API_H
#define SAVEFOOD_SISTEMA_API_H

#include "../exportacao.h"
#include "../dominio/model.h"

#include <memory>
#include <string>
#include <vector>

/** @brief Contrato das operacoes centrais do SaveFood (camada de nucleo). */
class SAVEFOOD_NUCLEO_API ISistemaSaveFood {
public:
    /** @brief Destrutor virtual: permite destruir a implementacao pela interface. */
    virtual ~ISistemaSaveFood();

    /** @brief Valida credenciais e abre uma sessao; credenciais invalidas nao autenticam. */
    virtual Sessao entrar(const std::string& email, const std::string& senha) = 0;
    /** @brief Encerra uma sessao de acesso ao sistema. */
    virtual bool sair(Sessao& sessao) = 0;

    /** @brief Publica oferta de gerente autorizado; retorna ID positivo ou zero em caso de recusa. */
    virtual int publicarOferta(const Sessao& gerente, const std::string& descricao,
                               int quantidade, double pesoTotalKg, double precoUnitario,
                               const std::string& janelaRetirada,
                               Oferta* ofertaPublicada = nullptr) = 0;
    /** @brief Lista ofertas ativas e disponiveis que correspondem ao filtro do consumidor. */
    virtual std::vector<Oferta> consultarOfertas(const Sessao& consumidor,
                                                 const std::string& pesquisa = "") const = 0;
    /** @brief Consulta uma oferta acessivel ao usuario, sem alterar o estoque. */
    virtual bool detalhesOferta(const Sessao& usuario, int idOferta, Oferta& oferta) const = 0;
    /** @brief Solicita reserva de unidades; retorna false se a operacao for recusada. */
    virtual bool reservar(const Sessao& consumidor, int idOferta, int quantidade,
                          int& idReservaCriada, Reserva* reservaCriada = nullptr) = 0;
    /** @brief Cancela reserva do titular e restaura o saldo uma unica vez. */
    virtual bool cancelarReserva(const Sessao& consumidor, int idReserva) = 0;
    /** @brief Lista apenas reservas pertencentes ao consumidor autenticado. */
    virtual std::vector<Reserva> minhasReservas(const Sessao& consumidor) const = 0;
    /** @brief Verifica sessao, perfil e, quando informado, restaurante de vinculo. */
    virtual bool permitir(const Sessao& sessao, Perfil perfil, int idRestaurante = 0) const = 0;
    /** @brief Lista reservas do restaurante do gerente autenticado. */
    virtual std::vector<Reserva> listarReservas(const Sessao& gerente) const = 0;
    /** @brief Registra uma retirada integral, com peso e valor positivos e finitos. */
    virtual bool confirmarRetirada(const Sessao& gerente, int idReserva,
                                   int quantidadeRetirada, double pesoAproveitadoKg,
                                   double valorRecebido) = 0;
    /** @brief Soma peso e receita de retiradas confirmadas do restaurante autorizado. */
    virtual ResumoIndicadores obterResumo(const Sessao& gerente) const = 0;
    /** @brief Retorna o saldo da oferta ou -1 se inexistente ou inacessivel. */
    virtual int saldoOferta(const Sessao& usuario, int idOferta) const = 0;

protected:
    /** @brief Construtor padrao, acessivel somente as implementacoes. */
    ISistemaSaveFood() = default;
    /** @brief Construtor de copia, acessivel somente as implementacoes. */
    ISistemaSaveFood(const ISistemaSaveFood& outro) = default;
    /** @brief Atribuicao por copia, acessivel somente as implementacoes. */
    ISistemaSaveFood& operator=(const ISistemaSaveFood& outro) = default;
};

/** @brief Fabrica da camada de nucleo: unico ponto de criacao do sistema.
 *
 * Quem usa o nucleo recebe um ISistemaSaveFood e nunca depende da classe
 * concreta. Trocar a implementacao (por exemplo, por uma com banco de
 * dados) exigira alterar apenas esta fabrica.
 */
class SAVEFOOD_NUCLEO_API FabricaSistema {
public:
    /** @brief A fabrica so possui metodos estaticos; nao deve ser instanciada. */
    FabricaSistema() = delete;

    /** @brief Cria um sistema em memoria com as contas de demonstracao.
     * @return Ponteiro proprietario para a nova instancia.
     */
    static std::unique_ptr<ISistemaSaveFood> criarSistema();
};

#endif
