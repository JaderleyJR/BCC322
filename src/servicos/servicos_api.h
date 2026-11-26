/** @file servicos_api.h
 * @brief Interface publica da camada de servicos: tipos de entrada,
 * interfaces IServico* e FabricaServicos.
 *
 * A camada de apresentacao usa somente este cabecalho. As classes
 * concretas ficam em services.h e so sao criadas pela FabricaServicos.
 */
#ifndef SAVEFOOD_SERVICOS_API_H
#define SAVEFOOD_SERVICOS_API_H

#include "../exportacao.h"
#include "../nucleo/sistema_api.h"

#include <memory>
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

/** @brief Contrato para abrir e encerrar sessoes. */
class SAVEFOOD_SERVICOS_API IServicoAutenticacao {
public:
    /** @brief Destrutor virtual. */
    virtual ~IServicoAutenticacao();
    /** @brief Valida credenciais e abre uma sessao; credenciais invalidas nao autenticam. */
    virtual bool entrar(const std::string& email, const std::string& senha, Sessao& sessao) = 0;
    /** @brief Encerra uma sessao de acesso ao sistema. */
    virtual void sair(Sessao& sessao) = 0;
protected:
    /** @brief Construtor padrao, acessivel somente as implementacoes. */
    IServicoAutenticacao() = default;
    /** @brief Construtor de copia, acessivel somente as implementacoes. */
    IServicoAutenticacao(const IServicoAutenticacao& outro) = default;
    /** @brief Atribuicao por copia, acessivel somente as implementacoes. */
    IServicoAutenticacao& operator=(const IServicoAutenticacao& outro) = default;
};

/** @brief Contrato auxiliar para verificar autorizacao. */
class SAVEFOOD_SERVICOS_API IServicoAcesso {
public:
    /** @brief Destrutor virtual. */
    virtual ~IServicoAcesso();
    /** @brief Verifica sessao, perfil e, quando informado, restaurante de vinculo. */
    virtual bool permitir(const Sessao& sessao, Papel papel, int idRestaurante = 0) const = 0;
protected:
    /** @brief Construtor padrao, acessivel somente as implementacoes. */
    IServicoAcesso() = default;
    /** @brief Construtor de copia, acessivel somente as implementacoes. */
    IServicoAcesso(const IServicoAcesso& outro) = default;
    /** @brief Atribuicao por copia, acessivel somente as implementacoes. */
    IServicoAcesso& operator=(const IServicoAcesso& outro) = default;
};

/** @brief Contrato de publicacao, pesquisa e detalhamento de ofertas (SB01 e SB02). */
class SAVEFOOD_SERVICOS_API IServicoOfertas {
public:
    /** @brief Destrutor virtual. */
    virtual ~IServicoOfertas();
    /** @brief Publica oferta comercial com os dados informados pelo gerente. */
    virtual bool publicar(const DadosOferta& dados, Oferta& oferta) = 0;
    /** @brief Pesquisa ofertas disponiveis pelo termo informado. */
    virtual Colecao<Oferta> consultar(const FiltroOferta& filtro) const = 0;
    /** @brief Retorna detalhes de uma oferta acessivel sem alterar dados. */
    virtual bool detalhes(int idOferta, Oferta& oferta) const = 0;
protected:
    /** @brief Construtor padrao, acessivel somente as implementacoes. */
    IServicoOfertas() = default;
    /** @brief Construtor de copia, acessivel somente as implementacoes. */
    IServicoOfertas(const IServicoOfertas& outro) = default;
    /** @brief Atribuicao por copia, acessivel somente as implementacoes. */
    IServicoOfertas& operator=(const IServicoOfertas& outro) = default;
};

