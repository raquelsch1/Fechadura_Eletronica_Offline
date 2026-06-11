#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "pico_hal.h"
#include "hash/sha-256.h"
#include "autentica_senha_fechadura.h"

bool autentica_senha_fechadura(const char *senha) {
    // Gera o hash SHA-256 da senha recebida
    uint8_t hash_bin[32];
    calc_sha_256(hash_bin, senha, strlen(senha));

    // Converte o hash binário para string hexadecimal
    char hash_hex[65];
    hash_to_hex(hash_bin, hash_hex);

    // Abre o arquivo com o hash da senha salva
    int file = pico_open("senha_fechadura.txt", LFS_O_RDONLY);
    if (file < 0) {
        printf("Erro ao abrir senha_fechadura.txt\n");
        return false;
    }

    char linha[80] = {0};  // suficiente para 64 chars + quebra de linha
    int total = pico_read(file, linha, sizeof(linha) - 1);
    pico_close(file);

    // Remove quebra de linha ou espaços no final
    for (int i = 0; i < (int)strlen(linha); i++) {
        if (linha[i] == '\n' || linha[i] == '\r' || isspace(linha[i])) {
            linha[i] = '\0';
            break;
        }
    }

    // Compara o hash calculado com o hash armazenado
    return strcmp(linha, hash_hex) == 0;
}
