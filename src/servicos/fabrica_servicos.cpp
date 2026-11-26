/** @file fabrica_servicos.cpp
 * @brief Implementacao da FabricaServicos e dos destrutores das interfaces de servico.
 */
#include "services.h"

// Definidos aqui (e nao inline) para que as tabelas virtuais e as
// informacoes de tipo das interfaces fiquem dentro do componente savefood_servicos.
IServicoAutenticacao::~IServicoAutenticacao() = default;
IServicoAcesso::~IServicoAcesso() = default;
IServicoOfertas::~IServicoOfertas() = default;
IServicoReservas::~IServicoReservas() = default;
IServicoRetiradas::~IServicoRetiradas() = default;
IServicoIndicadores::~IServicoIndicadores() = default;

std::unique_ptr<IServicoAutenticacao> FabricaServicos::criarAutenticacao(ISistemaSaveFood& sistema) {
    return std::make_unique<ServicoAutenticacao>(sistema);
}

std::unique_ptr<IServicoAcesso> FabricaServicos::criarAcesso(const ISistemaSaveFood& sistema) {
    return std::make_unique<ServicoAcesso>(sistema);
}

std::unique_ptr<IServicoOfertas> FabricaServicos::criarOfertas(ISistemaSaveFood& sistema,
                                                               const Sessao& sessao) {
    return std::make_unique<ServicoOfertas>(sistema, sessao);
}

std::unique_ptr<IServicoReservas> FabricaServicos::criarReservas(ISistemaSaveFood& sistema,
                                                                 const Sessao& sessao) {
    return std::make_unique<ServicoReservas>(sistema, sessao);
}

std::unique_ptr<IServicoRetiradas> FabricaServicos::criarRetiradas(ISistemaSaveFood& sistema,
                                                                   const Sessao& sessao) {
    return std::make_unique<ServicoRetiradas>(sistema, sessao);
}

std::unique_ptr<IServicoIndicadores> FabricaServicos::criarIndicadores(ISistemaSaveFood& sistema,
                                                                       const Sessao& sessao) {
    return std::make_unique<ServicoIndicadores>(sistema, sessao);
}
