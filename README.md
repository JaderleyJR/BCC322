# SaveFood - Sprint 3

Projeto em C++17 para a disciplina de Engenharia de Software da UFOP.
O codigo implementa o fluxo comercial minimo definido no backlog: acesso,
publicacao, consulta, reserva, confirmacao presencial e indicadores basicos.

## Arvore de diretorios

```text
SaveFood/
|-- bin/
|   |-- savefood                (gerado na compilacao)
|   |-- testes_funcionais       (gerado na compilacao)
|   `-- testes_unitarios        (gerado na compilacao)
|-- src/
|   |-- main.cpp
|   |-- model.h
|   |-- model.cpp
|   |-- system.h
|   |-- system.cpp
|   |-- services.h
|   |-- services.cpp
|   |-- flow.h
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
|-- Makefile
|-- compilar_windows.bat
`-- README.md
```

## Forma canonica

As classes concretas `Restaurante`, `Usuario`, `Sessao`, `Oferta`, `Reserva` e
`SistemaSaveFood` possuem construtor padrao, construtor de copia,
operador de atribuicao e destrutor. Em `SistemaSaveFood`, a copia dos
dados e protegida porque a classe possui um `std::mutex`.

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

## API alinhada ao projeto da Sprint 2

`services.h` e `services.cpp` separam as operacoes entre as classes:

- `ServicoAutenticacao`: `entrar`, `sair`.
- `ServicoAcesso`: `permitir` (verificacao auxiliar; cada operacao tambem valida a sessao).
- `ServicoOfertas`: `publicar`, `consultar`, `detalhes`.
- `ServicoReservas`: `reservar`.
- `ServicoRetiradas`: `listar`, `confirmar`.
- `ServicoIndicadores`: `obterResumo`.

Os tipos `DadosOferta`, `FiltroOferta`, `ConfirmacaoRetirada`, `Sessao`,
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

1. `testePublicacaoEConsulta`: login, publicacao, catalogo e permissoes.
2. `testeReservasConcorrentes`: duas reservas simultaneas do mesmo lote;
   no maximo uma pode ser aceita quando o saldo nao basta para as duas.
3. `testeRetiradaEIndicadores`: retirada/pagamento, regras entre restaurantes
   e calculo de kg e receita sem contar reservas pendentes.

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
- O peso e informado manualmente no cadastro e distribuido proporcionalmente
  entre as unidades quando ocorre retirada parcial.
- Doacoes, cancelamento de reservas, expiracao e cadastro aberto estao fora
  do MVP implementado.

Os testes usam exclusivamente dados de demonstracao.
