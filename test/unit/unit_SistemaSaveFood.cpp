/** @file unit_SistemaSaveFood.cpp
 * @brief Testes unitarios da classe SistemaSaveFood.
 *
 * Os metodos privados prepararContasDemo() e usuarioDaSessaoSemLock() nao
 * sao acessiveis aos testes; sao exercitados pelo construtor e por todas
 * as operacoes que validam sessao.
 */
#include <assert.h>
#include <atomic>
#include <cmath>
#include <thread>
#include "unit_SistemaSaveFood.h"
#include "../../src/system.h"

namespace {
const char* EMAIL_GERENTE_A = "gerente@savefood.demo";
const char* EMAIL_GERENTE_B = "gerente2@savefood.demo";
const char* EMAIL_CONSUMIDOR_1 = "consumidor@savefood.demo";
const char* EMAIL_CONSUMIDOR_2 = "consumidor2@savefood.demo";
const char* SENHA_GERENTE = "gerente123";
const char* SENHA_CONSUMIDOR = "consumidor123";

bool quase(double a, double b) { return std::fabs(a - b) < 1e-9; }

/** @brief Publica a oferta padrao do projeto da API (5 marmitas, 2,5 kg, R$ 12). */
int publicarMarmita(SistemaSaveFood& s, const Sessao& gerente, int quantidade = 5) {
    return s.publicarOferta(gerente, "Marmita", quantidade, 2.5, 12.0, "18h-19h");
}
}

void unit_SistemaSaveFood_constructor(void) {
    SistemaSaveFood s;
    assert(s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE).ativa());
    assert(s.entrar(EMAIL_GERENTE_B, SENHA_GERENTE).ativa());
    assert(s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR).ativa());
    assert(s.entrar(EMAIL_CONSUMIDOR_2, SENHA_CONSUMIDOR).ativa());
    // Nenhuma oferta existe no sistema recem-criado.
    Sessao c = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    assert(s.consultarOfertas(c).empty());
}

void unit_SistemaSaveFood_copyConstructor(void) {
    SistemaSaveFood original;
    Sessao g = original.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao c = original.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    int oferta = publicarMarmita(original, g, 4);
    assert(oferta > 0);

    SistemaSaveFood copia(original);
    int idReserva = 0;
    assert(copia.reservar(c, oferta, 2, idReserva));
    assert(copia.saldoOferta(g, oferta) == 2);
    assert(original.saldoOferta(g, oferta) == 4); // original intacto
}

void unit_SistemaSaveFood_assignmentOperator(void) {
    SistemaSaveFood original;
    Sessao g = original.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao c = original.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    int oferta = publicarMarmita(original, g, 4);

    SistemaSaveFood atribuido;
    atribuido = original;
    int idReserva = 0;
    assert(atribuido.reservar(c, oferta, 3, idReserva));
    assert(atribuido.saldoOferta(g, oferta) == 1);
    assert(original.saldoOferta(g, oferta) == 4);

    atribuido = atribuido; // autoatribuicao nao trava nem perde dados
    assert(atribuido.saldoOferta(g, oferta) == 1);
}

void unit_SistemaSaveFood_destructor(void) {
    SistemaSaveFood original;
    Sessao g = original.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    int oferta = publicarMarmita(original, g);
    SistemaSaveFood* copia = new SistemaSaveFood(original);
    delete copia;
    assert(original.saldoOferta(g, oferta) == 5);
}

void unit_SistemaSaveFood_entrar(void) {
    SistemaSaveFood s;
    Sessao valida = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    assert(valida.ativa());
    assert(!s.entrar(EMAIL_CONSUMIDOR_1, "errada").ativa());
    assert(!s.entrar("naoexiste@savefood.demo", SENHA_CONSUMIDOR).ativa());
    assert(!s.entrar("", SENHA_CONSUMIDOR).ativa());
    assert(!s.entrar(EMAIL_CONSUMIDOR_1, "").ativa());
    // Dois logins do mesmo usuario geram sessoes distintas e validas.
    Sessao segunda = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    assert(segunda.ativa());
    assert(s.permitir(valida, Perfil::Consumidor) && s.permitir(segunda, Perfil::Consumidor));
}

void unit_SistemaSaveFood_sair(void) {
    SistemaSaveFood s;
    Sessao c = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    Sessao copiaAntiga = c;
    assert(s.sair(c));
    assert(!c.ativa());
    assert(!s.sair(c));                               // sair duas vezes
    assert(!s.permitir(copiaAntiga, Perfil::Consumidor)); // copia antiga invalidada no sistema
    Sessao vazia;
    assert(!s.sair(vazia));
}

