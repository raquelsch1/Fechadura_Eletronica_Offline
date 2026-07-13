#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "pico_hal.h"
#include "hash/sha-256.h"

bool alterar_senha_fechadura(const char *nova_senha) {
    // Gera o hash SHA-256 da nova senha
    uint8_t hash_bin[32];
    calc_sha_256(hash_bin, nova_senha, strlen(nova_senha));

    // Converte o hash binário para string hexadecimal
    char hash_hex[65];  // 64 caracteres + '\0'
    hash_to_hex(hash_bin, hash_hex);

    // Remove o arquivo antigo
    pico_remove("senha_fechadura.txt");

    // Cria o novo arquivo com o novo hash
    int file = pico_open("senha_fechadura.txt", LFS_O_WRONLY | LFS_O_CREAT);
    if (file < 0) {
        printf("Erro ao criar senha_fechadura.txt\n");
        return false;
    }

    int escrito = pico_write(file, hash_hex, strlen(hash_hex));
    pico_close(file);

    // Verifica se escreveu corretamente
    if (escrito != (int)strlen(hash_hex)) {
        printf("Erro ao escrever a nova senha no arquivo\n");
        return false;
    }

    printf("Senha da fechadura alterada com sucesso.\n");
    return true;
}
