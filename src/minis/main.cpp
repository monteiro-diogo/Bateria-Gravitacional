#include <Arduino.h>
#include "comms.h"
#include "ina226.h"

// ID do Mini (1 ou 2, dependendo de que placa está a gravar)
const int ID_MINI = 1; 

void setup() {
  Serial.begin(115200);
  delay(1000); // Dá tempo ao Serial para conectar

  Serial.println("\n--- ARRANQUE DO MINI (ESP32-C3) ---");
  
  // Inicia o sensor (todo o Scanner I2C e Debugging acontece lá dentro agora)
  iniciarSensor();

  // Inicia as comunicações ESP-NOW
  initComms(); 
  
  Serial.println("Arrancou! A entrar no Loop de leitura...");
}

void loop() {
  static unsigned long ultimoEnvio = 0;
  
  // Envia os dados para o Master apenas a cada 1 segundo (1000 ms)
  if (millis() - ultimoEnvio >= 1000) {  
    ultimoEnvio = millis();

    // 1. Lê o sensor
    DadosEnergia dados = lerDadosSensor();

    // 2. Dispara pelo ar para o Master (que depois mete no Web Server)
    sendData(ID_MINI, dados.tensao_V, dados.corrente_mA, dados.potencia_mW);
  }
}