/** @brief Contrato de reserva e cancelamento de unidades (SB03 e SB04). */
class SAVEFOOD_SERVICOS_API IServicoReservas {
public:
    /** @brief Destrutor virtual. */
    virtual ~IServicoReservas();
    /** @brief Solicita reserva de unidades; retorna false se a operacao for recusada. */
    virtual bool reservar(int idOferta, int quantidade, Reserva& reserva) = 0;
    /** @brief Cancela uma reserva pendente; retorna false se a operacao for recusada. */
    virtual bool cancelar(int idReserva) = 0;
    /** @brief Lista apenas reservas pertencentes ao consumidor autenticado. */
    virtual Colecao<Reserva> minhasReservas() const = 0;
protected:
    /** @brief Construtor padrao, acessivel somente as implementacoes. */
    IServicoReservas() = default;
    /** @brief Construtor de copia, acessivel somente as implementacoes. */
    IServicoReservas(const IServicoReservas& outro) = default;
    /** @brief Atribuicao por copia, acessivel somente as implementacoes. */
    IServicoReservas& operator=(const IServicoReservas& outro) = default;
};

/** @brief Contrato de consulta e confirmacao de retirada (SB05). */
class SAVEFOOD_SERVICOS_API IServicoRetiradas {
public:
    /** @brief Destrutor virtual. */
    virtual ~IServicoRetiradas();
    /** @brief Lista reservas do estabelecimento vinculado ao gerente. */
    virtual Colecao<Reserva> listar() const = 0;
    /** @brief Confirma a retirada de uma reserva do restaurante autorizado. */
    virtual bool confirmar(int idReserva, const DadosRetirada& dados) = 0;
protected:
    /** @brief Construtor padrao, acessivel somente as implementacoes. */
    IServicoRetiradas() = default;
    /** @brief Construtor de copia, acessivel somente as implementacoes. */
    IServicoRetiradas(const IServicoRetiradas& outro) = default;
    /** @brief Atribuicao por copia, acessivel somente as implementacoes. */
    IServicoRetiradas& operator=(const IServicoRetiradas& outro) = default;
};

/** @brief Contrato de consulta de indicadores do restaurante (SB06). */
class SAVEFOOD_SERVICOS_API IServicoIndicadores {
public:
    /** @brief Destrutor virtual. */
    virtual ~IServicoIndicadores();
    /** @brief Soma peso e receita de retiradas confirmadas do restaurante autorizado. */
    virtual ResumoIndicadores obterResumo() const = 0;
protected:
    /** @brief Construtor padrao, acessivel somente as implementacoes. */
    IServicoIndicadores() = default;
    /** @brief Construtor de copia, acessivel somente as implementacoes. */
    IServicoIndicadores(const IServicoIndicadores& outro) = default;
    /** @brief Atribuicao por copia, acessivel somente as implementacoes. */
    IServicoIndicadores& operator=(const IServicoIndicadores& outro) = default;
};

/** @brief Fabrica da camada de servicos: unico ponto de criacao dos servicos.
 *
 * Os servicos nao possuem o sistema: o ISistemaSaveFood recebido deve
 * continuar existindo enquanto os servicos criados forem usados.
 */
class SAVEFOOD_SERVICOS_API FabricaServicos {
public:
    /** @brief A fabrica so possui metodos estaticos; nao deve ser instanciada. */
    FabricaServicos() = delete;

    /** @brief Cria o servico de autenticacao ligado ao sistema informado. */
    static std::unique_ptr<IServicoAutenticacao> criarAutenticacao(ISistemaSaveFood& sistema);
    /** @brief Cria o servico de verificacao de acesso ligado ao sistema informado. */
    static std::unique_ptr<IServicoAcesso> criarAcesso(const ISistemaSaveFood& sistema);
    /** @brief Cria o servico de ofertas que atua com a sessao informada. */
    static std::unique_ptr<IServicoOfertas> criarOfertas(ISistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Cria o servico de reservas que atua com a sessao informada. */
    static std::unique_ptr<IServicoReservas> criarReservas(ISistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Cria o servico de retiradas que atua com a sessao informada. */
    static std::unique_ptr<IServicoRetiradas> criarRetiradas(ISistemaSaveFood& sistema, const Sessao& sessao);
    /** @brief Cria o servico de indicadores que atua com a sessao informada. */
    static std::unique_ptr<IServicoIndicadores> criarIndicadores(ISistemaSaveFood& sistema, const Sessao& sessao);
};

#endif
