//
// Created by corre on 06/06/2026.
//

#ifndef PRODUTOS_H
#define PRODUTOS_H
#include "estruturas.h"
#include <stdio.h>




void entrar_etapa(Dados *dados);
void passar_etapa(Dados *dados, produto *p, int indice_vetor);
void passar_atividade(produto *p);
void iniciar(Dados *dados, char nome_arquivo[50]);
void criar_relatorio(Dados *dados, char nome_arquivo_saida[50], char arquivo_entrada[50]);
void avancar_tick(Dados *dados);
produto* iniciar_produto(int id, int tick_atual,char nome_produto[]);
void finalizar(Dados *dados);
void imprimir_dados_produto(FILE *f, produto *p, char status[], int tick_atual_simulacao);
void liberar_produto(produto *p);
void buscar_produto(Dados *dados, int id_buscado);

#endif //PRODUTOS_H