#ifndef INA226_H
#define INA226_H

struct DadosEnergia {
  float tensao_V;
  float corrente_mA;
  float potencia_mW;
};

bool iniciarSensor();
DadosEnergia lerDadosSensor();

#endif