void unit_SistemaSaveFood_publicarOferta(void) {
    SistemaSaveFood s;
    Sessao g = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao c = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);

    // SB01 CT-01: dados validos.
    Oferta publicada;
    int id = s.publicarOferta(g, "Marmita", 5, 2.5, 12.0, "18h-19h", &publicada);
    assert(id > 0);
    assert(publicada.id() == id && publicada.idRestaurante() == 1);
    assert(publicada.quantidadeDisponivel() == 5 && publicada.ativa());

    // SB01 CT-02: janela vazia nao cria lote.
    assert(s.publicarOferta(g, "Marmita", 5, 2.5, 12.0, "") == 0);
    // Outros dados invalidos e perfil errado.
    assert(s.publicarOferta(g, "", 5, 2.5, 12.0, "18h-19h") == 0);
    assert(s.publicarOferta(g, "Marmita", 0, 2.5, 12.0, "18h-19h") == 0);
    assert(s.publicarOferta(g, "Marmita", 5, 2.5, 0.0, "18h-19h") == 0);
    assert(s.publicarOferta(c, "Marmita", 5, 2.5, 12.0, "18h-19h") == 0);
    assert(s.publicarOferta(Sessao(), "Marmita", 5, 2.5, 12.0, "18h-19h") == 0);

    // Recusas nao consomem IDs: a proxima publicacao valida recebe id + 1.
    assert(publicarMarmita(s, g) == id + 1);
}

void unit_SistemaSaveFood_consultarOfertas(void) {
    SistemaSaveFood s;
    Sessao g = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao c1 = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    Sessao c2 = s.entrar(EMAIL_CONSUMIDOR_2, SENHA_CONSUMIDOR);
    int marmita = publicarMarmita(s, g);
    int esgotada = s.publicarOferta(g, "Pao de queijo", 1, 0.2, 3.0, "8h-9h");
    int idReserva = 0;
    assert(s.reservar(c2, esgotada, 1, idReserva));

    // SB02 CT-01: so ofertas ativas e com saldo aparecem.
    auto todas = s.consultarOfertas(c1);
    assert(todas.size() == 1 && todas.front().id() == marmita);
    // Pesquisa sem diferenciar maiusculas.
    assert(s.consultarOfertas(c1, "MARMITA").size() == 1);
    // SB02 CT-02: termo sem correspondencia retorna lista vazia sem alterar saldo.
    assert(s.consultarOfertas(c1, "sopa").empty());
    assert(s.saldoOferta(g, marmita) == 5);
    // Gerente e sessao invalida nao consultam o catalogo.
    assert(s.consultarOfertas(g).empty());
    assert(s.consultarOfertas(Sessao()).empty());
}

void unit_SistemaSaveFood_detalhesOferta(void) {
    SistemaSaveFood s;
    Sessao gA = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao gB = s.entrar(EMAIL_GERENTE_B, SENHA_GERENTE);
    Sessao c = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    int oferta = publicarMarmita(s, gA, 2);

    Oferta encontrada;
    assert(s.detalhesOferta(c, oferta, encontrada));
    assert(encontrada.descricao() == "Marmita" && quase(encontrada.precoUnitario(), 12.0));
    assert(s.detalhesOferta(gA, oferta, encontrada));   // gerente dono
    assert(!s.detalhesOferta(gB, oferta, encontrada));  // gerente de outro restaurante
    assert(!s.detalhesOferta(c, 999, encontrada));      // inexistente

    int idReserva = 0;
    assert(s.reservar(c, oferta, 2, idReserva));
    assert(!s.detalhesOferta(c, oferta, encontrada));   // esgotada: consumidor nao ve
    assert(s.detalhesOferta(gA, oferta, encontrada));   // dono continua vendo
    assert(encontrada.quantidadeDisponivel() == 0);
}

