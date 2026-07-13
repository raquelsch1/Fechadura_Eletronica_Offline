#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "pico_hal.h"
#include "logs-page.h"

extern char http_response[8192];

void create_logs_page() {
    char conteudo_tabela[8192] = {0};
    char linhas[100][160]; 
    int total_linhas = 0;

    int file = pico_open("logs.txt", LFS_O_RDONLY);
    if (file < 0) {
        snprintf(conteudo_tabela, sizeof(conteudo_tabela),
                 "<tr><td colspan=\"4\">Erro ao abrir o arquivo de logs.</td></tr>");
    } else {
        char buffer[128];
        char linha[160] = {0};
        int linha_pos = 0;

        int read_bytes;
        while ((read_bytes = pico_read(file, buffer, sizeof(buffer))) > 0) {
            for (int i = 0; i < read_bytes; i++) {
                char c = buffer[i];
                if (c == '\n' || linha_pos >= (int)(sizeof(linha) - 1)) {
                    linha[linha_pos] = '\0';
                    if (total_linhas < 100 && strlen(linha) > 0) {
                        strncpy(linhas[total_linhas], linha, sizeof(linhas[0]) - 1);
                        total_linhas++;
                    }
                    memset(linha, 0, sizeof(linha));
                    linha_pos = 0;
                } else {
                    linha[linha_pos++] = c;
                }
            }
        }
        pico_close(file);

        for (int i = total_linhas - 1; i >= 0; i--) {
            char nome[64], tipo[16], acao[64], data[32];
            if (sscanf(linhas[i], "%63[^,],%15[^,],%63[^,],%31[^\n]", nome, tipo, acao, data) == 4) {
                char linha_html[512];
                snprintf(linha_html, sizeof(linha_html),
                    "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>",
                    nome, tipo, acao, data);
                strncat(conteudo_tabela, linha_html, sizeof(conteudo_tabela) - strlen(conteudo_tabela) - 1);
            }
        }
    }

    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>\n"
        "<html lang=\"pt-BR\">\n"
        "<head>\n"
        "<meta charset=\"UTF-8\">\n"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "<title>Logs do Sistema</title>\n"
        "<style>\n"
        "body {\n"
        "    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n"
        "    background: linear-gradient(135deg, #f5f7fa 0%%, #c3cfe2 100%%);\n"
        "    margin: 0;\n"
        "    padding: 0;\n"
        "    color: #333333;\n"
        "}\n"
        ".container-administrador {\n"
        "    display: flex;\n"
        "    flex-direction: column;\n"
        "    min-height: 100vh;\n"
        "}\n"
        ".barra-navegacao {\n"
        "    background-color: #166088;\n"
        "    color: white;\n"
        "    padding: 0.8rem 1rem;\n"
        "    display: flex;\n"
        "    justify-content: space-between;\n"
        "    align-items: center;\n"
        "}\n"
         ".botao-logout {\n"
        "  background: #e53935;\n"
        "  color: white;\n"
        "  padding: 0.4rem 0.8rem;\n"
        "  border: none;\n"
        "  border-radius: 6px;\n"
        "  font-size: 0.9rem;\n"
        "  font-weight: bold;\n"
        "  cursor: pointer;\n"
        "  transition: opacity 0.2s;\n"
        "}\n"
        ".botao-logout:hover {\n"
        "  opacity: 0.9;\n"
        "}\n"
        ".lado-esquerdo {\n"
                "  display: flex;\n"
                "  align-items: center;\n"
                "  gap: 0.6rem;\n"
        "}\n"
        ".lado-esquerdo form {\n"
        "  display: contents;\n"
        "}\n"
        ".marca {\n"
        "    font-size: 1.2rem;\n"
        "    font-weight: bold;\n"
        "}\n"
        ".menu-navegacao {\n"
        "    display: flex;\n"
        "    gap: 0.8rem;\n"
        "}\n"
        ".item-navegacao {\n"
        "    color: white;\n"
        "    text-decoration: none;\n"
        "    padding: 0.4rem 0.8rem;\n"
        "    border-radius: 4px;\n"
        "    font-size: 0.9rem;\n"
        "    transition: background-color 0.2s;\n"
        "}\n"
        ".item-navegacao:hover {\n"
        "    background-color: rgba(255, 255, 255, 0.1);\n"
        "}\n"
        ".conteudo-principal {\n"
        "    flex: 1;\n"
        "    display: flex;\n"
        "    flex-direction: column;\n"
        "    align-items: center;\n"
        "    padding: 2rem;\n"
        "}\n"
        "form {\n"
        "    margin-bottom: 1rem;\n"
        "}\n"
        "button {\n"
        "    background-color: #f44336;\n"
        "    color: white;\n"
        "    padding: 0.5rem 1rem;\n"
        "    border: none;\n"
        "    border-radius: 6px;\n"
        "    font-size: 0.9rem;\n"
        "    cursor: pointer;\n"
        "}\n"
        ".tabela-logs {\n"
        "    width: 100%%;\n"
        "    max-width: 800px;\n"
        "    border-collapse: collapse;\n"
        "    background: white;\n"
        "    border-radius: 12px;\n"
        "    overflow: hidden;\n"
        "    box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);\n"
        "}\n"
        ".tabela-logs th, .tabela-logs td {\n"
        "    padding: 12px;\n"
        "    border-bottom: 1px solid #ddd;\n"
        "    text-align: left;\n"
        "    font-size: 0.95rem;\n"
        "}\n"
        ".tabela-logs th {\n"
        "    background-color: #166088;\n"
        "    color: white;\n"
        "}\n"
        "</style>\n"
        "</head>\n"
        "<body>\n"
        "<div class=\"container-administrador\">\n"
        "<nav class=\"barra-navegacao\">\n"
        "<div class=\"lado-esquerdo\">\n"
        "  <form method=\"POST\" action=\"/logout\">\n"
        "    <button type=\"submit\" class=\"botao botao-logout\">Sair</button>\n"
        "  </form>\n"
        "  <div class=\"marca\">Logs do Sistema</div>\n"
        "</div>\n"
        "<div class=\"menu-navegacao\">\n"
        "<a href=\"/principal_admin\" class=\"item-navegacao\">Página Principal</a>\n"
        "</div>\n"
        "</nav>\n"
        "<div class=\"conteudo-principal\">\n"
        "<form method=\"POST\" action=\"/limpar-logs\">\n"
        "<button type=\"submit\">Limpar logs antigos</button>\n"
        "</form>\n"
        "<table class=\"tabela-logs\">\n"
        "<thead>\n"
        "<tr>\n"
        "<th>Nome</th>\n"
        "<th>Tipo de Usuário</th>\n"
        "<th>Ação</th>\n"
        "<th>Data e Hora</th>\n"
        "</tr>\n"
        "</thead>\n"
        "<tbody>\n"
        "%s\n"
        "</tbody>\n"
        "</table>\n"
        "</div>\n"
        "</div>\n"
        "<script>\n"
        "setTimeout(function() {\n"
        "    window.location.reload();\n"
        "}, 10000);\n"
        "</script>\n"
        "</body>\n"
        "</html>\n",
        conteudo_tabela);
}
