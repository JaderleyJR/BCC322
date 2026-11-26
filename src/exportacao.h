/** @file exportacao.h
 * @brief Macros de exportacao dos componentes (bibliotecas compartilhadas).
 *
 * Cada camada do produto e compilada como uma biblioteca compartilhada
 * (.dll no Windows, .so no Linux). Somente o que for marcado com a macro
 * da camada fica visivel para as camadas superiores; o restante, como as
 * funcoes em namespace anonimo, permanece interno ao componente.
 *
 * Ao compilar uma biblioteca, o Makefile define SAVEFOOD_<CAMADA>_EXPORTA.
 * Quem usa a biblioteca nao define a macro e passa a importar os simbolos.
 *
 * | Camada     | Biblioteca         | Macro                     |
 * | ---------- | ------------------ | ------------------------- |
 * | Dominio    | savefood_dominio   | SAVEFOOD_DOMINIO_API      |
 * | Nucleo     | savefood_nucleo    | SAVEFOOD_NUCLEO_API       |
 * | Servicos   | savefood_servicos  | SAVEFOOD_SERVICOS_API     |
 */
#ifndef SAVEFOOD_EXPORTACAO_H
#define SAVEFOOD_EXPORTACAO_H

#if defined(_WIN32) || defined(__CYGWIN__)
    #define SAVEFOOD_EXPORTA __declspec(dllexport)
    #define SAVEFOOD_IMPORTA __declspec(dllimport)
#else
    #define SAVEFOOD_EXPORTA __attribute__((visibility("default")))
    #define SAVEFOOD_IMPORTA __attribute__((visibility("default")))
#endif

#ifdef SAVEFOOD_DOMINIO_EXPORTA
    #define SAVEFOOD_DOMINIO_API SAVEFOOD_EXPORTA
#else
    #define SAVEFOOD_DOMINIO_API SAVEFOOD_IMPORTA
#endif

#ifdef SAVEFOOD_NUCLEO_EXPORTA
    #define SAVEFOOD_NUCLEO_API SAVEFOOD_EXPORTA
#else
    #define SAVEFOOD_NUCLEO_API SAVEFOOD_IMPORTA
#endif

#ifdef SAVEFOOD_SERVICOS_EXPORTA
    #define SAVEFOOD_SERVICOS_API SAVEFOOD_EXPORTA
#else
    #define SAVEFOOD_SERVICOS_API SAVEFOOD_IMPORTA
#endif

#endif
