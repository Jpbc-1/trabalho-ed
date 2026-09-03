#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"

//LISTA DUPLAMENTE ENCADEADA

etapa *iniciar_etapa(int id, char nome[], int capacidade, float taxa_falha, int quantidade_atividade) {
    etapa *nova = (etapa *)calloc(1,sizeof(etapa));
    if (nova == NULL) {
        printf("Erro ao alocar memoria");
        return NULL;
    }
    nova->id = id;
    strcpy(nova->nome, nome);
    nova->capacidade = capacidade;
    nova->taxa_falha = taxa_falha;
    nova->quantidade_atividade = quantidade_atividade;

    nova->vector = (produto **)calloc(capacidade ,  sizeof(produto *));
    return nova;
}
void inserir_etapa(etapa **final, etapa *nova_etapa) {
    if (*final == NULL) {
        *final = nova_etapa;
    } else {
        (*final)->prox = nova_etapa;
        nova_etapa->ant = *final;
        *final = nova_etapa;
    }
}
void mostrar_etapas(etapa *lista) {
    if (lista == NULL) {
        printf("\n A linha de producao esta vazia.\n");
        return;
    }

    etapa *atual = lista;
    printf("\n========================================================\n");
    printf("             STATUS DA LINHA DE PRODUCAO                \n");
    printf("========================================================\n");

    while (atual != NULL) {
        printf("\n[ETAPA %d] %s\n", atual->id, atual->nome);
        printf("--------------------------------------------------------\n");


        printf(" Capacidade : %d vagas (Ocupacao atual: %d)\n", atual->capacidade, atual->ocupacao_atual);
        printf(" Atividades : %d cadastradas\n", atual->quantidade_atividade);
        printf(" Taxa Falha : %.2f%%\n", atual->taxa_falha * 100);
        printf(" Metricas   : %d sucesso | %d falhas totais\n", atual->total_produtos_saida, atual->total_falhas);


        if (atual->ocupacao_atual > 0) {
            printf("\n [Produtos em Processamento]:\n");

            for (int i = 0; i < atual->capacidade; i++) {
                if (atual->vector[i] != NULL) {
                    produto *p = atual->vector[i];
                    printf("   Produto ID %d | Tempo Restante: %ds\n",
                            p->id, p->tempo_restante);
                }
            }
        } else {
            printf("\n Nenhum produto na etapa.\n");
        }

        no *aux = atual->fila_pronto;
        if (aux) printf("\n [Produtos na fila para proxima etapa]:\n");
        mostrar_fila(aux);
        printf("========================================================\n");
        atual = atual->prox;
    }
}
void mostrar_etapa_id(etapa *lista, int id) {
    etapa *atual = lista;
    while (atual  && atual->id != id) {
        atual = atual->prox;
    }
    if (!atual) {
        printf("etapa nao encontrada!");
        return ;
    }
    printf("\n[ETAPA %d] %s\n", atual->id, atual->nome);
    printf("--------------------------------------------------------\n");


    printf(" Capacidade : %d vagas (Ocupacao atual: %d)\n", atual->capacidade, atual->ocupacao_atual);
    printf(" Atividades : %d cadastradas\n", atual->quantidade_atividade);
    printf(" Taxa Falha : %.2f%%\n", atual->taxa_falha * 100);
    printf(" Metricas   : %d sucesso | %d falhas totais\n", atual->total_produtos_saida, atual->total_falhas);


    if (atual->ocupacao_atual > 0) {
        printf("\n [Produtos em Processamento]:\n");

        for (int i = 0; i < atual->capacidade; i++) {
            if (atual->vector[i] != NULL) {
                produto *p = atual->vector[i];
                printf("   Produto ID %d | Tempo Restante: %ds\n",
                        p->id, p->tempo_restante);
            }
        }
    } else {
        printf("\n Nenhum produto na etapa.\n");
    }

    no *aux = atual->fila_pronto;
    if (aux) printf("\n [Produtos na fila para proxima etapa]:\n");
    mostrar_fila(aux);
    printf("========================================================\n");
}
void mostrar_etapa_nome(etapa *lista,char nome[50]){
    etapa *atual = lista;
    while (atual  && strcmp(nome, atual->nome) != 0) {
        atual = atual->prox;
    }
    if (!atual) {
        printf("etapa nao encontrada!");
        return ;
    }
    printf("\n[ETAPA %d] %s\n", atual->id, atual->nome);
    printf("--------------------------------------------------------\n");


    printf(" Capacidade : %d vagas (Ocupacao atual: %d)\n", atual->capacidade, atual->ocupacao_atual);
    printf(" Atividades : %d cadastradas\n", atual->quantidade_atividade);
    printf(" Taxa Falha : %.2f%%\n", atual->taxa_falha * 100);
    printf(" Metricas   : %d sucesso | %d falhas totais\n", atual->total_produtos_saida, atual->total_falhas);


    if (atual->ocupacao_atual > 0) {
        printf("\n [Produtos em Processamento]:\n");

        for (int i = 0; i < atual->capacidade; i++) {
            if (atual->vector[i] != NULL) {
                produto *p = atual->vector[i];
                printf("   Produto ID %d | Tempo Restante: %ds\n",
                        p->id, p->tempo_restante);
            }
        }
    } else {
        printf("\n Nenhum produto na etapa.\n");
    }

    no *aux = atual->fila_pronto;
    if (aux) printf("\n [Produtos na fila para proxima etapa]:\n");
    mostrar_fila(aux);
    printf("========================================================\n");
}


