# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)


# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(projeto_fechadura C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(pico-littlefs-master)

# Add executable. Default name is the project name, version 0.1

add_executable(projeto_fechadura 
        main.c  
        dhcpserver/dhcpserver.c
        dnsserver/dnsserver.c
        http.c
        web/login.c
        web/principal.c
        web/principal_admin.c
        hash/sha-256.c
        autenticacao.c
        autentica_senha_fechadura.c
        web/cadastro-usuario.c
        cadastrar_usuario.c
        logs.c
        web/logs-page.c
        controle_fechadura.c
        web/alterar-senha.c
        modificar_senha_fechadura.c
        controle_DS1307.c
        remover_usuario.c
        web/excluir-usuario.c
)

pico_set_program_name(projeto_fechadura "projeto_fechadura")
pico_set_program_version(projeto_fechadura "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(projeto_fechadura 0)
pico_enable_stdio_usb(projeto_fechadura 1)

# Add the standard library to the build
target_link_libraries(projeto_fechadura
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(projeto_fechadura PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        pico-littlefs-master
        pico-littlefs-master/littlefs-lib
        pico-littlefs-master/stdio-lib
)

# Add any user requested libraries
target_link_libraries(projeto_fechadura 
        hardware_spi
        hardware_dma
        hardware_timer
        hardware_clocks
        pico_cyw43_arch_lwip_poll
        pico_stdlib
        littlefs-lib
        stdinit-lib 
        hardware_i2c
        )

pico_add_extra_outputs(projeto_fechadura)

