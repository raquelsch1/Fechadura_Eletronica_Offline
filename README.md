# Fechadura Eletrônica com Interface Web e Operação Local

## Descrição

Este projeto consiste no desenvolvimento de uma fechadura eletrônica com interface web embarcada e funcionamento totalmente local, utilizando a Raspberry Pi Pico W. O sistema permite o gerenciamento de usuários, autenticação de acesso e acionamento da fechadura por meio de uma interface web acessada através de uma rede Wi-Fi local criada pelo próprio dispositivo.

## Problema que o Projeto Resolve

O projeto busca solucionar o problema do controle de acesso a ambientes de forma segura, prática e independente de conexão com a internet.

Sistemas tradicionais baseados em chaves físicas apresentam limitações, como perda, cópia não autorizada ou esquecimento das chaves. Além disso, normalmente não oferecem mecanismos de rastreabilidade, dificultando a identificação de quem acessou determinado ambiente e em qual momento isso ocorreu.

A solução proposta permite o controle de acesso por usuários autenticados, registrando tentativas de acesso e aberturas da fechadura, proporcionando maior segurança, monitoramento e autonomia operacional mesmo em locais sem acesso à internet.

## Componentes Utilizados

- Raspberry Pi Pico W (BitDogLab)
- LED RGB
- Módulo Relé
- Fechadura elétrica
- RTC DS3231

## Funcionalidades Implementadas

- Criação de ponto de acesso Wi-Fi local
- Interface web embarcada
- Cadastro e gerenciamento de usuários
- Autenticação de acesso
- Acionamento da fechadura por relé
- Registro de logs de acesso
- Operação totalmente offline
- Integração de módulo RTC para registro preciso de data e hora nos logs
- Aprimoramento da interface web
  
## Tecnologias Utilizadas

- Linguagem C
- Raspberry Pi Pico SDK
- LittleFS
- Servidor HTTP embarcado
- HTML
- CSS
- JavaScript