void unit_SistemaSaveFood_reservar(void) {
    SistemaSaveFood s;
    Sessao g = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao c = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    int oferta = publicarMarmita(s, g);

    // SB03 CT-01: reserva 2 de 5.
    int idReserva = 0;
    Reserva criada;
    assert(s.reservar(c, oferta, 2, idReserva, &criada));
    assert(idReserva > 0 && criada.id() == idReserva);
    assert(criada.situacao() == EstadoReserva::Pendente);
    assert(s.saldoOferta(g, oferta) == 3);

    // Recusas: quantidade invalida, oferta inexistente, gerente reservando.
    int idRecusada = -7;
    assert(!s.reservar(c, oferta, 0, idRecusada));
    assert(!s.reservar(c, oferta, -1, idRecusada));
    assert(!s.reservar(c, 999, 1, idRecusada));
    assert(!s.reservar(g, oferta, 1, idRecusada));
    assert(idRecusada == -7); // saida nao e alterada em caso de recusa

    // SB03 CT-02: saldo 1, pedido de 2.
    assert(s.reservar(c, oferta, 2, idReserva));
    assert(s.saldoOferta(g, oferta) == 1);
    assert(!s.reservar(c, oferta, 2, idReserva));
    assert(s.saldoOferta(g, oferta) == 1);
    assert(s.minhasReservas(c).size() == 2);

    // Secao 3.3: oferta com 4 unidades, dois consumidores pedem 3 ao mesmo tempo.
    for (int rodada = 0; rodada < 200; ++rodada) {
        SistemaSaveFood sc;
        Sessao gc = sc.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
        Sessao c1 = sc.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
        Sessao c2 = sc.entrar(EMAIL_CONSUMIDOR_2, SENHA_CONSUMIDOR);
        int disputada = publicarMarmita(sc, gc, 4);
        std::atomic<bool> largada(false);
        bool ok1 = false, ok2 = false;
        int id1 = 0, id2 = 0;
        std::thread t1([&] { while (!largada) {} ok1 = sc.reservar(c1, disputada, 3, id1); });
        std::thread t2([&] { while (!largada) {} ok2 = sc.reservar(c2, disputada, 3, id2); });
        largada = true;
        t1.join();
        t2.join();
        assert(ok1 != ok2);                         // exatamente uma aceita
        assert(sc.saldoOferta(gc, disputada) == 1); // nunca negativo
        assert(sc.listarReservas(gc).size() == 1);  // sem reserva parcial
        // O vencedor cancela: 3 unidades voltam exatamente uma vez.
        const Sessao& vencedor = ok1 ? c1 : c2;
        const int idVencedor = ok1 ? id1 : id2;
        assert(sc.cancelarReserva(vencedor, idVencedor));
        assert(!sc.cancelarReserva(vencedor, idVencedor));
        assert(sc.saldoOferta(gc, disputada) == 4);
    }
}

void unit_SistemaSaveFood_cancelarReserva(void) {
    SistemaSaveFood s;
    Sessao g = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao c1 = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    Sessao c2 = s.entrar(EMAIL_CONSUMIDOR_2, SENHA_CONSUMIDOR);
    int oferta = publicarMarmita(s, g);
    int idReserva = 0;
    assert(s.reservar(c1, oferta, 2, idReserva));

    assert(!s.cancelarReserva(c2, idReserva));  // terceiro
    assert(!s.cancelarReserva(g, idReserva));   // gerente
    assert(!s.cancelarReserva(c1, 999));        // inexistente
    assert(s.saldoOferta(g, oferta) == 3);

    // SB04 CT-01 e CT-02.
    assert(s.cancelarReserva(c1, idReserva));
    assert(s.saldoOferta(g, oferta) == 5);
    assert(!s.cancelarReserva(c1, idReserva));
    assert(s.saldoOferta(g, oferta) == 5);

    // Reserva ja retirada nao pode ser cancelada.
    int idRetirada = 0;
    assert(s.reservar(c1, oferta, 1, idRetirada));
    assert(s.confirmarRetirada(g, idRetirada, 1, 0.5, 12.0));
    assert(!s.cancelarReserva(c1, idRetirada));
}

void unit_SistemaSaveFood_minhasReservas(void) {
    SistemaSaveFood s;
    Sessao g = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao c1 = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    Sessao c2 = s.entrar(EMAIL_CONSUMIDOR_2, SENHA_CONSUMIDOR);
    int oferta = publicarMarmita(s, g);
    int id1 = 0, id2 = 0;
    assert(s.reservar(c1, oferta, 1, id1));
    assert(s.reservar(c2, oferta, 1, id2));

    auto minhas = s.minhasReservas(c1);
    assert(minhas.size() == 1 && minhas.front().id() == id1);
    assert(s.cancelarReserva(c1, id1));
    minhas = s.minhasReservas(c1);
    assert(minhas.size() == 1 && minhas.front().situacao() == EstadoReserva::Cancelada);
    assert(s.minhasReservas(g).empty());
    assert(s.minhasReservas(Sessao()).empty());
}

void unit_SistemaSaveFood_permitir(void) {
    SistemaSaveFood s;
    Sessao g = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao c = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    assert(s.permitir(g, Perfil::Gerente));
    assert(s.permitir(g, Perfil::Gerente, 1));
    assert(!s.permitir(g, Perfil::Gerente, 2));
    assert(!s.permitir(g, Perfil::Consumidor));
    assert(s.permitir(c, Perfil::Consumidor));
    assert(!s.permitir(c, Perfil::Consumidor, 1)); // consumidor nao tem restaurante
    assert(!s.permitir(Sessao(), Perfil::Consumidor));
}