// Lista Simplesmente Encadeada

atividade *iniciar_atividade(int id, char nome[], int tempo_ativ, float taxa_falha) {
    atividade *nova = (atividade *)calloc(1,sizeof(atividade));
    if (nova == NULL) return NULL;

    nova->id = id;
    strcpy(nova->nome, nome);
    nova->tempo = tempo_ativ;
    nova->taxa_falha = taxa_falha;

    return nova;
}
void inserir_atividade(atividade **lista_atividades, atividade *nova_atividade) {
    if (*lista_atividades == NULL) {
        *lista_atividades = nova_atividade;
    } else {
        atividade *atual = *lista_atividades;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = nova_atividade;
    }
}
void mostrar_atividades(etapa *lista) {
    etapa *aux = lista;
    while (aux) {
        atividade *atual = aux->atividade;
        printf("\nAtividades da Etapa %d:\n", aux->id);
        while (atual != NULL) {
            printf("  -> [%d] %s (Tempo: %ds | Falha: %.2f)\n",
                   atual->id, atual->nome, atual->tempo, atual->taxa_falha);
            printf("[Produtos na atividade]:");
            int item = 0;
            if (aux->ocupacao_atual > 0) {
                for (int i = 0; i < aux->capacidade; i++) {
                    if (aux->vector[i] != NULL && (aux->vector[i])->atividade_atual != NULL &&  (aux->vector[i])->atividade_atual->id == atual->id) {
                        printf("   Produto ID %d | Tempo Restante: %ds\n",
                                (aux->vector[i])->id, (aux->vector[i])->tempo_restante);
                        item ++;
                    }
                }
            } if (item == 0) {
                printf("\n Nenhum produto na atividade.\n");
            }
            atual = atual->prox;
        }
        aux = aux->prox;
    }
}
void mostrar_atividades_id(etapa *lista, int id_etapa) {
    etapa *aux = lista;
    while (aux  && aux->id != id_etapa) {
        aux = aux->prox;
    }
    if (!aux) {
        printf("etapa nao encontrada!");
        return ;
    }
    atividade *atual = aux->atividade;
    printf("\nAtividades da Etapa %d:\n", id_etapa);
    while (atual != NULL) {
        printf("  -> [%d] %s (Tempo: %ds | Falha: %.2f)\n",
               atual->id, atual->nome, atual->tempo, atual->taxa_falha);
        printf("[Produtos na atividade]:");
        int item = 0;
        if (aux->ocupacao_atual > 0) {
            for (int i = 0; i < aux->capacidade; i++) {
                if (aux->vector[i] != NULL && (aux->vector[i])->atividade_atual != NULL &&  (aux->vector[i])->atividade_atual->id == atual->id) {
                    printf("   Produto ID %d | Tempo Restante: %ds\n",
                            (aux->vector[i])->id, (aux->vector[i])->tempo_restante);
                    item ++;
                }
            }
        } if (item == 0) {
            printf("\n Nenhum produto na atividade.\n");
        }

        atual = atual->prox;
    }
}
void mostrar_atividades_nome(etapa *lista, char nome[50]) {
    etapa *aux = lista;
    while (aux && strcmp(aux->nome, nome) != 0 ) {
        aux = aux->prox;
    }
    if (!aux) {
        printf("etapa nao encontrada!");
        return ;
    }

    atividade *atual = aux->atividade;
    printf("\nAtividades da Etapa %d (%s):\n", aux->id, aux->nome);

    while (atual != NULL) {
        printf("  -> [%d] %s (Tempo: %ds | Falha: %.2f)\n",
               atual->id, atual->nome, atual->tempo, atual->taxa_falha);
        printf("[Produtos na atividade]:");

        int item = 0;
        if (aux->ocupacao_atual > 0) {
            for (int i = 0; i < aux->capacidade; i++) {
                if (aux->vector[i] != NULL && (aux->vector[i])->atividade_atual != NULL &&  (aux->vector[i])->atividade_atual->id == atual->id) {
                    printf("   Produto ID %d | Tempo Restante: %ds\n",
                            (aux->vector[i])->id, (aux->vector[i])->tempo_restante);
                    item ++;
                }
            }
        }
        if (item == 0) {
            printf("\n Nenhum produto na atividade.\n");
        }

        atual = atual->prox;
    }
}
void mostrar_atividade_id_id(etapa *lista,int id_atividade, int id_etapa){
    etapa *aux = lista;
    while (aux  && aux->id != id_etapa) {
        aux = aux->prox;
    }
    if (!aux) {
        printf("etapa nao encontrada!");
        return ;
    }
    atividade *atual = aux->atividade;
    while (atual && atual->id != id_atividade) atual = atual->prox;
    if (!atual) {
        printf("Atividade nao encontrada!");
        return ;
    }
    printf("\nAtividade %d da Etapa %d (%s):\n",id_atividade, id_etapa,aux->nome);
    printf("  ->  %s (Tempo: %ds | Falha: %.2f)\n",
                atual->nome, atual->tempo, atual->taxa_falha);
    printf("[Produtos na atividade]:");
    int item = 0;
    if (aux->ocupacao_atual > 0) {
        for (int i = 0; i < aux->capacidade; i++) {
            if (aux->vector[i] != NULL && (aux->vector[i])->atividade_atual != NULL &&  (aux->vector[i])->atividade_atual->id == atual->id) {
                printf("   Produto ID %d | Tempo Restante: %ds\n",
                        (aux->vector[i])->id, (aux->vector[i])->tempo_restante);
                item ++;
            }
        }
    } if (item == 0) {
        printf("\n Nenhum produto na atividade.\n");
    }
}
void mostrar_atividade_id_nome(etapa *lista,int id_atividade, char nome[50]) {
    etapa *aux = lista;
    while (aux  && strcmp(aux->nome, nome) != 0) {
        aux = aux->prox;
    }
    if (!aux) {
        printf("etapa nao encontrada!");
        return ;
    }
    atividade *atual = aux->atividade;
    while (atual && atual->id != id_atividade) atual = atual->prox;
    if (!atual) {
        printf("Atividade nao encontrada!");
        return ;
    }
    printf("\nAtividade %d da Etapa %d (%s):\n",id_atividade,aux->id, nome);
    printf("  ->  %s (Tempo: %ds | Falha: %.2f)\n",
                atual->nome, atual->tempo, atual->taxa_falha);
    printf("[Produtos na atividade]:");
    int item = 0;
    if (aux->ocupacao_atual > 0) {
        for (int i = 0; i < aux->capacidade; i++) {
            if (aux->vector[i] != NULL && (aux->vector[i])->atividade_atual != NULL &&  (aux->vector[i])->atividade_atual->id == atual->id) {
                printf("   Produto ID %d | Tempo Restante: %ds\n",
                        (aux->vector[i])->id, (aux->vector[i])->tempo_restante);
                item ++;
            }
        }
    }if (item == 0) {
        printf("\n Nenhum produto na atividade.\n");
    }
}
void mostrar_atividade_nome_id(etapa *lista,char nome[50], int id_etapa) {
    etapa *aux = lista;
    while (aux  && aux->id != id_etapa) {
        aux = aux->prox;
    }
    if (!aux) {
        printf("etapa nao encontrada!");
        return ;
    }
    atividade *atual = aux->atividade;
    while (atual && strcmp(atual->nome,nome) != 0) atual = atual->prox;
    if (!atual) {
        printf("Atividade nao encontrada!");
        return ;
    }
    printf("\nAtividade %d da Etapa %d (%s):\n",atual->id, id_etapa,aux->nome);
    printf("  ->  %s (Tempo: %ds | Falha: %.2f)\n",
                atual->nome, atual->tempo, atual->taxa_falha);
    printf("[Produtos na atividade]:");
    int item = 0;
    if (aux->ocupacao_atual > 0) {
        for (int i = 0; i < aux->capacidade; i++) {
            if (aux->vector[i] != NULL && (aux->vector[i])->atividade_atual != NULL &&  (aux->vector[i])->atividade_atual->id == atual->id) {
                printf("   Produto ID %d | Tempo Restante: %ds\n",
                        (aux->vector[i])->id, (aux->vector[i])->tempo_restante);
                item++;
            }
        }
    } if (item == 0) {
        printf("\n Nenhum produto na atividade.\n");
    }
}
void mostrar_atividade_nome_nome(etapa *lista,char nome_atividade[50], char nome_etapa[50]) {
    etapa *aux = lista;
    while (aux  && strcmp(aux->nome, nome_etapa) !=0) {
        aux = aux->prox;
    }
    if (!aux) {
        printf("etapa nao encontrada!");
        return ;
    }
    atividade *atual = aux->atividade;
    while (atual && strcmp(atual->nome,nome_atividade) != 0) atual = atual->prox;
    if (!atual) {
        printf("Atividade nao encontrada!");
        return ;
    }
    printf("\nAtividade %d da Etapa %d (%s):\n",atual->id, aux->id,aux->nome);
    printf("  ->  %s (Tempo: %ds | Falha: %.2f)\n",
                atual->nome, atual->tempo, atual->taxa_falha);
    printf("[Produtos na atividade]:");
    int item = 0;
    if (aux->ocupacao_atual > 0) {
        for (int i = 0; i < aux->capacidade; i++) {
            if (aux->vector[i] != NULL && (aux->vector[i])->atividade_atual != NULL && (aux->vector[i])->atividade_atual->id == atual->id) {
                printf("   Produto ID %d | Tempo Restante: %ds\n",
                        (aux->vector[i])->id, (aux->vector[i])->tempo_restante);
                item++;
            }
        }
    }
    if (item == 0) {
        printf("\n Nenhum produto na atividade.\n");
    }
}
void liberar_atividades(atividade **lista) {
    if (*lista == NULL) {
        printf("\nAtividades vazia!");
        return;
    }
    atividade *atual = *lista;

    while (atual != NULL) {
        atividade *prox = atual->prox;
        free(atual);
        atual = prox;
    }

    *lista = NULL;
}

