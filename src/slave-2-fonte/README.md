# Firmware: Nó Slave 2 (Fonte / Mini 2)

## Descrição do Módulo
Este diretório compreende o código-fonte relativo ao nó periférico "Mini 2". Este microcontrolador está posicionado a montante do sistema, com a finalidade exclusiva de monitorizar a fonte de geração ou alimentação elétrica. Atua de forma contínua a extrair métricas de potência e envia esses dados via rádio para o nó central.

## Arquitetura de Ficheiros Fonte (.cpp)

### 1. main.cpp
* **Função**: Ciclo de execução principal e orquestração de leituras.
* **Descrição**:
  * Regista a identificação do nó na topologia da rede através da constante de identificação `ID_MINI` com o valor 2.
  * O bloco de inicialização aplica um tempo de estabilização mecânica (`DELAY_ARRANQUE_MS`) antes de arrancar a configuração de rotinas do barramento I2C e de interface wireless.
  * O ciclo `loop` opera com base em intervalos temporizados (`INTERVALO_TELEMETRIA_MS`). Esta abordagem garante que a interrogação ao sensor e a emissão por rádio ocorrem a uma frequência definida, evitando a saturação do barramento I2C, a colisão de pacotes na rede e o bloqueio do processador.

### 2. comms.cpp
* **Função**: Camada de Comunicação e Transporte (ESP-NOW).
* **Descrição**:
  * Configura o hardware rádio do microcontrolador no modo `WIFI_STA` (Station), fixando a transmissão no canal físico de Wi-Fi 1 para garantir sincronismo de frequência com o Master.
  * Efetua o registo em memória (`esp_now_add_peer`) do endereço MAC de destino do nó principal, importado das configurações de segurança (`segredos.h`).
  * A função de transmissão (`sendData`) codifica os parâmetros elétricos e a identificação de hardware numa estrutura otimizada (`struct_message`) e injeta os dados na rede.
  * Incorpora uma interrupção de *callback* (`OnDataSent`) que devolve o código de estado da operação (`ESP_NOW_SEND_SUCCESS` ou falha), possibilitando um sistema de confirmação e rastreamento da integridade da comunicação rádio com o Master.

### 3. ina219.cpp
* **Função**: Driver de Aquisição do Sensor de Potência.
* **Descrição**:
  * Gere a infraestrutura de medição, abstraindo a biblioteca `Adafruit_INA219`. Mapeia a comunicação I2C para os pinos GPIO 4 (SDA) e 5 (SCL) da placa.
  * A rotina `lerDadosINA219` interroga os registos de conversão analógico-digital para extrair a tensão diferencial e a corrente circulante, computando localmente a potência instantânea (em mW).
  * Inclui uma camada de condicionamento de sinal via software, introduzindo um filtro de *noise-floor* que força os parâmetros a zero perante leituras de tensão de barramento inferiores a 0.05V, evitando a propagação de ruído na ausência de carga ou de sinal da fonte.