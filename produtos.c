
#include "produtos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <time.h>






void passar_atividade(produto *p) {
    p->atividade_atual->total_produtos_saida++;
    p->atividade_atual = p->atividade_atual->prox;
    p->tempo_restante = p->atividade_atual->tempo;
}

void passar_etapa(Dados *dados, produto *p, int indice_vetor) {
    etapa *maquina = p->etapa_atual;


    int tempo_gasto_na_maquina = 0;
    NoHistorico *h = p->topo;
    while (h != NULL && h->id_etapa == maquina->id) {

        tempo_gasto_na_maquina += (h->tick_fim - h->tick_inicio) + h->tick_fila;
        h = h->prox;
    }
    if (tempo_gasto_na_maquina > maquina->tempo_maximo) {
        maquina->tempo_maximo = tempo_gasto_na_maquina;
    }


    maquina->vector[indice_vetor] = NULL;
    if (maquina == dados->ultima_etapa)
        maquina->ocupacao_atual--;
    maquina->total_produtos_saida++;

    p->atividade_atual = NULL;


    p->tick_espera_temporario = dados->tick_atual;


    inserir_fila(&(maquina->fila_pronto), p);


    if (maquina == dados->ultima_etapa) {
        p->tick_saida_linha = dados->tick_atual;
        dados->produtos_concluidos++;
        p->etapa_atual = NULL;
        remover_arvore(dados, p->id);
    } else {
        p->tentativa_etapa = 1;
    }
}

void entrar_etapa(Dados *dados) {
    etapa *atual = dados->ultima_etapa;

    while (atual != NULL) {
        no **fila_origem;
        if (atual == dados->primeira_etapa) {
            fila_origem = &(dados->entrada_fabrica);
        } else {
            fila_origem = &(atual->ant->fila_pronto);
        }

        int inseridos_neste_tick = 0;


        while (atual->ocupacao_atual < atual->capacidade && *fila_origem != NULL) {


            if (atual == dados->primeira_etapa && inseridos_neste_tick >= dados->taxa_entrada) {
                break;
            }

            produto *p = remover_fila(fila_origem);

            if (atual != dados->primeira_etapa) {
                atual->ant->ocupacao_atual--;
            }

            if (atual == dados->primeira_etapa) {
                inseridos_neste_tick++;
                p->tick_entrada_linha = dados->tick_atual;
                inserir_arvore(dados, p);
            }


            int tempo_na_fila = dados->tick_atual - p->tick_espera_temporario;
            p->tick_espera += tempo_na_fila;
            atual->soma_tempo_fila += tempo_na_fila;
            p->tick_espera_temporario = tempo_na_fila;

            for (int i = 0; i < atual->capacidade; i++) {
                if (atual->vector[i] == NULL) {
                    atual->vector[i] = p;
                    atual->ocupacao_atual++;

                    p->etapa_atual = atual;
                    p->atividade_atual = atual->atividade;

                    if (p->atividade_atual != NULL) {
                        p->tempo_restante = p->atividade_atual->tempo;
                    }
                    break;
                }
            }
        }
        atual = atual->ant;
    }
}

