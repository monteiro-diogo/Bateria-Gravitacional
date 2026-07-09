# Firmware: Nó Master (Controlador Central)

## Descrição do Módulo
Este diretório contém o código-fonte responsável por gerir o nó principal do sistema da bateria gravitacional. O Master atua como o cérebro da rede: cria a infraestrutura de comunicação (Access Point Wi-Fi e ESP-NOW), gere os dados fornecidos pelos nós periféricos (Minis), serve a interface gráfica ao utilizador (via web server e LittleFS) e controla diretamente a atuação eletromecânica de carga/descarga e dissipação de energia.

## Resumo dos Ficheiros Fonte (.cpp)

### 1. web_server.cpp
* **Função**: Gestão de rede Wi-Fi e Servidor Web Embebido.
* **Descrição**: 
  * Inicia o sistema de ficheiros LittleFS para carregar a interface gráfica estática (`index.html`, CSS, JS).
  * Configura o ESP32 como um Access Point (`WIFI_AP_STA`) para permitir a ligação direta de dispositivos de monitorização.
  * Disponibiliza a rota REST API `/dados`, que constrói e devolve um objeto JSON em tempo real contendo as leituras agregadas de tensão, corrente e potência do Master e dos nós periféricos, bem como o estado global do sistema.

### 2. comms.cpp
* **Função**: Protocolo de Comunicação Sem Fios (ESP-NOW).
* **Descrição**: 
  * Inicializa e gere a receção de pacotes de dados via ESP-NOW (`OnDataRecv`).
  * Processa a telemetria proveniente dos três nós escravos (identificados pelos IDs 1, 2 e 3) e armazena os dados em estruturas de memória global (`struct_message`), permitindo que o resto do sistema aceda instantaneamente à última leitura de cada Mini.

### 3. ibt2.cpp
* **Função**: Controlo de Potência e Atuação Eletromecânica.
* **Descrição**:
  * Controla os drivers de ponte H IBT-2 através de modulação PWM (frequência de 25kHz a 8 bits).
  * **Motor Principal**: Inclui lógica avançada de proteção. Implementa proteção anti-stall (bloqueio mecânico), cortando a alimentação se a corrente medida no Mini 1 ultrapassar os 2000 mA. Efetua regulação dinâmica do duty cycle para manter o equivalente a 12V no motor face a flutuações da fonte.
  * **Circuito de Dissipação**: Gere um segundo driver IBT-2 acoplado a uma resistência de cimento de 10 Ohms para dissipar o excesso de energia em situações de sobrecarga.

### 4. ina226.cpp
* **Função**: Aquisição de Telemetria Local.
* **Descrição**:
  * Estabelece comunicação I2C com o sensor de potência INA226. Inclui uma rotina de *I2C Scanner* no arranque para diagnosticar falhas de ligação física.
  * Configurado especificamente para operar com um shunt de 0.009 Ohms, parametrizando um limite de corrente de 8A por segurança.
  * Filtra ruído (ignora tensões residuais abaixo de 0.05V) e calcula a potência em tempo real (em mW).

### 5. sg90.cpp
* **Função**: Controlo do Servomotor.
* **Descrição**: 
  * Abstração para o controlo de um servomotor padrão (SG90) via sinal PWM a 50Hz (resolução de 16 bits). Mapeia ângulos de 0 a 180 graus em valores de *duty cycle* compreendidos entre 1638 e 8192, contendo também rotinas de calibração/teste automático.

### 6. hardware.cpp
* **Função**: Inicialização Centralizada de Periféricos.
* **Descrição**: 
  * Concentra as chamadas de inicialização dos vários módulos de hardware físicos (`iniciarINA226`, `iniciarSG90`, `iniciarIBT2`, `iniciarDissipacao`), assegurando que as abstrações estão configuradas de forma sequencial antes da entrada no ciclo principal de execução.

### 7. telemetry.cpp
* **Função**: Registo e Depuração de Dados (Logging).
* **Descrição**: 
  * Agrega as informações elétricas e mecânicas de toda a infraestrutura (dados locais do Master, potência dos Mini 1 e 2, e distâncias lidas pelo Mini 3).
  * Imprime um relatório formatado na porta série respeitando um intervalo temporal (`INTERVALO_TELEMETRIA_MS`), permitindo diagnosticar a totalidade do sistema via consola.