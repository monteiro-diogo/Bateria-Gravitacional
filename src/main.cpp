#include <Arduino.h>
#include "ina219.h"
#include "web_server.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Configuracao WI-FI ---");

  if (!iniciarSensor()) {
    Serial.println("ERRO: Sensor INA219 não encontrado!");
    while (1) { delay(10); } 
  }
  
  iniciarWebServer(); // Inicia a rede definida no segredos.h
  
  Serial.println("Sistema online!");
  Serial.println("----------------------------------------------");
}

void loop() {
  // 1. Processa os pedidos do site (Wi-Fi) de web_server.cpp
  processarWebServer();
  
  // 2. Imprime no Monitor Série (USB) para podermos visualizarmos o funcionamento no terminal do pc
  static unsigned long ultimoEnvio = 0;
  if (millis() - ultimoEnvio >= 1000) {  // Fazemos isto a cada 1 segundo (1000ms)
    ultimoEnvio = millis();
    DadosEnergia dados = lerDadosSensor();
    
    // Formato tabela (V, C, P) para o teu VSCode
    Serial.print(dados.tensao_fonte_V); Serial.print(" V |\t");
    Serial.print(dados.corrente_mA); Serial.print(" mA |\t");
    Serial.print(dados.potencia_mW); Serial.println(" mW");
  }
}