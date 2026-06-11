#include <stdio.h>
#include <string.h>
#include "pico_hal.h"  
#include "remover_usuario.h"

bool remover_usuario(const char *nome, const char *tipo, const char *usuario) {
    int file = pico_open("usuarios.txt", LFS_O_RDONLY);
    if (file < 0) {
        printf("Erro ao abrir usuarios.txt para leitura\n");
        return false;
    }

    char buffer[128];
    char linha[256];
    int linha_pos = 0;

    char usuarios_atualizados[4096] = {0}; 

    int lidos;
    bool encontrou = false;

    while ((lidos = pico_read(file, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < lidos; i++) {
            char c = buffer[i];
            if (c == '\n' || linha_pos >= (int)(sizeof(linha) - 1)) {
                linha[linha_pos] = '\0';

                char nome_lido[64] = {0};
                char tipo_lido[16] = {0};
                char usuario_lido[64] = {0};
    
                int campos = sscanf(linha, "%63[^,],%15[^,],%63[^,]", nome_lido, tipo_lido, usuario_lido);

                if (campos == 3) {
                    if (strcmp(nome, nome_lido) == 0 &&
                        strcmp(tipo, tipo_lido) == 0 &&
                        strcmp(usuario, usuario_lido) == 0) {
                        encontrou = true;
                    } else {
                        strncat(usuarios_atualizados, linha, sizeof(usuarios_atualizados) - strlen(usuarios_atualizados) - 2);
                        strncat(usuarios_atualizados, "\n", sizeof(usuarios_atualizados) - strlen(usuarios_atualizados) - 1);
                    }
                } else {
                    strncat(usuarios_atualizados, linha, sizeof(usuarios_atualizados) - strlen(usuarios_atualizados) - 2);
                    strncat(usuarios_atualizados, "\n", sizeof(usuarios_atualizados) - strlen(usuarios_atualizados) - 1);
                }

                linha_pos = 0;
            } else {
                linha[linha_pos++] = c;
            }
        }
    }

    pico_close(file);

    if (!encontrou) {
        return false;
    }

    file = pico_open("usuarios.txt", LFS_O_WRONLY | LFS_O_TRUNC);
    if (file < 0) {
        printf("Erro ao abrir usuarios.txt para sobrescrever\n");
        return false;
    }

    pico_write(file, usuarios_atualizados, strlen(usuarios_atualizados));
    pico_close(file);

    return true;
}
