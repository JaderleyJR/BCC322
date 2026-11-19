# SaveFood - Sprint 4

Projeto em C++17 para a disciplina de Engenharia de Software da UFOP.
O codigo implementa o fluxo comercial minimo definido no backlog: acesso,
publicacao, consulta, reserva, cancelamento, confirmacao presencial e indicadores basicos.

## Arvore de diretorios

```text
SaveFood/
|-- bin/                       # Executaveis
|-- include/                   # Cabecalhos originais do produto
|   |-- model.h
|   |-- system.h
|   |-- services.h
|   `-- flow.h
|-- src/                       # Implementacoes originais
|   |-- main.cpp
|   |-- model.cpp
|   |-- system.cpp
|   |-- services.cpp
|   `-- flow.cpp
|-- test/
|   |-- funcional/
|   |   |-- main.cpp
|   |   |-- funcional_tests.h
|   |   `-- funcional_tests.cpp
|   `-- unit/
|       |-- main.cpp
|       |-- unit_tests.h
|       `-- unit_tests.cpp
|-- docs/
|   |-- graficos/              # Diagramas SVG e fontes DOT
|   `-- html/                  # Gerado pelo Doxygen
|-- Doxyfile
|-- Makefile
|-- compilar_windows.bat
|-- gerar_documentacao.bat
`-- README.md
```

## Modularizacao e tratamento de erros

Somente os quatro cabecalhos do produto foram movidos para `include/`.
Os cabecalhos de teste permanecem nos respectivos diretorios. A compilacao
usa `-Iinclude`, sem criar classes, funcoes ou assinaturas novas.

As operacoes existentes preservam seus retornos: `false` para recusas,
`0` para publicacao recusada, sessao inativa para login recusado, `-1` para
saldo inacessivel e colecoes vazias ou resumo zerado para consultas sem acesso.
As consultas vazias nao distinguem falta de acesso de ausencia de registros.
Foram reforcadas as validacoes de dados em branco, limites de IDs,
estoque e estados da reserva. A publicacao desfaz a insercao se houver uma
excecao durante a copia do resultado. A demonstracao informa a falha,
encerra as sessoes e o programa principal trata excecoes e retorna 1.
As verificacoes de acesso e os controles de concorrencia existentes foram mantidos.

## Sprint 4 - Doxygen e Graphviz

Os cabecalhos e fontes contem comentarios `@file`, `@brief`, `@param` e
`@return` nas operacoes, incluindo os testes. O arquivo `Doxyfile` configura
HTML, navegacao de fontes, graficos de colaboracao entre classes, hierarquia
(quando existir heranca) e dependencias de inclusao de arquivos.

### Windows

1. Instale o [Doxygen](https://www.doxygen.nl/download.html) e o
   [Graphviz](https://graphviz.org/download/).
2. Adicione os diretorios dos executaveis `doxygen.exe` e `dot.exe` ao PATH.
3. Abra o terminal na pasta SaveFood e execute `gerar_documentacao.bat`.
   O script gera e abre `docs/html/index.html` no navegador.

### Linux

Com Doxygen e Graphviz instalados, abra o terminal na pasta SaveFood:

```bash
make docs
```

Ou execute `doxygen Doxyfile`. Abra `docs/html/index.html` no navegador.

### Graficos incluidos

- `docs/graficos/dependencias.svg`: grafo dos includes internos do projeto.
- `docs/graficos/classes.svg`: estrutura das classes e relacoes existentes.

Os dois SVGs foram gerados separadamente pelo Graphviz e estao prontos para
visualizacao. Os arquivos `.dot` permitem reproduzir os graficos. Eles nao
substituem os graficos automaticos do Doxygen. Neste ambiente, o Doxygen nao
estava instalado e o download foi bloqueado; portanto, o HTML e os graficos
automaticos do Doxygen ainda precisam ser gerados localmente com o comando acima.

## Forma canonica

As classes concretas `Restaurante`, `Usuario`, `Sessao`, `Oferta`, `Reserva` e
`SistemaSaveFood` possuem construtor padrao, construtor de copia,
operador de atribuicao e destrutor. Em `SistemaSaveFood`, a copia dos
dados e protegida porque a classe possui um `std::mutex`.
As seis classes de servico tambem possuem as quatro operacoes canonicas.
Copias de servicos compartilham o sistema de origem, sem possuir sua memoria;
o sistema deve permanecer vivo enquanto os servicos o utilizam.

## Compilacao

No Linux com `g++`, abra o terminal dentro da pasta `SaveFood`:

```bash
make
make test
./bin/savefood
```

Os executaveis sao criados apenas em `bin/`. No Windows com MinGW,
execute `compilar_windows.bat` no terminal com o `g++` no PATH para
gerar os tres arquivos `.exe` em `bin/` e executar os testes.
O projeto nao depende de bibliotecas externas, somente de C++17 e threads.
Use uma versao do MinGW-w64 com suporte a C++17 e threads POSIX.
Os binarios sem extensao incluidos no ZIP foram compilados em Linux;
no Windows, gere os arquivos .exe com o script.

## API alinhada ao projeto da Sprint 2

`include/services.h` e `src/services.cpp` separam as operacoes entre as classes:

- `ServicoAutenticacao`: `entrar`, `sair`.
- `ServicoAcesso`: `permitir` (verificacao auxiliar; cada operacao tambem valida a sessao).
- `ServicoOfertas`: `publicar`, `consultar`, `detalhes`.
- `ServicoReservas`: `reservar`, `minhasReservas`, `cancelar`.
- `ServicoRetiradas`: `listar`, `confirmar`.
- `ServicoIndicadores`: `obterResumo`.

Os tipos `DadosOferta`, `FiltroOferta`, `DadosRetirada`, `Sessao`,
`Colecao<T>` e `ResumoIndicadores` seguem os contratos simplificados do
Projeto de API da Sprint 2. Todos os servicos recebem o mesmo
`SistemaSaveFood` para compartilhar os dados e uma `Sessao` para aplicar
o perfil de acesso. Os testes funcionais agora exercitam os servicos da API.

O metodo `detalhes` foi incluido para consultar uma oferta pelo ID.
Os dados continuam em memoria para facilitar a demonstracao da Sprint 3;
persistencia em banco ou arquivo sera uma etapa posterior.

## Testes funcionais

Cada fluxo e implementado em uma funcao separada, declarada em
`test/funcional/funcional_tests.h` e definida em
`test/funcional/funcional_tests.cpp`:

| Funcao | Caso de uso / PDF | CT-01 | CT-02 |
| --- | --- | --- | --- |
| `testeReservarQuantidade` | SB03, p. 6 | Reserva 2 de 5; saldo 3 e situacao Pendente. | Com saldo 1, reserva de 2 recusada sem novo registro. |
| `testeCancelarReserva` | SB04, p. 7 | Reserva propria de 2 cancelada; saldo 3 volta a 5. | Novo cancelamento recusado; saldo continua 5. |
| `testeConfirmarRetirada` | SB05, p. 8 | Gerente A confirma 2 unidades, 1 kg e R$ 24; situacao Retirada. | Gerente A nao confirma reserva do restaurante B; dados intactos. |

Cada funcao monta seu proprio sistema com dados de demonstracao. Os IDs sao
obtidos da API, em vez de depender dos IDs ilustrativos do PDF. As verificacoes
usam os servicos publicos, incluindo a consulta de saldo, sem alterar atributos
internos. Falhas retornam codigo 1 no main; sucesso retorna 0. As tres funcoes
sao executadas mesmo quando uma delas falha.

A escolha destes tres casos nao elimina os outros servicos do produto, mas
esta suite funcional nao executa separadamente SB01, SB02, SB06 ou o teste
concorrente da secao 3.3 do PDF.

Os testes unitarios verificam copia, atribuicao e validacoes das classes.
Os tres programas (`src/main.cpp`, `test/unit/main.cpp` e
`test/funcional/main.cpp`) sao compilados separadamente.

## Contas ficticias de demonstracao

| Papel | E-mail | Senha |
| --- | --- | --- |
| Gerente (restaurante 1) | `gerente@savefood.demo` | `gerente123` |
| Consumidor 1 | `consumidor@savefood.demo` | `consumidor123` |
| Consumidor 2 | `consumidor2@savefood.demo` | `consumidor123` |
| Gerente (restaurante 2) | `gerente2@savefood.demo` | `gerente123` |

## Limites desta versao

- Dados em memoria: o programa e uma demonstracao funcional, sem banco de dados.
- O hash simples de senha serve apenas para contas ficticias do exercicio;
  em producao deve ser substituido por Argon2id, scrypt ou PBKDF2.
- Pagamento e informado pelo gerente na retirada, sem integracao financeira.
- O peso efetivamente entregue e informado pelo gerente na confirmacao.
  Retiradas parciais sao recusadas enquanto a regra nao for definida.
- Doacoes, expiracao e cadastro aberto estao fora do MVP implementado.
- Dados de retirada sao armazenados na propria Reserva, como na arquitetura
  existente, sem uma classe Retirada separada.

Os testes usam exclusivamente dados de demonstracao.
