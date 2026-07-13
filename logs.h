#ifndef LOGS_H
#define LOGS_H

void atualiza_logs(const char *nome, int acao, const char *tipo_usuario);
void limpar_logs();
void obter_data_hora(char *data_hora_out, size_t tamanho);

#endif
