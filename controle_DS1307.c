#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições do DS1307
#define PORTA_I2C       i2c1
#define PINO_SDA        18
#define PINO_SCL        19
#define ENDERECO_DS3231 0x68


static uint8_t bcd_para_decimal(uint8_t valor) {
    return ((valor >> 4) * 10) + (valor & 0x0F);
}


void obter_data_hora(char *data_hora_out, size_t tamanho)
{
    uint8_t reg = 0x00;
    uint8_t dados[7];

    i2c_write_blocking(PORTA_I2C, ENDERECO_DS3231,
                       &reg, 1, true);

    i2c_read_blocking(PORTA_I2C, ENDERECO_DS3231,
                      dados, 7, false);

    dados[0] = bcd_para_decimal(dados[0] & 0x7F);
    dados[1] = bcd_para_decimal(dados[1]);
    dados[2] = bcd_para_decimal(dados[2] & 0x3F);
    dados[3] = bcd_para_decimal(dados[3]);
    dados[4] = bcd_para_decimal(dados[4]);
    dados[5] = bcd_para_decimal(dados[5] & 0x1F);
    dados[6] = bcd_para_decimal(dados[6]);

    snprintf(data_hora_out,
             tamanho,
             "%02d/%02d/20%02d %02d:%02d:%02d",
             dados[4],
             dados[5],
             dados[6],
             dados[2],
             dados[1],
             dados[0]);
}

void inicializar_relogio_gpio(void)
{
    i2c_init(PORTA_I2C, 100000);

    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(PINO_SDA);
    gpio_pull_up(PINO_SCL);
}

uint8_t decimal_para_bcd(uint8_t valor) {
    return ((valor / 10) << 4) | (valor % 10);
}

void configurar_relogio(uint8_t *data)
{
    uint8_t buffer[8];

    buffer[0] = 0x00;

    for (int i = 0; i < 7; i++)
        buffer[i + 1] = decimal_para_bcd(data[i]);

    i2c_write_blocking(PORTA_I2C,
                       ENDERECO_DS3231,
                       buffer,
                       8,
                       false);
}