void avancar_tick(Dados *dados) {
    dados->tick_atual++;

    etapa *atual = dados->ultima_etapa;

    while (atual != NULL) {
        if (atual->ocupacao_atual > 0) {
            for (int i = 0; i < atual->capacidade; i++) {
                produto *p = atual->vector[i];

                if (p != NULL && p->atividade_atual != NULL) {
                    p->tempo_restante--;
                    p->tempo_total_atividade++;

                    atual->soma_tempo_total++;
                    p->atividade_atual->soma_tempo_total++;


                    if (p->tempo_restante <= 0) {

                        float sorteio = (float)rand() / (float)RAND_MAX;
                        int fila_gasta = p->tick_espera_temporario;
                        p->tick_espera_temporario = 0;

                        if (sorteio <= atual->taxa_falha) {

                            p->falhas++;
                            dados->falhas_totais++;
                            atual->total_falhas++;

                            NoHistorico *hist = iniciar_pilha_h(atual->id, p->tentativa_etapa, p->atividade_atual->id, p->atividade_atual->nome, fila_gasta, dados->tick_atual - p->atividade_atual->tempo, dados->tick_atual, 0);
                            inserir_pilha_h(&(p->topo), hist);

                            atual->vector[i] = NULL;
                            atual->ocupacao_atual--;
                            inserir_pilha_d(&(dados->lixeira), p);
                            remover_arvore(dados, p->id);
                            dados->produtos_descartados++;
                            p->tick_saida_linha = dados->tick_atual;
                        }
                        else if (sorteio <= p->atividade_atual->taxa_falha) {

                            p->falhas++;
                            dados->falhas_totais++;
                            atual->total_falhas++;

                            NoHistorico *hist = iniciar_pilha_h(atual->id, p->tentativa_etapa, p->atividade_atual->id, p->atividade_atual->nome, fila_gasta, dados->tick_atual - p->atividade_atual->tempo, dados->tick_atual, 0);
                            inserir_pilha_h(&(p->topo), hist);

                            p->tentativa_etapa++;
                            p->atividade_atual = atual->atividade;
                            p->tempo_restante = p->atividade_atual->tempo;
                        }
                        else {

                            NoHistorico *hist = iniciar_pilha_h(atual->id, p->tentativa_etapa, p->atividade_atual->id, p->atividade_atual->nome, fila_gasta, dados->tick_atual - p->atividade_atual->tempo, dados->tick_atual, 1);
                            inserir_pilha_h(&(p->topo), hist);


                            if (p->atividade_atual->prox != NULL) {
                                passar_atividade(p);
                            } else {
                                p->atividade_atual->total_produtos_saida++;
                                passar_etapa(dados, p, i);
                            }
                        }
                    }
                }
            }
        }
        atual = atual->ant;
    }


    entrar_etapa(dados);
}

produto* iniciar_produto(int id, int tick_atual,char nome_produto[]) {
    produto *p = (produto *)calloc(1,sizeof(produto));
    if (!p) return NULL;

    p->id = id;
    strcpy(p->nome, nome_produto);

    p->tick_criacao = tick_atual;
    p->tentativa_etapa = 1;

    return p;
}

void iniciar(Dados *dados, char nome_arquivo[50]) {
    memset(dados, 0, sizeof(Dados));

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Nao foi possivel abrir o arquivo: %s\n", nome_arquivo);
        return;
    }

    char ing[256];
    fgets(ing, sizeof(ing), arquivo);

    char comando[50];
    int contador_id_produto = 1;

    while (fscanf(arquivo, "%s", comando) == 1) {

        if (strcmp(comando, "SIMULACAO") == 0) {
            fscanf(arquivo, "%s %d %d", dados->nome_simulacao, &dados->semente, &dados->limite_seg);
            srand(dados->semente);
        }

        else if (strcmp(comando, "PRODUTO") == 0 || strcmp(comando, "PRODUTOS") == 0) {
            int qtd, taxa;
            char nome_prod[50];
            fscanf(arquivo, "%d %d %s", &qtd, &taxa, nome_prod);

            dados->meta_produtos += qtd;
            if (dados->taxa_entrada == 0)dados->taxa_entrada = taxa;

            for (int i = 0; i < qtd; i++) {
                produto *novo_p = iniciar_produto(contador_id_produto, 0, nome_prod);
                inserir_fila(&(dados->entrada_fabrica), novo_p);
                contador_id_produto++;
            }
        }

        else if (strcmp(comando, "LINHA_PRODUCAO") == 0) {
            int qtd_etapas;
            fscanf(arquivo, "%d", &qtd_etapas);
        }

        else if (strcmp(comando, "ETAPA") == 0) {
            int e_id, e_qtd_ativ, e_cap;
            float e_taxa;
            char e_nome[50];

            fscanf(arquivo, "%d %d %d %f %s", &e_id, &e_qtd_ativ, &e_cap, &e_taxa, e_nome);

            etapa *nova_etapa = iniciar_etapa(e_id, e_nome, e_cap, e_taxa, e_qtd_ativ);
            inserir_etapa(&(dados->ultima_etapa), nova_etapa);

            if (dados->primeira_etapa == NULL) {
                dados->primeira_etapa = dados->ultima_etapa;
            }
        }

        else if (strcmp(comando, "ATIVIDADE") == 0) {
            int a_id, a_tempo;
            float a_taxa;
            char a_nome[50];

            fscanf(arquivo, "%d %d %f %s", &a_id, &a_tempo, &a_taxa, a_nome);

            atividade *nova_ativ = iniciar_atividade(a_id, a_nome, a_tempo, a_taxa);

            if (dados->ultima_etapa != NULL) {
                dados->ultima_etapa->tempo_minimo += a_tempo;
                inserir_atividade(&(dados->ultima_etapa->atividade), nova_ativ);
            }
        }
    }

    fclose(arquivo);

    printf("\n %s iniciada\n", dados->nome_simulacao);
}

