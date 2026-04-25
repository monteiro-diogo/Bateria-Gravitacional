#include "ina226.h"
#include <Wire.h>
#include <INA226.h>

// A biblioteca do Rob Tillaart exige o endereço I2C
INA226 ina226(0x44); 

bool iniciarSensor() {
  Serial.println("\n--- A INICIAR O SENSOR INA226 ---");
  
  // Inicializa o I2C nos pinos corretos (SDA=4, SCL=5). confirma de acordo com o teu hardware
  Wire.begin(4, 5);

  // --- DEBUG: SCANNER I2C ---
  Serial.println("A procurar dispositivos I2C nos pinos 4 e 5...");
  byte count = 0;
  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission(i);
    // Se o dispositivo responder, o endereçamento foi bem-sucedido e substituímos na linha 6
    if (Wire.endTransmission() == 0) {
      Serial.print("Encontrado sensor no endereco: 0x");
      Serial.println(i, HEX);
      count++;
    }
  }
  
  if (count == 0) {
    Serial.println("ERRO FISICO: Nenhum sensor encontrado! Verifica os fios e a alimentacao.");
    while(1) { delay(100); } // Pára a placa aqui para evitar loop de erros
  }
  Serial.println("---------------------------------");
  // --- FIM DO SCANNER I2C ---

  // Iniciar a Biblioteca
  if (!ina226.begin()) { 
    Serial.println("ERRO: Biblioteca INA226 falhou no begin()!"); 
    while(1) { delay(10); } // Pára a placa
  }
  
  // --- CONFIGURAÇÃO CRÍTICA DO NOVO SHUNT R009 ---
  // Definimos o teto para 8 Amperes por segurança, com o shunt de 0.009 Ohms
  // No máximo colocamos 9A para evitar o risco de queimar o sensor, mesmo que o sistema possa chegar a 10A em picos.
  ina226.setMaxCurrentShunt(8.0, 0.009); 
  
  Serial.println("SUCESSO: Sensor INA226 pronto!");
  return true;
}

DadosEnergia lerDadosSensor() {
  DadosEnergia dados;
  
  // Lê os valores da biblioteca
  dados.tensao_V = ina226.getBusVoltage();
  
  float corrente_A = ina226.getCurrent(); 
  dados.corrente_mA = abs(corrente_A * 1000.0); 
  
  float potencia_W = ina226.getPower(); 
  dados.potencia_mW = abs(potencia_W * 1000.0);

  // Se falhar
  // dados.potencia_mW = dados.tensao_V * dados.corrente_mA;

  // Filtro de ruído (Podes descomentar quando o sistema estiver no local final)
  if (dados.tensao_V < 1.0) {
    dados.tensao_V = 0.0;
    dados.corrente_mA = 0.0;
    dados.potencia_mW = 0.0;
  }

  return dados;
}