void unit_SistemaSaveFood_listarReservas(void) {
    SistemaSaveFood s;
    Sessao gA = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao gB = s.entrar(EMAIL_GERENTE_B, SENHA_GERENTE);
    Sessao c = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    int ofertaA = publicarMarmita(s, gA);
    int ofertaB = publicarMarmita(s, gB);
    int idA = 0, idB = 0;
    assert(s.reservar(c, ofertaA, 1, idA));
    assert(s.reservar(c, ofertaB, 2, idB));

    auto listaA = s.listarReservas(gA);
    auto listaB = s.listarReservas(gB);
    assert(listaA.size() == 1 && listaA.front().id() == idA);
    assert(listaB.size() == 1 && listaB.front().id() == idB);
    assert(s.listarReservas(c).empty());
}

void unit_SistemaSaveFood_confirmarRetirada(void) {
    SistemaSaveFood s;
    Sessao gA = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao gB = s.entrar(EMAIL_GERENTE_B, SENHA_GERENTE);
    Sessao c = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    int oferta = publicarMarmita(s, gA);
    int id = 0;
    assert(s.reservar(c, oferta, 2, id));

    assert(!s.confirmarRetirada(gB, id, 2, 1.0, 24.0));  // outro restaurante
    assert(!s.confirmarRetirada(c, id, 2, 1.0, 24.0));   // consumidor
    assert(!s.confirmarRetirada(gA, 999, 2, 1.0, 24.0)); // inexistente
    assert(!s.confirmarRetirada(gA, id, 1, 0.5, 12.0));  // parcial
    assert(s.confirmarRetirada(gA, id, 2, 1.0, 24.0));
    assert(!s.confirmarRetirada(gA, id, 2, 1.0, 24.0));  // duplicada
    assert(s.listarReservas(gA).front().situacao() == EstadoReserva::Retirada);
}

void unit_SistemaSaveFood_obterResumo(void) {
    SistemaSaveFood s;
    Sessao gA = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao gB = s.entrar(EMAIL_GERENTE_B, SENHA_GERENTE);
    Sessao c = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    int oferta = publicarMarmita(s, gA);
    int retirada = 0, pendente = 0;
    assert(s.reservar(c, oferta, 2, retirada));
    assert(s.reservar(c, oferta, 1, pendente));

    ResumoIndicadores vazio = s.obterResumo(gA);
    assert(quase(vazio.pesoAproveitadoKg, 0.0) && quase(vazio.receitaBrutaReais, 0.0));

    assert(s.confirmarRetirada(gA, retirada, 2, 1.0, 24.0));
    ResumoIndicadores resumoA = s.obterResumo(gA);
    assert(quase(resumoA.pesoAproveitadoKg, 1.0));   // so retiradas confirmadas
    assert(quase(resumoA.receitaBrutaReais, 24.0));
    ResumoIndicadores resumoB = s.obterResumo(gB);
    assert(quase(resumoB.pesoAproveitadoKg, 0.0));   // outro restaurante
    ResumoIndicadores doConsumidor = s.obterResumo(c);
    assert(quase(doConsumidor.receitaBrutaReais, 0.0));
}

void unit_SistemaSaveFood_saldoOferta(void) {
    SistemaSaveFood s;
    Sessao gA = s.entrar(EMAIL_GERENTE_A, SENHA_GERENTE);
    Sessao gB = s.entrar(EMAIL_GERENTE_B, SENHA_GERENTE);
    Sessao c = s.entrar(EMAIL_CONSUMIDOR_1, SENHA_CONSUMIDOR);
    int oferta = publicarMarmita(s, gA);
    assert(s.saldoOferta(gA, oferta) == 5);
    assert(s.saldoOferta(c, oferta) == 5);
    assert(s.saldoOferta(gB, oferta) == -1);       // gerente de outro restaurante
    assert(s.saldoOferta(gA, 999) == -1);          // inexistente
    assert(s.saldoOferta(Sessao(), oferta) == -1); // sem sessao
}

void run_unit_test_SistemaSaveFood(void) {
    unit_SistemaSaveFood_constructor();
    unit_SistemaSaveFood_copyConstructor();
    unit_SistemaSaveFood_assignmentOperator();
    unit_SistemaSaveFood_destructor();
    unit_SistemaSaveFood_entrar();
    unit_SistemaSaveFood_sair();
    unit_SistemaSaveFood_publicarOferta();
    unit_SistemaSaveFood_consultarOfertas();
    unit_SistemaSaveFood_detalhesOferta();
    unit_SistemaSaveFood_reservar();
    unit_SistemaSaveFood_cancelarReserva();
    unit_SistemaSaveFood_minhasReservas();
    unit_SistemaSaveFood_permitir();
    unit_SistemaSaveFood_listarReservas();
    unit_SistemaSaveFood_confirmarRetirada();
    unit_SistemaSaveFood_obterResumo();
    unit_SistemaSaveFood_saldoOferta();
}