void finalizar(Dados *dados) {
    if (dados == NULL) return;

    no *aux_ent = dados->entrada_fabrica;
    while (aux_ent != NULL) {
        no *prox = aux_ent->prox;
        liberar_produto(aux_ent->produto);
        free(aux_ent);
        aux_ent = prox;
    }
    dados->entrada_fabrica = NULL;

    no *aux_lix = dados->lixeira;
    while (aux_lix != NULL) {
        no *prox = aux_lix->prox;
        liberar_produto(aux_lix->produto);
        free(aux_lix);
        aux_lix = prox;
    }
    dados->lixeira = NULL;

    etapa *e_atual = dados->primeira_etapa;
    while (e_atual != NULL) {

        if (e_atual->vector != NULL && e_atual->ocupacao_atual > 0) {
            for (int i = 0; i < e_atual->capacidade; i++) {
                if (e_atual->vector[i] != NULL) {
                    liberar_produto(e_atual->vector[i]);
                    e_atual->vector[i] = NULL;
                }
            }
        }

        no *aux_fila = e_atual->fila_pronto;
        while (aux_fila != NULL) {
            no *prox = aux_fila->prox;
            liberar_produto(aux_fila->produto);
            free(aux_fila);
            aux_fila = prox;
        }
        e_atual->fila_pronto = NULL;

        e_atual = e_atual->prox;
    }
    finalizar_arvore(dados);
    liberar_linha(&(dados->primeira_etapa));
}

