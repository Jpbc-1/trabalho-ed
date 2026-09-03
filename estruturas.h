//
// Created by corre on 06/06/2026.
//

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H


struct produto;

struct atividade
{
    int id;
    char nome[50];
    float taxa_falha;
    int tempo;
    struct atividade *prox;
    int total_produtos_saida;
    int soma_tempo_fila;
    int soma_tempo_total;
};
typedef struct atividade atividade;

struct no {
    struct produto *produto;
    struct no *prox;
};
typedef  struct no no;


struct etapa
{
    int id;
    char nome[50];
    int capacidade;
    int ocupacao_atual;
    int quantidade_atividade;
    float taxa_falha;
    struct produto **vector;
    no *fila_pronto;
    atividade *atividade;
    struct etapa *prox;
    struct etapa *ant;
    int total_falhas;
    int total_produtos_saida;
    int tempo_minimo;
    int tempo_maximo;
    int soma_tempo_total;
    int soma_tempo_fila;
};
typedef struct etapa etapa;


typedef struct NoHistorico {
    int id_etapa;
    int tentativa;
    int id_atividade;
    char nome_atividade[50];
    int tick_fila;
    int tick_inicio;
    int tick_fim;
    int status_ok;

    struct NoHistorico *prox;
    struct NoHistorico *ant;
} NoHistorico;


struct produto
{
    int id;
    char nome[50];
    int tempo_total_atividade;
    int tempo_restante;
    int tick_criacao;
    int tick_entrada_linha;
    int tick_saida_linha;
    int tick_espera;
    int tick_espera_temporario;
    int falhas;
    int tentativa_etapa;
    NoHistorico *topo;
    etapa *etapa_atual;
    atividade *atividade_atual;
};
typedef struct produto produto;

typedef struct noa{
    produto * produto;
    struct noa* dir;
    struct noa* esq;

}noa;

typedef struct Dados{
    char nome_simulacao[100];
    int semente;
    int limite_seg;
    int taxa_entrada;
    int meta_produtos;
    int falhas_totais;
    int tick_atual;
    int produtos_concluidos;
    int produtos_descartados;

    no *entrada_fabrica;
    etapa *primeira_etapa;
    etapa *ultima_etapa;
    no *lixeira;
    noa *arvore;

} Dados;



// FUNÇÕES ETAPAS:

etapa *iniciar_etapa(int id, char nome[], int capacidade,float taxa_falha, int quantidade_atividade);
void inserir_etapa(etapa **final,etapa *nova_etapa);
void mostrar_etapas(etapa *lista);
void mostrar_etapa_id(etapa *lista, int id);
void mostrar_etapa_nome(etapa *lista,char nome[50]);


// FUNÇÕES ATIVIDADES:

atividade *iniciar_atividade(int id, char nome[], int  tempo_ativ, float taxa_falha);
void inserir_atividade(atividade **lista, atividade *nova_atividade);
void mostrar_atividades(etapa *lista);
void mostrar_atividades_id(etapa *lista,int id_etapa);
void mostrar_atividades_nome(etapa *lista, char nome[50]);
void mostrar_atividade_id_id(etapa *lista,int id_atividade, int id_etapa);
void mostrar_atividade_id_nome(etapa *lista,int id_atividade, char nome[50]);
void mostrar_atividade_nome_id(etapa *lista,char nome[50], int id_etapa);
void mostrar_atividade_nome_nome(etapa *lista,char nome_atividade[50], char nome_etapa[50]);
void liberar_atividades(atividade **lista);

void liberar_linha(etapa **lista);
void mostrar_linha(etapa *lista);

// FUNÇÕES FILA:

void inserir_fila(no **fila,produto *produto);
void mostrar_fila(no *f);
produto* remover_fila(no **fila);
void liberar_fila(no **f);

//FUNÇÕES PILHA DESCARTE:

void inserir_pilha_d(no** pilha,produto *produto);


//FUNÇÕES PILHA HISTORICO:

NoHistorico* iniciar_pilha_h(int id_etapa,int tentativa,int id_atividade,char nome_atividade[50],int tick_fila,int tick_inicio,int tick_fim,int status_ok);
void inserir_pilha_h(NoHistorico **pilha, NoHistorico *novo_no);
void mostrar_pilha_h(NoHistorico *topo);

void inserir_arvore(Dados* dados,produto* produto);
noa* remover_noa(noa* raiz, int id_buscado);
void remover_arvore(Dados* dados, int id);
void liberar_noa(noa* raiz);
void finalizar_arvore(Dados* dados);


#endif //ESTRUTURAS_H