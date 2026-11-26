/** @file model.h
 * @brief Classes do dominio, perfis e estados.
 */
#ifndef SAVEFOOD_MODEL_H
#define SAVEFOOD_MODEL_H

#include "../exportacao.h"

#include <cstddef>
#include <string>

/** @brief Perfis de acesso ao sistema. */
enum class Perfil { Gerente, Consumidor };
using Papel = Perfil;

/** @brief Estados de uma reserva: pendente, cancelada ou retirada. */
enum class EstadoReserva { Pendente, Cancelada, Retirada };

// Restaurante faz parte do dominio, mesmo que os dados sejam de demonstracao.
/** @brief Estabelecimento responsavel pelas ofertas comerciais. */
class SAVEFOOD_DOMINIO_API Restaurante {
    /** @brief Identificador do registro. */
    int id_;
    /** @brief Nome do restaurante. */
    std::string nome_;
public:
    /** @brief Construtor padrao. */
    Restaurante();
    /** @brief Constroi o objeto com os dados informados. */
    Restaurante(int id, const std::string& nome);
    /** @brief Construtor de copia. */
    Restaurante(const Restaurante& outro);
    /** @brief Operador de atribuicao por copia. */
    Restaurante& operator=(const Restaurante& outro);
    /** @brief Destrutor. */
    ~Restaurante();

    /** @brief Retorna o identificador do registro. */
    int id() const;
    /** @brief Retorna o nome do restaurante. */
    const std::string& nome() const;
};

// Forma canonica: construtor, copia, atribuicao e destrutor.
/** @brief Conta de gerente ou consumidor, com credenciais de demonstracao. */
class SAVEFOOD_DOMINIO_API Usuario {
    /** @brief Identificador do registro. */
    int id_;
    /** @brief Endereco de email da conta. */
    std::string email_;
    /** @brief Hash ilustrativo da senha de demonstracao. */
    std::size_t resumoSenha_;
    /** @brief Perfil de acesso. */
    Perfil perfil_;
    /** @brief Identificador do restaurante de vinculo. */
    int idRestaurante_;
public:
    /** @brief Construtor padrao. */
    Usuario();
    /** @brief Constroi o objeto com os dados informados. */
    Usuario(int id, const std::string& email, const std::string& senha,
            Perfil perfil, int idRestaurante);
    /** @brief Construtor de copia. */
    Usuario(const Usuario& outro);
    /** @brief Operador de atribuicao por copia. */
    Usuario& operator=(const Usuario& outro);
    /** @brief Destrutor. */
    ~Usuario();

    /** @brief Retorna o identificador do registro. */
    int id() const;
    /** @brief Retorna o email da conta. */
    const std::string& email() const;
    /** @brief Retorna o perfil do usuario. */
    Perfil perfil() const;
    /** @brief Retorna o identificador do restaurante de vinculo. */
    int idRestaurante() const;
    /** @brief Compara a senha informada com o hash de demonstracao. */
    bool conferirSenha(const std::string& senha) const;
};

class SistemaSaveFood;
/** @brief Identificacao temporaria de um usuario autenticado. */
class SAVEFOOD_DOMINIO_API Sessao {
    friend class SistemaSaveFood;
    /** @brief Codigo da sessao. */
    int codigo_;
    /** @brief Usuario associado a sessao. */
    int idUsuario_;
    /** @brief Indica se o registro esta ativo. */
    bool ativa_;
    /** @brief Constroi o objeto com os dados informados. */
    Sessao(int codigo, int idUsuario);
public:
    /** @brief Construtor padrao. */
    Sessao();
    /** @brief Construtor de copia. */
    Sessao(const Sessao& outra);
    /** @brief Operador de atribuicao por copia. */
    Sessao& operator=(const Sessao& outra);
    /** @brief Destrutor. */
    ~Sessao();
    /** @brief Informa se o objeto esta marcado como ativo. */
    bool ativa() const;
    /** @brief Informa o estado local da sessao; o sistema valida seu codigo nas operacoes. */
    bool autenticada() const;
};

/** @brief Lote comercial com quantidade, peso, preco e janela de retirada. */
class SAVEFOOD_DOMINIO_API Oferta {
    /** @brief Identificador do registro. */
    int id_;
    /** @brief Identificador do restaurante de vinculo. */
    int idRestaurante_;
    /** @brief Descricao do alimento anunciado. */
    std::string descricao_;
    /** @brief Quantidade inicial do lote. */
    int quantidadeTotal_;
    /** @brief Unidades ainda disponiveis para reserva. */
    int quantidadeDisponivel_;
    /** @brief Peso total anunciado em quilogramas. */
    double pesoTotalKg_;
    /** @brief Preco de uma unidade em reais. */
    double precoUnitario_;
    /** @brief Horario previsto para a retirada. */
    std::string janelaRetirada_;
    /** @brief Indica se o registro esta ativo. */
    bool ativa_;
public:
    /** @brief Construtor padrao. */
    Oferta();
    /** @brief Constroi o objeto com os dados informados. */
    Oferta(int id, int idRestaurante, const std::string& descricao,
           int quantidade, double pesoTotalKg, double precoUnitario,
           const std::string& janelaRetirada);
    /** @brief Construtor de copia. */
    Oferta(const Oferta& outra);
    /** @brief Operador de atribuicao por copia. */
    Oferta& operator=(const Oferta& outra);
    /** @brief Destrutor. */
    ~Oferta();

