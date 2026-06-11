#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "pico_hal.h"  
#include "hash/sha-256.h"      
#include "cadastrar_usuario.h" 

bool cadastrar_usuario(const char *nome, const char *tipo, const char *usuario, const char *senha) {
    // Gera hash SHA-256 da senha
    uint8_t hash_bin[32];
    calc_sha_256(hash_bin, senha, strlen(senha));

    // Converte o hash para string hexadecimal
    char hash_hex[65];
    hash_to_hex(hash_bin, hash_hex);

    // Abre o arquivo para adicionar ao final
    int file = pico_open("usuarios.txt", LFS_O_WRONLY | LFS_O_APPEND);
    if (file < 0) {
        printf("Erro ao abrir usuarios.txt para escrita.\n");
        return false;
    }

    // Monta a linha com o novo usuário
    char linha[256];
    snprintf(linha, sizeof(linha), "%s,%s,%s,%s\n", nome, tipo, usuario, hash_hex);

    // Escreve no arquivo
    int written = pico_write(file, linha, strlen(linha));
    pico_close(file);

    if (written < 0) {
        printf("Erro ao escrever no arquivo de usuários.\n");
        return false;
    }

    printf("Usuário cadastrado com sucesso: %s\n", linha);
    return true;
}