void liberar_linha(etapa **lista) {
    if (*lista == NULL) {
        printf("Linha vazia!");
        return;
    }
    etapa *atual = *lista;
    while (atual != NULL) {
        etapa *prox = atual->prox;
        if (atual->vector != NULL) free(atual->vector);
        if (atual->fila_pronto) liberar_fila(&(atual->fila_pronto));
        liberar_atividades(&(atual->atividade));
        free(atual);
        atual = prox;
    }
    *lista = NULL;
}
void mostrar_linha(etapa *lista) {
    if (lista == NULL) {
        printf("\nA linha de producao esta vazia.\n");
        return;
    }

    etapa *etapa_atual = lista;
    printf("\n=================================================================\n");
    printf("                  STATUS GLOBAL DA FABRICA                       \n");
    printf("=================================================================\n");

    while (etapa_atual != NULL) {
        printf("\n[ETAPA %d] %s\n", etapa_atual->id, etapa_atual->nome);
        printf("-----------------------------------------------------------------\n");
        printf(" Capacidade : %d vagas (Ocupacao atual: %d)\n", etapa_atual->capacidade, etapa_atual->ocupacao_atual);
        printf(" Atividades : %d cadastradas\n", etapa_atual->quantidade_atividade);
        printf(" Taxa Falha : %.2f%%\n", etapa_atual->taxa_falha * 100);
        printf(" Metricas   : %d sucesso | %d falhas totais\n", etapa_atual->total_produtos_saida, etapa_atual->total_falhas);
        printf("-----------------------------------------------------------------\n");

        atividade *ativ_atual = etapa_atual->atividade;

        if (ativ_atual == NULL) {
            printf(" Nenhuma atividade cadastrada nesta etapa.\n");
        }

        while (ativ_atual != NULL) {
            printf("\n  >> Atividade [%d]: %s (Tempo base: %ds | Falha: %.0f%%)\n",
                   ativ_atual->id, ativ_atual->nome, ativ_atual->tempo, ativ_atual->taxa_falha * 100);

            int produtos_encontrados = 0;

            if (etapa_atual->ocupacao_atual > 0) {
                for (int i = 0; i < etapa_atual->capacidade; i++) {
                    produto *p = etapa_atual->vector[i];

                    if (p != NULL && p->atividade_atual != NULL && p->atividade_atual->id == ativ_atual->id) {
                    printf("       -> Produto ID: %03d | Tempo Restante: %2ds\n", p->id, p->tempo_restante);
                        produtos_encontrados++;
                    }
                }
            }
            if (produtos_encontrados == 0) {
                printf("       -> (Nenhum produto em processamento nessa atividade)\n");
            }

            ativ_atual = ativ_atual->prox;
        }
        printf("\n  [Fila de Espera para Proxima Etapa]: ");
        no *aux_fila = etapa_atual->fila_pronto;
        if (aux_fila == NULL) {
            printf("Vazia\n");
        } else mostrar_fila(aux_fila);
        printf("=================================================================\n");
        etapa_atual = etapa_atual->prox;
    }
}

