#ifndef AUTENTICACAO_H
#define AUTENTICACAO_H

#include <stdbool.h>

bool autenticar(const char *username, const char *senha, char *tipo_out, char *nome_out);

#endif
