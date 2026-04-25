#include "ina219.h"
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

bool iniciarSensor() { // Função para iniciar o sensor INA219 ou semelhante
  if (!ina219.begin()) {
    return false;
  }
  return true;
}

DadosEnergia lerDadosSensor() {
  float busvoltage = 0;
  float current_mA = 0;
  float power_mW = 0;
  
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  current_mA = abs(current_mA); // Para não nos dar valores negativos
  power_mW = ina219.getPower_mW();

  // Para evitar ruido
  if (busvoltage < 1.0) {
    busvoltage = 0.0;
    current_mA = 0.0;
  }

  // Preencher o pacote
  DadosEnergia dados_master;
  dados_master.tensao_V = busvoltage;
  dados_master.corrente_mA = current_mA;
  dados_master.potencia_mW = power_mW;

  return dados_master;
}