// FUNÇÕES DE FILA


void inserir_fila(no **f,produto *produto) {
    no *aux = (no *)malloc(sizeof(no));
    aux->produto = produto;
    aux->prox = NULL;

    if (*f == NULL) {
        *f = aux;
    } else {
        no *aux2 = *f;
        while (aux2->prox) aux2 = aux2->prox;
        aux2->prox = aux;
    }
}
void mostrar_fila(no *f) {
    no *aux = f;
    if (!aux) {
        printf("fila vazia\n");
        return;
    }
    printf("\n");
    while (aux != NULL) {
        printf("       -> Produto ID: %03d \n", aux->produto->id);
        aux = aux->prox;
    }
}
produto* remover_fila(no **f) {
    if (*f == NULL) return NULL;
    no *aux = *f;
    produto *p = aux->produto;
    *f = (*f)->prox;
    free(aux);
    return p;
}
void liberar_fila(no **f) {
    no *aux = *f;
    if (!aux) {
        printf("fila vazia\n");
        return;
    }
    while (aux != NULL) {
        no* prox = aux->prox;
        free(aux);
        aux = prox;
    }
}

//FUNÇÕES DE PILHA DE DESCARTE


void inserir_pilha_d(no** pilha,produto *produto) {
    no *aux = (no *)malloc(sizeof(no));
    aux->produto = produto;
    aux->prox = *pilha;
    *pilha = aux;
}