void criar_relatorio(Dados *dados, char nome_arquivo_saida[50], char arquivo_entrada[50]) {
    FILE *f = fopen(nome_arquivo_saida, "w");
    if (!f) {
        printf(" Nao foi possivel criar o arquivo de relatorio!\n");
        return;
    }

    float tempo_medio_linha = 0.0;
    float tempo_medio_espera = 0.0;
    int produtos_faltantes = dados->meta_produtos - dados->produtos_concluidos;

    no *estoque = NULL;
    if (dados->ultima_etapa != NULL) estoque = dados->ultima_etapa->fila_pronto;

    no *aux = estoque;
    int qtd_estoque = 0;
    while (aux != NULL) {
        produto *p = aux->produto;
        tempo_medio_linha += (p->tick_saida_linha - p->tick_entrada_linha);
        tempo_medio_espera += p->tick_espera;
        qtd_estoque++;
        aux = aux->prox;
    }

    if (qtd_estoque > 0) {
        tempo_medio_linha /= qtd_estoque;
        tempo_medio_espera /= qtd_estoque;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char timestamp[50];
    sprintf(timestamp, "%04d%02d%02d_%02d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fprintf(f, "=== METADADOS ===\n");
    fprintf(f, "id_simulacao: %s_%s\n", dados->nome_simulacao, timestamp);
    fprintf(f, "semente_utilizada: %d\n", dados->semente);
    fprintf(f, "arquivo_entrada: %s\n", arquivo_entrada);
    fprintf(f, "tick_fim: %d\n", dados->tick_atual);
    fprintf(f, "produtos_concluidos: %d\n", dados->produtos_concluidos);
    fprintf(f, "tempo_medio_linha: %.2f\n", tempo_medio_linha);
    fprintf(f, "tempo_medio_espera: %.2f\n", tempo_medio_espera);
    fprintf(f, "falhas_totais: %d\n", dados->falhas_totais);
    fprintf(f, "Meta alcancada: %s\n", (dados->produtos_concluidos >= dados->meta_produtos) ? "sim" : "nao");
    fprintf(f, "Produtos faltantes: %d\n\n", produtos_faltantes);

    fprintf(f, "=== RELATORIO DA SIMULACAO %s ===\n", dados->nome_simulacao);

    fprintf(f, "------- RELATORIO DAS ETAPAS-----------------\n");
    etapa *e_atual = dados->primeira_etapa;
    while (e_atual != NULL) {
        float falhas_por_prod = (e_atual->total_produtos_saida > 0) ? (float)e_atual->total_falhas / e_atual->total_produtos_saida : 0.0;
        float tempo_med_etapa = (e_atual->total_produtos_saida > 0) ? (float)e_atual->soma_tempo_total / e_atual->total_produtos_saida : 0.0;
        float tempo_med_fila = (e_atual->total_produtos_saida > 0) ? (float)e_atual->soma_tempo_fila / e_atual->total_produtos_saida : 0.0;

        fprintf(f, "ETAPA %d %s:\n", e_atual->id, e_atual->nome);
        fprintf(f, "Atividades: %d\n", e_atual->quantidade_atividade);
        fprintf(f, "Quantidade de falhas: %d\n", e_atual->total_falhas);
        fprintf(f, "Falhas por produto: %.2f\n\n", falhas_por_prod);

        fprintf(f, "Tempo minimo: %d\n", e_atual->tempo_minimo);
        fprintf(f, "Tempo medio: %.2f\n", tempo_med_etapa);
        fprintf(f, "Tempo maximo: %d\n", e_atual->tempo_maximo);
        fprintf(f, "Tempo medio em fila: %.2f\n", tempo_med_fila);
        e_atual = e_atual->prox;
    }


    fprintf(f, "------- RELATORIO DAS ATIVIDADES--------------\n");
    e_atual = dados->primeira_etapa;
    while (e_atual != NULL) {
        atividade *a_atual = e_atual->atividade;
        while (a_atual != NULL) {
            fprintf(f, "ETAPA %d %s:\n", e_atual->id, e_atual->nome);
            fprintf(f, "ATIVIDADE %d %s:\n", a_atual->id, a_atual->nome);

            fprintf(f, "Capacidade: %d\n", e_atual->capacidade);

            double vazao = (dados->tick_atual > 0) ? ((float)a_atual->total_produtos_saida / (float)dados->tick_atual) : 0.0;
            double tempo_med_total_a = (a_atual->total_produtos_saida > 0) ? (float)a_atual->soma_tempo_total / (float)a_atual->total_produtos_saida : 0.0;
            double tempo_med_fila_a = (a_atual->total_produtos_saida > 0) ? (float)a_atual->soma_tempo_fila / (float)a_atual->total_produtos_saida : 0.0;

            fprintf(f, "Vazao: %.2f\n", vazao);
            fprintf(f, "Tempo de execucao: %d\n", a_atual->tempo);
            fprintf(f, "Tempo medio em fila: %.2f\n", tempo_med_fila_a);
            fprintf(f, "Tempo medio total: %.2f\n\n", tempo_med_total_a);

            a_atual = a_atual->prox;
        }
        e_atual = e_atual->prox;
    }

    fprintf(f, "--------RELATORIO DOS PRODUTOS----------------\n\n");

    fprintf(f, "[1. PRODUTOS CONCLUIDOS COM SUCESSO]\n\n");
    if (estoque != NULL) {
        aux = estoque;
        while (aux != NULL) {
            imprimir_dados_produto(f, aux->produto, "CONCLUIDO", dados->tick_atual);
            aux = aux->prox;
        }
    } else {
        fprintf(f, "Nenhum produto concluido.\n\n");
    }

    fprintf(f, "[2. PRODUTOS DESCARTADOS (FALHA CRITICA)]\n\n");
    if (dados->lixeira != NULL) {
        aux = dados->lixeira;
        while (aux != NULL) {
            imprimir_dados_produto(f, aux->produto, "DESCARTADO", dados->tick_atual);
            aux = aux->prox;
        }
    } else {
        fprintf(f, "Nenhum produto descartado.\n\n");
    }

    fprintf(f, "[3. PRODUTOS EM PROCESSAMENTO ]\n\n");
    e_atual = dados->primeira_etapa;
    int produtos_linha = 0;

    while (e_atual != NULL) {
        if (e_atual->ocupacao_atual > 0) {
            for (int i = 0; i < e_atual->capacidade; i++) {
                if (e_atual->vector[i] != NULL) {
                    char status_maquina[100];
                    sprintf(status_maquina, "PROCESSANDO (Etapa %d)(Atividade %d)", e_atual->id,(e_atual->vector[i])->atividade_atual->id);
                    imprimir_dados_produto(f, e_atual->vector[i], status_maquina, dados->tick_atual);
                    produtos_linha++;
                }
            }
        }

        if (e_atual != dados->ultima_etapa && e_atual->fila_pronto != NULL) {
            aux = e_atual->fila_pronto;
            while (aux != NULL) {
                char status_fila[100];
                sprintf(status_fila, "AGUARDANDO (Fila da Etapa %d)", e_atual->prox->id);
                imprimir_dados_produto(f, aux->produto, status_fila, dados->tick_atual);
                produtos_linha++;
                aux = aux->prox;
            }
        }
        e_atual = e_atual->prox;
    }

    if (produtos_linha == 0) {
        fprintf(f, "Nenhum produto em processamento nas linhas.\n\n");
    }

    fprintf(f, "[4. PRODUTOS NA PORTA DA FABRICA]\n\n");
    if (dados->entrada_fabrica != NULL) {
        aux = dados->entrada_fabrica;
        while (aux != NULL) {
            imprimir_dados_produto(f, aux->produto, "NAO INICIADO", dados->tick_atual);
            aux = aux->prox;
        }
    } else {
        fprintf(f, "Nenhum produto aguardando entrada.\n\n");
    }

    fclose(f);
}

void imprimir_dados_produto(FILE *f, produto *p, char status[], int tick_atual_simulacao) {
    fprintf(f, "--- PRODUTO %03d ---\n", p->id);
    fprintf(f, "Modelo: %s\n", p->nome);
    fprintf(f, "Status: %s\n\n", status);

    fprintf(f, "Criacao: tick %d\n", p->tick_criacao);

    if (p->tick_entrada_linha > 0) {
        fprintf(f, "Entrada na linha: tick %d\n", p->tick_entrada_linha);
    } else {
        fprintf(f, "Entrada na linha: (Aguardando)\n");
    }

    if (p->tick_saida_linha > 0) {
        fprintf(f, "Saida da linha: tick %d\n", p->tick_saida_linha);
        fprintf(f, "Tempo total: %d ticks\n", p->tick_saida_linha - p->tick_entrada_linha);
    } else if (p->tick_entrada_linha > 0) {
        fprintf(f, "Saida da linha: (Em andamento)\n");
        fprintf(f, "Tempo processado ate agora: %d ticks\n", tick_atual_simulacao - p->tick_entrada_linha);
    }

    fprintf(f, "Tempo em espera: %d ticks\n\n", p->tick_espera);
    fprintf(f, "Falhas: %d\n", p->falhas);

    if (p->topo != NULL) {
        fprintf(f, "Trajetoria:\n");
        NoHistorico *hist = p->topo;
        while (hist->prox != NULL) hist = hist->prox;

        int etapa_impressa = -1;
        int tentativa_impressa = -1;
        int ticks_gastos_etapa = 0;

        while (hist != NULL) {
            if (hist->id_etapa != etapa_impressa || hist->tentativa != tentativa_impressa) {
                if (etapa_impressa != -1) {
                     fprintf(f, "Ticks na etapa: %d ticks\n", ticks_gastos_etapa);
                }
                fprintf(f, "Etapa %d tentativa %d:\n", hist->id_etapa, hist->tentativa);
                etapa_impressa = hist->id_etapa;
                tentativa_impressa = hist->tentativa;
                ticks_gastos_etapa = 0;
            }

            fprintf(f, "  Atividade %d (%s) fila:%d inicio:%d fim:%d <%s>\n",
                    hist->id_atividade, hist->nome_atividade,
                    hist->tick_fila, hist->tick_inicio, hist->tick_fim,
                    (hist->status_ok) ? "OK" : "FALHOU");

            ticks_gastos_etapa += (hist->tick_fim - hist->tick_inicio);
            hist = hist->ant;
        }

        if (etapa_impressa != -1) {
            fprintf(f, "Ticks na etapa: %d ticks\n", ticks_gastos_etapa);
        }
    } else {
        fprintf(f, "Trajetoria: Nenhuma atividade registrada ainda.\n");
    }
    fprintf(f, "----------------------------------------------\n\n");
}

void liberar_produto(produto *p) {
    if (p == NULL) return;

    NoHistorico *atual = p->topo;
    while (atual != NULL) {
        NoHistorico *prox = atual->prox;
        free(atual);
        atual = prox;
    }

    free(p);
}

void buscar_produto(Dados *dados, int id_buscado) {
    if (dados == NULL) return;

    produto *encontrado = NULL;
    char localizacao[100] = "";
    char detalhes_etapa[100] = "";

    no *aux_ent = dados->entrada_fabrica;
    while (aux_ent != NULL) {
        if (aux_ent->produto->id == id_buscado) {
            encontrado = aux_ent->produto;
            strcpy(localizacao, "Aguardando na porta de entrada");
            strcpy(detalhes_etapa, "-");
            break;
        }
        aux_ent = aux_ent->prox;
    }

    if (!encontrado) {
        no *aux_lix = dados->lixeira;
        while (aux_lix != NULL) {
            if (aux_lix->produto->id == id_buscado) {
                encontrado = aux_lix->produto;
                strcpy(localizacao, "Lixeira (Descartado por falha critica)");
                strcpy(detalhes_etapa, "-");
                break;
            }
            aux_lix = aux_lix->prox;
        }
    }

    if (!encontrado) {
        noa* atual = dados->arvore;
        while (atual != NULL) {
            if (id_buscado == atual->produto->id) {
                encontrado = atual->produto;

                if (encontrado->etapa_atual != NULL) {
                    strcpy(localizacao, "Em processamento ou Fila Interna");
                    sprintf(detalhes_etapa, "Etapa: %s | Ativ: %s",
                            encontrado->etapa_atual->nome,
                            encontrado->atividade_atual ? encontrado->atividade_atual->nome : "Transitando/Fila");
                }
                break;
            } else if (id_buscado < atual->produto->id) {
                atual = atual->esq;
            } else {
                atual = atual->dir;
            }
        }
    }

    if (!encontrado && dados->ultima_etapa != NULL) {
        no *aux_fila = dados->ultima_etapa->fila_pronto;
        while (aux_fila != NULL) {
            if (aux_fila->produto->id == id_buscado) {
                encontrado = aux_fila->produto;
                strcpy(localizacao, "Estoque Final (Concluido com sucesso)");
                strcpy(detalhes_etapa, "-");
                break;
            }
            aux_fila = aux_fila->prox;
        }
    }

    printf("\n========================================================\n");
    if (encontrado) {
        printf(" [ID: %03d] Produto: %s\n", encontrado->id, encontrado->nome);
        printf("--------------------------------------------------------\n");
        printf(" Status   : %s\n", localizacao);
        printf(" Posicao  : %s\n", detalhes_etapa);


        if (encontrado->tick_saida_linha > 0) {
            printf(" Tempo    : Finalizado em %d ticks.\n", encontrado->tick_saida_linha - encontrado->tick_entrada_linha);
        }
        else if (encontrado->tick_entrada_linha > 0) {
            printf(" Tempo    : %d ticks na fabrica", dados->tick_atual - encontrado->tick_entrada_linha);

            if (encontrado->atividade_atual != NULL) {
                printf(" (Faltam %ds na ativ. atual)\n", encontrado->tempo_restante);
            } else {
                printf("\n");
            }
        }
        else {
            printf(" Tempo    : 0 ticks (Nao entrou na linha).\n");
        }
    } else {
        printf("  Produto ID %03d nao encontrado no sistema.\n", id_buscado);
    }
    printf("========================================================\n");
}