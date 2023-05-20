# SSC0142 - Trabalho de Arduino, Emissor Par

## Objetivo

O objetivo deste projeto consiste em estabelecer uma comunicação síncrona entre duas plataformas Arduino. A transmissão da mensagem será realizada de forma serial e síncrona.

🇺🇸: The aim of this project is to achieve synchronous communication between two Arduino platforms. The message will be transmitted in a serial and synchronous manner. 

## Membros

- **João Antônio Misson Milhorim - 11834331**
- **Pedro Kenzo Muramatsu Carmo - 11796451**
- **Pedro Rossi Silva Rodrigues - 11871775**

## Descrição do Projeto

No presente trabalho, o Arduino emissor será responsável por receber um caractere pela sua porta serial (por meio do Serial Monitor da plataforma) e transmiti-lo de forma síncrona e serial ao Arduino receptor. O Arduino receptor, por sua vez, irá imprimir os caracteres recebidos em sua porta serial, exibindo-os para o usuário por meio do Serial Monitor. É importante destacar que os caracteres serão transmitidos de acordo com o código [ASCII](https://upload.wikimedia.org/wikipedia/commons/d/dd/ASCII-Table.svg).

## Requisitos

- A comunicação entre as duas plataformas deverá ser síncrona, ou seja, deve haver uma sincronização entre o emissor e o receptor através de um clock;

- A comunicação deverá implementar o controle de erros através de um bit de paridade;

- A comunicação deverá implementar um handshake simples entre o emissor e o receptor.

## Vídeo de Apresentação

O vídeo de apresentação do projeto se encontra neste [LINK]().