//FUNÇÕES DE PILHA DE HISTÓRICO

NoHistorico* iniciar_pilha_h(int id_etapa, int tentativa, int id_atividade, char nome_atividade[], int tick_fila, int tick_inicio, int tick_fim, int status_ok) {
    NoHistorico *novo = (NoHistorico *)malloc(sizeof(NoHistorico));
    
    novo->id_etapa = id_etapa;
    novo->tentativa = tentativa;
    novo->id_atividade = id_atividade;
    strcpy(novo->nome_atividade, nome_atividade);
    novo->tick_fila = tick_fila;
    novo->tick_inicio = tick_inicio;
    novo->tick_fim = tick_fim;
    novo->status_ok = status_ok;

    novo->prox = NULL;
    novo->ant = NULL;

    return novo;
}
void inserir_pilha_h(NoHistorico **pilha, NoHistorico *novo_no) {
    novo_no->prox = *pilha;
    
    if (*pilha != NULL) {
        (*pilha)->ant = novo_no;
    }
    
    *pilha = novo_no;
}
void mostrar_pilha_h(NoHistorico *topo) {
    if (topo == NULL) {
        printf("\n O historico esta vazio.\n");
        return;
    }

    NoHistorico *atual = topo;
    while (atual->prox != NULL) atual = atual->prox;
    printf("\n========================================================\n");
    printf("               HISTORICO DE PROCESSAMENTO               \n");
    printf("========================================================\n");
    printf("                                                         \n\n");

    while (atual != NULL) {
        printf(" [ETAPA %02d | Tentativa %02d]\n", atual->id_etapa, atual->tentativa);
        printf("  -> Atividade : [%02d] %s\n", atual->id_atividade, atual->nome_atividade);

        if (atual->status_ok == 1) {
            printf("  -> Status    : [ OK ] Concluido com Sucesso\n");
        } else {
            printf("  -> Status    : [Falha] Falha %s (Processo Interrompido)\n",atual->prox != NULL? "Leve" : "Critica");
        }

        printf("  -> Timeline  : Esperou fila ate o tick %d | Iniciou no %d | Finalizou no %d\n",
               atual->tick_fila, atual->tick_inicio, atual->tick_fim);

        printf("--------------------------------------------------------\n");

        atual = atual->ant;
    }

    printf("========================================================\n\n");
}