    /** @brief Verifica identificadores, quantidade, peso, preco, descricao e janela. */
    bool valida() const;
    /** @brief Solicita reserva de unidades; retorna false se a operacao for recusada.
     * @param quantidade Unidades solicitadas.
     * @return true se o saldo foi reduzido; false sem alteracao.
     */
    bool reservar(int quantidade);
    /** @brief Devolve unidades ao estoque sem exceder a quantidade inicial.
     * @param quantidade Unidades devolvidas.
     * @return true se o saldo foi recomposto; false sem alteracao.
     */
    bool restaurarDisponibilidade(int quantidade);
    /** @brief Retorna o identificador do registro. */
    int id() const;
    /** @brief Retorna o identificador do restaurante de vinculo. */
    int idRestaurante() const;
    /** @brief Retorna a descricao da oferta. */
    const std::string& descricao() const;
    /** @brief Retorna a quantidade inicial do lote. */
    int quantidadeTotal() const;
    /** @brief Retorna o saldo atual do lote. */
    int quantidadeDisponivel() const;
    /** @brief Retorna a estimativa de peso por unidade, em kg. */
    double pesoPorUnidadeKg() const;
    /** @brief Retorna o preco unitario em reais. */
    double precoUnitario() const;
    /** @brief Retorna o horario de retirada. */
    const std::string& janelaRetirada() const;
    /** @brief Informa se o objeto esta marcado como ativo. */
    bool ativa() const;
    /** @brief Informa se a oferta esta ativa e possui saldo positivo. */
    bool estaAtiva() const;
};

/** @brief Reserva de unidades de uma oferta e registro da eventual retirada. */
class SAVEFOOD_DOMINIO_API Reserva {
    /** @brief Identificador do registro. */
    int id_;
    /** @brief Oferta associada a reserva. */
    int idOferta_;
    /** @brief Titular da reserva. */
    int idConsumidor_;
    /** @brief Unidades solicitadas. */
    int quantidadeReservada_;
    /** @brief Unidades efetivamente entregues. */
    int quantidadeRetirada_;
    /** @brief Valor recebido em reais. */
    double valorRecebido_;
    /** @brief Peso efetivamente entregue em quilogramas. */
    double pesoAproveitadoKg_;
    /** @brief Estado atual da reserva. */
    EstadoReserva situacao_;
public:
    /** @brief Construtor padrao. */
    Reserva();
    /** @brief Constroi o objeto com os dados informados. */
    Reserva(int id, int idOferta, int idConsumidor, int quantidade);
    /** @brief Construtor de copia. */
    Reserva(const Reserva& outra);
    /** @brief Operador de atribuicao por copia. */
    Reserva& operator=(const Reserva& outra);
    /** @brief Destrutor. */
    ~Reserva();

    /** @brief Cancela uma reserva pendente; retorna false se a operacao for recusada. */
    bool cancelar();
    /** @brief Registra uma retirada integral, com peso e valor positivos e finitos.
     * @param quantidadeRetirada Unidades entregues, iguais a quantidade reservada.
     * @param pesoAproveitadoKg Peso efetivo em kg.
     * @param valorRecebido Valor pago em reais.
     * @return true se confirmada; false sem alterar a reserva.
     */
    bool confirmarRetirada(int quantidadeRetirada, double pesoAproveitadoKg,
                           double valorRecebido);
    /** @brief Retorna o identificador do registro. */
    int id() const;
    /** @brief Retorna a oferta associada. */
    int idOferta() const;
    /** @brief Retorna o titular da reserva. */
    int idConsumidor() const;
    /** @brief Retorna a quantidade reservada. */
    int quantidadeReservada() const;
    /** @brief Retorna a quantidade entregue. */
    int quantidadeRetirada() const;
    /** @brief Retorna o valor recebido em reais. */
    double valorRecebido() const;
    /** @brief Retorna o peso entregue em kg. */
    double pesoAproveitadoKg() const;
    /** @brief Informa se a reserva foi retirada. */
    bool retiradaConfirmada() const;
    /** @brief Retorna o estado da reserva. */
    EstadoReserva situacao() const;
};

/** @brief Totais de peso e receita de retiradas confirmadas. */
struct SAVEFOOD_DOMINIO_API ResumoIndicadores {
    /** @brief Peso aproveitado em kg. */
    double pesoAproveitadoKg = 0.0;
    /** @brief Receita bruta em reais. */
    double receitaBrutaReais = 0.0;

    /** @brief Construtor padrao. */
    ResumoIndicadores() = default;
    /** @brief Construtor de copia. */
    ResumoIndicadores(const ResumoIndicadores& outro) = default;
    /** @brief Operador de atribuicao por copia. */
    ResumoIndicadores& operator=(const ResumoIndicadores& outro) = default;
    /** @brief Destrutor. */
    ~ResumoIndicadores() = default;
};

#endif
