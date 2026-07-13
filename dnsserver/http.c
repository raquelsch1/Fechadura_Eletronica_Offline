#include "http.h"
#include "alarme.h"
#include <string.h>
#include <stdio.h>
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

char http_response[1024]; // buffer para armazenar a resposta HTTP

// gera a página html
void create_http_response() {
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang=\"pt-BR\">"
        "<head>"
        "<meta charset=\"UTF-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "<title>Ativar Alarme de Emergência</title>"
        "<style>"
        "body {"
        "  background-color:rgb(255, 255, 255);"
        "  color: white;"
        "  font-family: Arial, sans-serif;"
        "  text-align: center;"
        "  padding: 50px;"
        "}"
        "h1 {"
        "  color: #ff4c4c;"
        "  margin-top: 50px;"
        "}"
        ".button {"
        "  display: inline-block;"
        "  padding: 15px 30px;"
        "  margin: 20px;"
        "  font-size: 18px;"
        "  text-decoration: none;"
        "  color: white;"
        "  border-radius: 8px;"
        "  transition: background-color 0.3s;"
        "}"
        ".on {"
        "  background-color: #ff1c1c;"
        "}"
        ".on:hover {"
        "  background-color: #e60000;"
        "}"
        ".off {"
        "  background-color: #555;"
        "}"
        ".off:hover {"
        "  background-color: #333;"
        "}"
        "</style>"
        "</head>"
        "<body>"
        "<h1>Ativar Alarme de Emergência</h1>"
        "<a class=\"button on\" href=\"/emergencia\">Ligar</a>"
        "<a class=\"button off\" href=\"/desligar\">Desligar</a>"
        "</body>"
        "</html>\r\n");
}

// trata as requisições http
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        return ERR_OK;
    }
    
    char *request = (char *)p->payload; // converte dados recebidos para string
    printf("Requisição recebida: %s\n", request); // imprime no monitor serial a requisição recebida

    // se a requisição get for do tipo emergência
    if (strstr(request, "GET /emergencia")) {
        if (!alarme_ativo && alarme_id == 0) { // parâmetros de segurança para não ativar o alarme quando a página for recarregada
            alarme_id = add_alarm_in_ms(0, alarme_callback, NULL, true); // ativa um alarme imediato para a função que vai acionar o led e buzzer
            exibe_display(); // chama exibe_display para atualizar o display
        }

        // após um get, redireciona para página princial 
        snprintf(http_response, sizeof(http_response),
            "HTTP/1.1 303 See Other\r\n"
            "Location: /\r\n"
            "Content-Length: 0\r\n\r\n");
    } else if (strstr(request, "GET /desligar")) { // se a requisição for do tipo desligar
        cancel_alarm(alarme_id); // cancela o alarme
        alarme_id = 0; // zera o id do alarme
        alarme_ativo = false; // seta a flag do alarme como falsa
        desliga_tudo(); // desliga o led e o buzzer
        exibe_display(); // chama exibe_display para atualizar a mensagem

        // redireciona para a página principal
        snprintf(http_response, sizeof(http_response),
            "HTTP/1.1 303 See Other\r\n"
            "Location: /\r\n"
            "Content-Length: 0\r\n\r\n");
    } else {
        create_http_response(); // gera o html
    }

    // envia a resposta e libera o buffer
    tcp_write(tpcb, http_response, strlen(http_response), TCP_WRITE_FLAG_COPY);
    pbuf_free(p);
    return ERR_OK;
}

static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_callback);
    return ERR_OK;
}

void start_http_server(void) {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) return;
    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) return;
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, connection_callback);
}
