#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições do DS1307
#define PORTA_I2C       i2c0
#define PINO_SDA        8
#define PINO_SCL        9
#define ENDERECO_DS1307 0x68


static uint8_t bcd_para_decimal(uint8_t valor) {
    return ((valor >> 4) * 10) + (valor & 0x0F);
}


void obter_data_hora(char *data_hora_out, size_t tamanho) {  
    uint8_t dados[7];
    uint8_t registrador = 0x00;

    i2c_write_blocking(PORTA_I2C, ENDERECO_DS1307, &registrador, 1, true);
    i2c_read_blocking(PORTA_I2C, ENDERECO_DS1307, dados, 7, false);

    for (int i = 0; i < 7; i++) {
        dados[i] = bcd_para_decimal(dados[i]);
    }

    snprintf(data_hora_out, tamanho, "%02d/%02d/20%02d %02d:%02d:%02d",
             dados[4], dados[5], dados[6], // dia, mês, ano
             dados[2], dados[1], dados[0]  // hora, minuto, segundo
    );
}

void inicializar_relogio_gpio(void) {
    i2c_init(PORTA_I2C, 100 * 1000); 
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SDA);
    gpio_pull_up(PINO_SCL);
}

uint8_t decimal_para_bcd(uint8_t valor) {
    return ((valor / 10) << 4) | (valor % 10);
}

void configurar_relogio(uint8_t *data_hora) {
    uint8_t dados[8] = {0x00}; 
    for (int i = 0; i < 7; i++) {
        dados[i + 1] = decimal_para_bcd(data_hora[i]);
    }
    i2c_write_blocking(PORTA_I2C, ENDERECO_DS1307, dados, 8, false);
}
