#include "pico/stdlib.h"
#include "controle_fechadura.h"

#define RELE 18
#define LED_VM 13
#define LED_VD 11

void init_fechadura() {
    gpio_init(RELE);
    gpio_set_dir(RELE, GPIO_OUT);
    gpio_put(RELE, 0); 

    gpio_init(LED_VM);
    gpio_set_dir(LED_VM, GPIO_OUT);
    gpio_put(LED_VM, 1); 

    gpio_init(LED_VD);
    gpio_set_dir(LED_VD, GPIO_OUT);
    gpio_put(LED_VD, 0);
}

void destranca_fechadura() {
    gpio_put(RELE, 1); 
    sleep_ms(100); 
    gpio_put(LED_VM, 0);
    gpio_put(LED_VD, 1);
}

void tranca_fechadura() {
    gpio_put(RELE, 0); 
    sleep_ms(100);
    gpio_put(LED_VM, 1); 
    gpio_put(LED_VD, 0); 
}