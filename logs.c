#include <stdio.h>
#include <string.h>
#include "pico_hal.h"
#include "logs.h"
#include "controle_DS1307.h"


void atualiza_logs(const char *nome, int acao, const char *tipo_usuario) {
    char data_hora[32];
    obter_data_hora(data_hora, sizeof(data_hora));

    const char *descricao_acao = NULL;
    switch (acao) {
        case 1:
            descricao_acao = "Autenticou no sistema";
            break;
        case 2:
            descricao_acao = "Destrancou fechadura";
            break;
        case 3: 
            descricao_acao = "Trancou fechadura";
            break;
        case 4:
            descricao_acao = "Alterou senha da fechadura";
            break;
        default:
            descricao_acao = "Ação desconhecida";
            break;
    }

    // Determina a descrição do tipo
    const char *tipo_descricao = NULL;
    if (strcmp(tipo_usuario, "admin") == 0) {
        tipo_descricao = "Administrador";
    } else {
        tipo_descricao = "Usuário Comum";
    }

    // Abre/cria arquivo
    int file = pico_open("logs.txt", LFS_O_WRONLY | LFS_O_APPEND);
    if (file < 0) {
        file = pico_open("logs.txt", LFS_O_WRONLY | LFS_O_CREAT);
        if (file < 0) {
            printf("Erro ao criar logs.txt: %d\n", file);
            return;
        }
    }

    // Escreve no formato: nome,tipo,ação,data_hora
    char linha[160];
    snprintf(linha, sizeof(linha), "%s,%s,%s,%s\n", nome, tipo_descricao, descricao_acao, data_hora);

    int written = pico_write(file, linha, strlen(linha));
    if (written < 0) {
        printf("Erro ao escrever em logs.txt\n");
    } else {
        printf("Log registrado: %s", linha);
    }

    pico_close(file);
}

void limpar_logs() {
    int file = pico_open("logs.txt", LFS_O_RDONLY);
    if (file < 0) {
        printf("Erro ao abrir logs.txt para leitura\n");
        return;
    }

    char buffer[128];
    char linha[160];
    int linha_pos = 0;

    char logs_recentes[4096] = {0};

    // Obter data atual do RTC
    char data_atual_str[32];
    obter_data_hora(data_atual_str, sizeof(data_atual_str));
    printf("Data atual detectada: %s\n", data_atual_str);

    int dia_atual, mes_atual, ano_atual;
    sscanf(data_atual_str, "%d/%d/%d", &dia_atual, &mes_atual, &ano_atual);

    int mes_ref = mes_atual - 6;
    int ano_ref = ano_atual;

    if (mes_ref <= 0) {
        mes_ref += 12;
        ano_ref -= 1;
    }

    int lidos;
    while ((lidos = pico_read(file, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < lidos; i++) {
            char c = buffer[i];
            if (c == '\n' || linha_pos >= (int)(sizeof(linha) - 1)) {
                linha[linha_pos] = '\0';

                // Extrai a data do final da linha
                char *ultima_virgula = strrchr(linha, ',');
                if (ultima_virgula && strlen(ultima_virgula) > 10) {
                    int dia_log, mes_log, ano_log;
                    char data_somente[16] = {0};
                    strncpy(data_somente, ultima_virgula + 1, 10); 
                    sscanf(data_somente, "%d/%d/%d", &dia_log, &mes_log, &ano_log);

                    int mantem = 0;

                    if (ano_log > ano_ref) {
                        mantem = 1;
                    } else if (ano_log == ano_ref) {
                        if (mes_log > mes_ref) mantem = 1;
                        else if (mes_log == mes_ref && dia_log >= dia_atual)
                            mantem = 1;
                    }

                    if (mantem) {
                        strncat(logs_recentes, linha, sizeof(logs_recentes) - strlen(logs_recentes) - 2);
                        strncat(logs_recentes, "\n", sizeof(logs_recentes) - strlen(logs_recentes) - 1);
                    }
                }

                linha_pos = 0;
            } else {
                linha[linha_pos++] = c;
            }
        }
    }

    pico_close(file);

    // Reescreve apenas os logs recentes
    file = pico_open("logs.txt", LFS_O_WRONLY | LFS_O_TRUNC);
    if (file >= 0) {
        pico_write(file, logs_recentes, strlen(logs_recentes));
        pico_close(file);
        printf("Logs antigos removidos com sucesso\n");
    } else {
        printf("Erro ao reabrir logs.txt para sobrescrever\n");
    }
    printf("Conteúdo atual do arquivo logs.txt:\n");
        file = pico_open("logs.txt", LFS_O_RDONLY);
        if (file >= 0) {
            char conteudo[128];
            int bytes;
            while ((bytes = pico_read(file, conteudo, sizeof(conteudo) - 1)) > 0) {
                conteudo[bytes] = '\0';
                printf("%s", conteudo);
            }
            pico_close(file);
        } else {
            printf("Erro ao abrir logs.txt para leitura final\n");
        }
}

