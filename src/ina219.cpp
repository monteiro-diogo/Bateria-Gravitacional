#include "ina219.h"
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

bool iniciarSensor() {
  if (!ina219.begin()) {
    return false;
  }
  return true;
}

DadosEnergia lerDadosSensor() {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  // A nossa "Zona Morta" para manter o ecrã limpo a zeros
  if (busvoltage < 1.0) {
    busvoltage = 0.0;
    shuntvoltage = 0.0;
    current_mA = 0.0;
  }

  // Preencher o pacote
  DadosEnergia dados;
  dados.tensao_fonte_V = busvoltage;
  dados.tensao_shunt_mV = shuntvoltage;
  dados.tensao_carga_V = loadvoltage;
  dados.corrente_mA = current_mA;
  dados.potencia_mW = power_mW;

  return dados;
}