#ifndef INA219_H
#define INA219_H

struct DadosEnergia {
  float tensao_fonte_V;
  float tensao_shunt_mV;
  float tensao_carga_V;
  float corrente_mA;
  float potencia_mW;
};

bool iniciarSensor();
DadosEnergia lerDadosSensor();

#endif