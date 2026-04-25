#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>

// A estrutura tem de ser EXATAMENTE igual à do Master
typedef struct struct_message {
  int id;
  float tensao_V;
  float corrente_mA;
  float potencia_mW;
} struct_message;

void initComms();
void sendData(int id, float tensao_V, float corrente_mA, float potencia_mW);

#endif