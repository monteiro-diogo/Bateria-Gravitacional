#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>

//igualar aos minis
typedef struct struct_message {
  int id;
  float tensao_V;
  float corrente_mA;
  float potencia_mW;
} struct_message;

// Funções para iniciar e aceder aos dados guardados
void initComms();
struct_message getDadosMini1();
struct_message getDadosMini2();

#endif