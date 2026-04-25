#include "comms.h"
#include <WiFi.h>
#include <esp_now.h>

// Variáveis para guardar em memória a última leitura de cada Mini
struct_message ultimaLeituraMini1 = {1, 0.0, 0.0, 0.0};
struct_message ultimaLeituraMini2 = {2, 0.0, 0.0, 0.0};

// Função Callback: O que fazer quando uma mensagem chega do "ar"
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  struct_message dadosRecebidos;
  memcpy(&dadosRecebidos, incomingData, sizeof(dadosRecebidos));
  
  // Guarda no cacifo correto dependendo de quem enviou
  if (dadosRecebidos.id == 1) {
    ultimaLeituraMini1 = dadosRecebidos;
  } else if (dadosRecebidos.id == 2) {
    ultimaLeituraMini2 = dadosRecebidos;
  }
}

void initComms() {
  // ATENÇÃO: Não usamos WiFi.mode() aqui porque o seu web_server.cpp 
  // já trata disso quando cria a rede com WiFi.softAP()
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("ERRO: Falha ao inicializar o ESP-NOW no Master!");
    return;
  }
  
  // Diz ao ESP32 para usar a nossa função OnDataRecv quando chegarem dados
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW Master Iniciado. A escuta...");
}

// Funções "Getter" para o web_server.cpp ir buscar os dados de forma segura
struct_message getDadosMini1() { return ultimaLeituraMini1; }
struct_message getDadosMini2() { return ultimaLeituraMini2; }