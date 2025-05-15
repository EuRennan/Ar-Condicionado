/* Exemplo básico de uso da tela de tinta eletrônica
 *
 * Heltec Vision Master E290
 *
 * Este código exibe "Hello World" no display ePaper.
 *
 * Biblioteca: https://github.com/HelTecAutomation/Heltec_ESP32
 * Suporte: support@heltec.cn
 */

#include "HT_DEPG0290BxS800FxX_BW.h"

// Inicializa o display com a pinagem usada
DEPG0290BxS800FxX_BW display(5, 4, 3, 6, 2, 1, -1, 6000000); // rst, dc, cs, busy, sck, mosi, miso, freq

#define DIRECTION ANGLE_0_DEGREE // Define a orientação da tela (0°)
/* Rotação da tela
 * ANGLE_0_DEGREE
 * ANGLE_90_DEGREE
 * ANGLE_180_DEGREE
 * ANGLE_270_DEGREE
 */

/* Funções para controle de alimentação do display */
void VextON() {
  pinMode(18, OUTPUT);
  digitalWrite(18, HIGH);
}

void VextOFF() { // Vext desligado por padrão
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando display...");

  VextON();               // Liga o regulador de tensão do display
  delay(100);             // Aguarda estabilização
  display.init();         // Inicializa o display
  display.screenRotate(DIRECTION); // Configura a rotação
  display.setFont(ArialMT_Plain_16); // Configura a fonte usada para texto
  display.setTextAlignment(TEXT_ALIGN_CENTER); // Alinha o texto ao centro

  // Limpa o display
  display.clear();
  
  // Exibe "Hello World" no centro do display
  display.drawString(display.width() / 2, display.height() / 2 - 10, "Hello World");
  display.display(); // Atualiza o conteúdo do display
}

void loop() {
  // O código não precisa de ações repetitivas, o loop fica vazio
}