//FUNCOES DA ARVORE

void inserir_arvore(Dados* dados, produto* p) {
    if (p == NULL) return;

    noa* novo = (noa*)calloc(1, sizeof(noa));
    novo->produto = p;

    if (dados->arvore == NULL) {
        dados->arvore = novo;
        return;
    }

    noa* atual = dados->arvore;
    while (1) {
        if (p->id < atual->produto->id) {
            if (atual->esq == NULL) {
                atual->esq = novo;
                break;
            }
            atual = atual->esq;
        } else {
            if (atual->dir == NULL) {
                atual->dir = novo;
                break;
            }
            atual = atual->dir;
        }
    }
}
noa* remover_noa(noa* raiz, int id_buscado) {
    if (raiz == NULL) return NULL;

    if (id_buscado < raiz->produto->id) {
        raiz->esq = remover_noa(raiz->esq, id_buscado);
    } else if (id_buscado > raiz->produto->id) {
        raiz->dir = remover_noa(raiz->dir, id_buscado);
    } else {
        if (raiz->esq == NULL) {
            noa* temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            noa* temp = raiz->esq;
            free(raiz);
            return temp;
        }
        noa* temp = raiz->dir;
        while (temp && temp->esq != NULL) temp = temp->esq;

        raiz->produto = temp->produto;
        raiz->dir = remover_noa(raiz->dir, temp->produto->id);
    }
    return raiz;
}
void remover_arvore(Dados* dados, int id) {
    dados->arvore = remover_noa(dados->arvore, id);
}
void liberar_noa(noa* raiz) {
    if (raiz != NULL) {
        liberar_noa(raiz->esq);
        liberar_noa(raiz->dir);
        free(raiz);
    }
}
void finalizar_arvore(Dados* dados) {
    liberar_noa(dados->arvore);
    dados->arvore = NULL;
}




