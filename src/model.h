#ifndef SAVEFOOD_MODEL_H
#define SAVEFOOD_MODEL_H

#include <cstddef>
#include <string>

enum class Perfil { Gerente, Consumidor };
using Papel = Perfil;

enum class EstadoReserva { Pendente, Retirada };

// Restaurante faz parte do dominio, mesmo que os dados sejam de demonstracao.
class Restaurante {
    int id_;
    std::string nome_;
public:
    Restaurante();
    Restaurante(int id, const std::string& nome);
    Restaurante(const Restaurante& outro);
    Restaurante& operator=(const Restaurante& outro);
    ~Restaurante();

    int id() const;
    const std::string& nome() const;
};

// Forma canonica: construtor, copia, atribuicao e destrutor.
class Usuario {
    int id_;
    std::string email_;
    std::size_t resumoSenha_;
    Perfil perfil_;
    int idRestaurante_;
public:
    Usuario();
    Usuario(int id, const std::string& email, const std::string& senha,
            Perfil perfil, int idRestaurante);
    Usuario(const Usuario& outro);
    Usuario& operator=(const Usuario& outro);
    ~Usuario();

    int id() const;
    const std::string& email() const;
    Perfil perfil() const;
    int idRestaurante() const;
    bool conferirSenha(const std::string& senha) const;
};

class SistemaSaveFood;
class Sessao {
    friend class SistemaSaveFood;
    int codigo_;
    int idUsuario_;
    bool ativa_;
    Sessao(int codigo, int idUsuario);
public:
    Sessao();
    Sessao(const Sessao& outra);
    Sessao& operator=(const Sessao& outra);
    ~Sessao();
    bool ativa() const;
    bool autenticada() const;
};

class Oferta {
    int id_;
    int idRestaurante_;
    std::string descricao_;
    int quantidadeTotal_;
    int quantidadeDisponivel_;
    double pesoTotalKg_;
    double precoUnitario_;
    std::string janelaRetirada_;
    bool ativa_;
public:
    Oferta();
    Oferta(int id, int idRestaurante, const std::string& descricao,
           int quantidade, double pesoTotalKg, double precoUnitario,
           const std::string& janelaRetirada);
    Oferta(const Oferta& outra);
    Oferta& operator=(const Oferta& outra);
    ~Oferta();

    bool valida() const;
    bool reservar(int quantidade);
    int id() const;
    int idRestaurante() const;
    const std::string& descricao() const;
    int quantidadeTotal() const;
    int quantidadeDisponivel() const;
    double pesoPorUnidadeKg() const;
    double precoUnitario() const;
    const std::string& janelaRetirada() const;
    bool ativa() const;
    bool estaAtiva() const;
};

class Reserva {
    int id_;
    int idOferta_;
    int idConsumidor_;
    int quantidadeReservada_;
    int quantidadeRetirada_;
    double valorRecebido_;
    bool retiradaConfirmada_;
public:
    Reserva();
    Reserva(int id, int idOferta, int idConsumidor, int quantidade);
    Reserva(const Reserva& outra);
    Reserva& operator=(const Reserva& outra);
    ~Reserva();

    bool confirmarRetirada(int quantidadeRetirada, double valorRecebido);
    int id() const;
    int idOferta() const;
    int idConsumidor() const;
    int quantidadeReservada() const;
    int quantidadeRetirada() const;
    double valorRecebido() const;
    bool retiradaConfirmada() const;
    EstadoReserva situacao() const;
};

struct ResumoIndicadores {
    double pesoAproveitadoKg = 0.0;
    double receitaBrutaReais = 0.0;

    ResumoIndicadores() = default;
    ResumoIndicadores(const ResumoIndicadores& outro) = default;
    ResumoIndicadores& operator=(const ResumoIndicadores& outro) = default;
    ~ResumoIndicadores() = default;
};

#endif
