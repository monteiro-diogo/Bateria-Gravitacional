#ifndef INA219_H
#define INA219_H

struct DadosEnergia {
  float tensao_V;
  float corrente_mA;
  float potencia_mW;
};

bool iniciarSensor();

DadosEnergia lerDadosSensor();

#endif