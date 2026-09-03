#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtos.h"


#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define PAUSA_MS(ms) Sleep(ms)
    double pegar_tempo_segundos() {
        return (double)GetTickCount() / 1000.0;
    }
    void configurar_terminal() {}
    void restaurar_terminal() {}
    int tem_tecla() { return _kbhit(); }
    char pegar_tecla() { return (char)_getch(); }
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
    #include <sys/time.h>

    #define PAUSA_MS(ms) usleep((ms) * 1000)

    struct termios old_tio;

    double pegar_tempo_segundos() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (double)tv.tv_sec + ((double)tv.tv_usec / 1000000.0);
    }

    void configurar_terminal() {
        struct termios new_tio;
        tcgetattr(STDIN_FILENO, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }

    void restaurar_terminal() {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    }

    int tem_tecla() {
        int ch = getchar();
        if (ch != EOF) {
            ungetc(ch, stdin);
            return 1;
        }
        return 0;
    }

    char pegar_tecla() { return getchar(); }
#endif


int main() {
    Dados dados;
    char entrada[20] = "entrada.txt" ;
    iniciar(&dados, entrada);


    double tempo_por_tick = 0.1; // 0.1 segundos reais = 1 Tick na fábrica
    double tempo_anterior = pegar_tempo_segundos();
    int simulacao_rodando = 1;

    configurar_terminal();
    printf("\n=== SIMULADOR INICIADO (Pressione 'P' para pausar o tempo) ===\n");

    while (simulacao_rodando && dados.tick_atual < dados.limite_seg && (dados.produtos_concluidos + dados.produtos_descartados) < dados.meta_produtos) {

        if (tem_tecla()) {
            char tecla = pegar_tecla();

            if (tecla == 'p' || tecla == 'P') {
                restaurar_terminal();
                int opcao = -1;

                do {
                    printf("\n========================================================\n");
                    printf("               PAUSADO NO TICK %05d                    \n", dados.tick_atual);
                    printf("========================================================\n");
                    printf(" 1. Visao Global da Fabrica (Todas as etapas e filas)\n");
                    printf(" 2. Ver Resumo das Etapas\n");
                    printf(" 3. Buscar Etapa (por ID ou Nome)\n");
                    printf(" 4. Ver Atividades de uma Etapa\n");
                    printf(" 5. Ver Atividade de uma Etapa\n");
                    printf(" 6. Buscar Produto (Rastreador por ID)\n");
                    printf(" 7. Gerar Relatorio Parcial Agora\n");
                    printf(" 8. Avancar 1 Tick Manualmente (Passo a Passo)\n");
                    printf(" 9. Mudar Velocidade da Simulacao\n");
                    printf(" 10. Continuar Execucao Automatica\n");
                    printf(" 0. Encerrar Simulacao Imediatamente\n");
                    printf("--------------------------------------------------------\n");
                    printf("Escolha: ");

                    if (scanf("%d", &opcao) != 1) {
                        while(getchar() != '\n');
                        continue;
                    }

                    switch(opcao) {
                        case 1:
                            mostrar_linha(dados.primeira_etapa);
                            break;

                        case 2:
                            mostrar_etapas(dados.primeira_etapa);
                            break;

                        case 3: {
                            char termo_etapa[50];
                            printf("Digite o ID ou Nome da Etapa: ");
                            scanf("%s", termo_etapa);

                            int id_etapa = atoi(termo_etapa);

                            if (id_etapa > 0) {
                                mostrar_etapa_id(dados.primeira_etapa, id_etapa);
                            } else {
                                mostrar_etapa_nome(dados.primeira_etapa, termo_etapa);
                            }
                            break;
                        }

                        case 4: {
                            char termo[50];
                            printf("Digite o ID ou Nome da Etapa para listar as atividades: ");
                            scanf("%s", termo);

                            int id_busca = atoi(termo);

                            if (id_busca > 0) {
                                mostrar_atividades_id(dados.primeira_etapa, id_busca);
                            } else {
                                mostrar_atividades_nome(dados.primeira_etapa, termo);
                            }
                            break;
                        }

                        case 5: {
                            char t_etapa[50];
                            char t_ativ[50];

                            printf("Digite o ID ou Nome da Etapa: ");
                            scanf("%s", t_etapa);
                            printf("Digite o ID ou Nome da Atividade: ");
                            scanf("%s", t_ativ);

                            int id_e = atoi(t_etapa);
                            int id_a = atoi(t_ativ);

                            if (id_e > 0 && id_a > 0) {
                                mostrar_atividade_id_id(dados.primeira_etapa, id_a, id_e);
                            }
                            else if (id_e > 0 && id_a == 0) {
                                mostrar_atividade_nome_id(dados.primeira_etapa, t_ativ, id_e);
                            }
                            else if (id_e == 0 && id_a > 0) {
                                mostrar_atividade_id_nome(dados.primeira_etapa, id_a, t_etapa);
                            }
                            else {
                                mostrar_atividade_nome_nome(dados.primeira_etapa, t_ativ, t_etapa);
                            }
                            break;
                        }

                        case 6: {
                            int id_prod;
                            printf("Digite o ID do Produto que deseja rastrear: ");
                            scanf("%d", &id_prod);
                            buscar_produto(&dados, id_prod);
                            break;
                        }


                        case 7: {
                            char nome_relatorio[100];

                            sprintf(nome_relatorio, "relatorio_tick_%d.txt", dados.tick_atual);
                            criar_relatorio(&dados, nome_relatorio, entrada);
                            printf("\n Relatorio parcial '%s' gerado com sucesso!\n", nome_relatorio);
                            break;
                        }

                        case 8:
                            avancar_tick(&dados);
                            printf("\n Tick avancado. O relogio agora marca: %d\n", dados.tick_atual);
                            break;

                        case 9:
                            printf("Digite o novo tempo do tick em segundos reais (ex: 0.5): ");
                            scanf("%lf", &tempo_por_tick);
                            printf("\nVelocidade alterada para %.2f segundos por tick.\n", tempo_por_tick);
                            break;

                        case 10:
                            printf("\nRetomando simulacao em tempo real...\n");
                            break;

                        case 0:
                            simulacao_rodando = 0;
                            printf("\n Encerrando simulacao por ordem do usuario...\n");
                            break;

                        default:
                            printf("\n Opcao invalida.\n");
                    }

                } while (opcao != 9 && opcao != 0);

                configurar_terminal();
                tempo_anterior = pegar_tempo_segundos();
            }
        }


        if (simulacao_rodando) {
            double tempo_atual = pegar_tempo_segundos();
            double tempo_decorrido = tempo_atual - tempo_anterior;

            if (tempo_decorrido >= tempo_por_tick) {


                avancar_tick(&dados);

                tempo_anterior += tempo_por_tick;

                if (dados.tick_atual % 10 == 0) {
                    printf("\r[Processando] Tick: %05d | Produtos Processados: %d/%d (Prontos: %d | Sucata: %d)",
                           dados.tick_atual,
                           (dados.produtos_concluidos + dados.produtos_descartados),
                           dados.meta_produtos,
                           dados.produtos_concluidos,
                           dados.produtos_descartados);
                    fflush(stdout);
                }
            }
            PAUSA_MS(1);
        }
    }


    restaurar_terminal();

    printf("\n\n========================================================\n");
    printf("                  SIMULACAO CONCLUIDA                   \n");
    printf("========================================================\n");

        if ((dados.produtos_concluidos + dados.produtos_descartados) >= dados.meta_produtos) {
            printf("MOTIVO: Todo o lote inicial foi processado.\n");
            printf("Resultado: %d concluidos com sucesso e %d descartados por falha.\n", dados.produtos_concluidos, dados.produtos_descartados);
        } else if (dados.tick_atual >= dados.limite_seg) {
            printf("MOTIVO: Limite de tempo estourado (Tick %d alcancado).\n", dados.limite_seg);
        } else {
            printf("MOTIVO: Interrompido manualmente pelo usuario.\n");
        }

    char nome_final[100];
    sprintf(nome_final, "relatorio_final_%s.txt", dados.nome_simulacao);
    criar_relatorio(&dados, nome_final, entrada);
    printf("Relatorio definitivo gerado: %s\n", nome_final);

    finalizar(&dados);

    return 0;
}