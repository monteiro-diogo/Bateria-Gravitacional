# Firmware: Nó Slave 1 (Cidade / Mini 1)

## Descrição do Módulo
Este diretório abrange o código-fonte desenvolvido para o nó periférico "Mini 1", dedicado à monitorização isolada dos parâmetros elétricos da rede consumidora. O microcontrolador opera como emissor na arquitetura de rede, recolhendo dados do hardware local e transmitindo-os para o coordenador central.

## Arquitetura de Ficheiros Fonte (.cpp)

### 1. main.cpp
* **Função**: Ponto de entrada do firmware e rotina de controlo principal.
* **Descrição**:
  * Define a identidade do nó na rede através da constante `ID_MINI` com o valor 1.
  * A rotina de inicialização (`setup`) estabelece a ligação série, aguarda um atraso de estabilização, e invoca as configurações iniciais do sensor I2C e da interface de rede .
  * O ciclo infinito (`loop`) executa uma interrogação periódica ao sensor de energia.
  * Quando o intervalo de telemetria definido em `INTERVALO_TELEMETRIA_MS` é atingido, os valores de tensão, corrente e potência são registados na consola série e enviados para o Master via ESP-NOW.

### 2. comms.cpp
* **Função**: Protocolo de Comunicação Sem Fios.
* **Descrição**:
  * Configura o microcontrolador no modo Station (`WIFI_STA`) para suportar a infraestrutura ESP-NOW, forçando a operação no canal de rádio 1[cite: 2].
  * Regista o nó Master como o destino da comunicação (Peer) recorrendo ao endereço MAC previamente definido nas configurações de segurança (`segredos.h`)[cite: 2].
  * Implementa a função de transmissão `sendData`, que empacota a identificação do nó e os dados numéricos na estrutura global `meusDados` e dispara o pacote para a rede[cite: 2].
  * Inclui um mecanismo de validação por interrupção (`OnDataSent`) que confirma o estado de entrega do pacote, alertando o sistema caso o Master não acuse a receção[cite: 2].

### 3. ina226.cpp
* **Função**: Aquisição Analógica de Dados de Potência.
* **Descrição**:
  * Estabelece a interface de comunicação I2C nos pinos SDA 4 e SCL 5, visando o endereço de hardware 0x44[cite: 3].
  * Integra um algoritmo de *I2C Scanner* na fase de arranque, que varre o barramento de comunicações para atestar a presença física do sensor antes de prosseguir com a execução do código[cite: 3].
  * A rotina de configuração calibra os parâmetros de leitura para a utilização de um shunt externo de 0.009 Ohms, parametrizando adicionalmente um teto de segurança de 8 Amperes[cite: 3].
  * A função `lerDadosINA226` interroga os registos do sensor, converte as grandezas para Volts, miliamperes e miliwatts, e aplica uma filtragem de ruído mecânico que suprime tensões espúrias inferiores a 0.05V[cite: 3].