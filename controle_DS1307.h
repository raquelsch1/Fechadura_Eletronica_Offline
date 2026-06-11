#ifndef CONTROLE_DS1307_H
#define CONTROLE_DS1307_H

uint8_t bcd_para_decimal(uint8_t valor);
void obter_data_hora(char *data_hora_out, size_t tamanho);
void inicializar_relogio_gpio(void);
void configurar_relogio(uint8_t *data_hora);

#endif
