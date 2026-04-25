#include "web_server.h"
#include "ina219.h"
#include "segredos.h"
#include "comms.h"
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

WebServer server(80);

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

void iniciarWebServer() {
  Serial.println("A ligar o Disco Rigido Interno (LittleFS)...");
    
  if (!LittleFS.begin(true)) {
    Serial.println("ERRO: Falha ao montar o LittleFS!");
    return;
  }

  // Para DEBUGGING (para veres o que tens gravado no disco da ESP32, se tiveres algo)
  Serial.println("\n--- CONTEUDO DO DISCO ---");
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  bool discoVazio = true;
  
  while(file){
      Serial.print("Encontrado: ");
      Serial.println(file.name());
      discoVazio = false;
      file = root.openNextFile();
  }
  if(discoVazio) {
      Serial.println("ALERTA: O DISCO ESTA COMPLETAMENTE VAZIO!");
  }
  Serial.println("-------------------------\n");

  Serial.println("A iniciar a Rede Wi-Fi do Master...");
  
  WiFi.mode(WIFI_AP_STA); // Força a placa a manter o modo Recetor ativo

  if (String(password).length() > 0) {
    WiFi.softAP(ssid, password, 1); // Canal 1
  } else {
    WiFi.softAP(ssid);
  }

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Rede criada: "); Serial.println(ssid);
  Serial.print("Endereco do Painel: http://"); Serial.println(IP);
  
  // Configuramos as rotas do servidor web
  server.on("/", []() {
    // Verificamos com o nome exato
    if (LittleFS.exists("/index.html")) {
        File file = LittleFS.open("/index.html", "r");
        server.streamFile(file, "text/html");
        file.close();
    } else {
        server.send(404, "text/plain", "ERRO: O ficheiro index.html nao esta gravado no disco da ESP32!");
    }
  });

  server.on("/dados", []() {
    // 1. Lê os dados do INA219 local
    DadosEnergia dados_master = lerDadosSensor();
    
    // 2. Vai buscar os últimos dados recebidos via ESP-NOW
    struct_message mini1 = getDadosMini1();
    struct_message mini2 = getDadosMini2();

    // 3. Monta o JSON com tudo junto
    String json = "{";
    json += "\"v_master\":" + String(dados_master.tensao_V) + ",";
    json += "\"c_master\":" + String(dados_master.corrente_mA) + ",";
    json += "\"p_master\":" + String(dados_master.potencia_mW) + ",";
    
    // Adiciona o Mini 1
    json += "\"v_mini1\":" + String(mini1.tensao_V) + ",";
    json += "\"c_mini1\":" + String(mini1.corrente_mA) + ",";
    json += "\"p_mini1\":" + String(mini1.potencia_mW) + ",";

    // Adiciona o Mini 2
    json += "\"v_mini2\":" + String(mini2.tensao_V) + ",";
    json += "\"c_mini2\":" + String(mini2.corrente_mA) + ",";
    json += "\"p_mini2\":" + String(mini2.potencia_mW);
    json += "}";
    
    server.send(200, "application/json", json);
  });

  server.begin();
  Serial.println("Servidor Web a correr!");
}

void processarWebServer() {
  server.handleClient(); 
}