# Sistema de Monitorização e Gestão de Energia - Bateria Gravitacional

## Descrição Geral
Este repositório contém o firmware e a estrutura de diretórios de um sistema de gestão de energia desenvolvido no âmbito da unidade curricular de Projeto II da Licenciatura em Engenharia Eletrotécnica e de Computadores da Faculdade de Ciências e Tecnologia da Universidade de Coimbra. O projeto foi desenvolvido de forma colaborativa por Diogo Monteiro, Francisco Santos, Pedro Carvalho e Tomás Francisco.

O sistema foca-se na monitorização contínua da rede elétrica e na ativação automática de baterias gravitacionais de reserva, com o objetivo de assegurar a continuidade da produção elétrica em situações de falha na rede pública. 

## Demonstração
<img width="270" height="270" alt="demo" src="https://github.com/user-attachments/assets/0afb6257-05f0-4b40-8968-27cb2aa6c2c4" />

## Arquitetura do Sistema
O projeto encontra-se particionado em quatro ambientes de compilação distintos geridos pelo PlatformIO, operando sobre o framework Arduino e a plataforma Espressif 32 (monitor serial configurado para 115200 baud rate)
* **Master**: Nó de controlo principal e agregação de dados.
* **Slave 1 (Cidade)**: Nó dedicado à monitorização dos parâmetros da rede consumidora.
* **Slave 2 (Fonte)**: Nó dedicado à monitorização dos parâmetros da fonte de alimentação elétrica.
* **Slave 3 (Peso)**: Nó de atuação e controlo do mecanismo de peso gravitacional.

## Estrutura de Diretórios
* `/data`: Ficheiros base para o servidor web e interface (carregados via LittleFS).
* `/include`: Ficheiros de cabeçalho (`.h`) globais do projeto.
* `/src`: Código-fonte em C++, segmentado por cada nó da arquitetura.

## Credenciais e Segurança
Informações sensíveis, como credenciais de rede, estão omitidas do controlo de versões, devendo ser criadas localmente no ficheiro `segredos.h`.

## Licença
Este projeto encontra-se licenciado sob a MIT License.
