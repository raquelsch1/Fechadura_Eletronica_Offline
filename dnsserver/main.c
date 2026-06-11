#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "dhcpserver/dhcpserver.h"
#include "inc/ssd1306.h"
#include "inc/display_utils.h"
#include "inc/big_string_drawer.h"
#include "lwip/ip4_addr.h"

// arquivos de cabeçalho para a lógica do alarme e do servidor http
#include "alarme.h"
#include "http.h"

// struct para configurações de rede
typedef struct {
    ip_addr_t gw;
    ip_addr_t mask;
} NET_CONFIG_T;

// Variáveis globais de rede
NET_CONFIG_T net_config;
dhcp_server_t dhcp_server;

// identificação SSID e senha do ponto de acesso
#define WIFI_SSID "alarme_wifi"
#define WIFI_PASS "alarme123"

// pinos i2c do display oled
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// buffer e estrutura do display
uint8_t ssd[1024];
struct render_area frame_area;

int main() {
    stdio_init_all();
    sleep_ms(2000);
    printf("Iniciando sistema...\n");

    // incializando led
    gpio_init(LED_V);
    gpio_set_dir(LED_V, GPIO_OUT);
    gpio_put(LED_V, 0);

    // inicializando buzzer
    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_put(BUZZER, 0);

    // chama a função de pwm do buzzer
    pwm_init_buzzer(BUZZER);

    // configurações do display
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();

    // configurações da área do display
    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;
    calculate_render_area_buffer_length(&frame_area);

    // Wi-Fi
    printf("Inicializando Wi-Fi...\n");
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar Wi-Fi\n");
        return 1;
    }

    // define IP e máscara do ponto de acesso
    IP4_ADDR(&net_config.gw, 192, 168, 4, 1);
    IP4_ADDR(&net_config.mask, 255, 255, 255, 0);
   
    // inicializa o servidor DHCP
    dhcp_server_init(&dhcp_server, &net_config.gw, &net_config.mask);

    //Ativa o modo Access Point com SSID e senha definidos
    printf("Ativando Access Point...\n");
    cyw43_arch_enable_ap_mode(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK);  
    printf("IP configurado: %s\n", ip4addr_ntoa((const ip4_addr_t *)&net_config.gw)); // exibe o ip configurado para o access point
    sleep_ms(1000);

    start_http_server(); // starta o servidor http

    exibe_display(); // chama a função para exibir textos no display 

    while (true) {
        cyw43_arch_poll(); // mantém o Wi-Fi funcionando
    }

    // nunca alcançado, mas boa prática
    dhcp_server_deinit(&dhcp_server);
    cyw43_arch_deinit();
    return 0;
}
