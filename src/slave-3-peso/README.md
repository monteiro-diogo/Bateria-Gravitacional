# Firmware: Nó Slave 3 (Peso / Mini 3)

## Descrição do Módulo
Este diretório agrupa o código-fonte referente ao nó periférico "Mini 3", focado na instrumentação espacial e segurança da bateria gravitacional. Ao contrário dos restantes nós focados em dados elétricos, este microcontrolador monitoriza em tempo real a altura e o deslocamento da massa atuadora, assegurando que o Master possui a informação de posição necessária para não exceder os limites mecânicos da estrutura.

## Arquitetura de Ficheiros Fonte (.cpp)

### 1. main.cpp
* **Função**: Máquina de estados de posição e coordenação de amostragem.
* **Descrição**:
  * Regista a identificação do nó na topologia com o valor `ID_MINI = 3`.
  * Instancia e inicializa dois sensores de distância ultrassónicos dedicados aos limites do sistema: `sensorCima` (GPIOs 32/33) e `sensorBaixo` (GPIOs 25/26).
  * O ciclo `loop` opera com base num temporizador não bloqueante de 250 milissegundos. Em cada iteração, recolhe a distância atual de ambos os sensores e invoca a função de transmissão, garantindo uma taxa de atualização posicional de 4 Hz sem comprometer o processamento paralelo.

### 2. comms.cpp
* **Função**: Protocolo de Transmissão de Telemetria (ESP-NOW).
* **Descrição**:
  * Configura a stack de rádio do microcontrolador para operar em modo Station (`WIFI_STA`) sobre o canal 1, assegurando alinhamento de frequência com a rede criada pelo Master.
  * O emparelhamento ocorre de forma transparente (sem encriptação) baseando-se no endereço MAC importado de `segredos.h`.
  * A rotina `sendData` recebe as métricas físicas (distâncias em centímetros) e o identificador do nó, encapsula-os numa estrutura de dados otimizada e procede ao envio. O estado do pacote (sucesso ou falha na receção) é reportado localmente na consola através do evento interrupção de hardware `OnDataSent`.

### 3. hcsr04.cpp
* **Função**: Driver de Aquisição Ultrassónica.
* **Descrição**:
  * Fornece a camada de abstração de hardware para a interação com sensores HC-SR04.
  * A função `lerDistanciaCm` estimula o pino de *trigger* com um pulso exato de 10 microssegundos.
  * Captura a largura do sinal de retorno (*echo*) através da instrução `pulseIn`, limitando a espera a um *timeout* rígido de 30 milissegundos (equivalente ao tempo de viagem do som para além do curso útil) para evitar o bloqueio da *thread* de execução.
  * Processa a conversão temporal para distância física (cm) usando a velocidade do som e aplica uma filtragem de erro estrita, devolvendo -1.0 cm caso ocorra perda de sinal ou leitura acima do limite tolerável (400 cm).