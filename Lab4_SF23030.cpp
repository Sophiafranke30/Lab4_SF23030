// Sophia Franke | 23030
// Lab 4: Señales PWM
// Fecha: 2025-08-15

#include <Arduino.h>
#include <stdint.h>

// ➽──────❥ ❀⊱༺♡༻⊰❀ ➽──────❥
// Definición de pines
#define LED_RED 13
#define LED_GREEN 12
#define LED_BLUE 14

#define PB_Color 18
#define PB_Bright 19

#define delayTime 250

#define PWM_Channel_Red 0
#define PWM_Channel_Green 1
#define PWM_Channel_Blue 2

#define PWM_Freq 100
#define PWM_Resolution 16

// ➽──────❥ ❀⊱༺♡༻⊰❀ ➽──────❥
// Variables Globales
volatile int32_t brightnessred = 0;
volatile int32_t brightnessblue = 0;
volatile int32_t brightnessgreen = 0;
volatile int32_t color = 0;

volatile bool colorButtonPressed = false;
volatile bool brightnessButtonPressed = false;
volatile int32_t lastColorButtonPress = 0;
volatile int32_t lastBrightnessButtonPress = 0;

// ➽──────❥ ❀⊱༺♡༻⊰❀ ➽──────❥
// Prototipos de funciones
void setupLEDs(void);
void setupColor(void);
void setupBrightness(void);
void IRAM_ATTR colorButtonISR(void);
void IRAM_ATTR brightnessButtonISR(void);
void updateLEDs(void);

// ➽──────❥ ❀⊱༺♡༻⊰❀ ➽──────❥
// Setup Principal
void setup() {
  Serial.begin(115200);
  setupLEDs();
  setupColor();
  setupBrightness();

  ledcWrite(PWM_Channel_Red, 0);
  ledcWrite(PWM_Channel_Green, 0);
  ledcWrite(PWM_Channel_Blue, 0);
}

// ➽──────❥ ❀⊱༺♡༻⊰❀ ➽──────❥
// Loop Principal
void loop() {
  if (colorButtonPressed) {
    colorButtonPressed = false;
    color++;
    if (color > 3) color = 0;
    Serial.print("Color cambiado a: ");
    switch (color) {
      case 0: Serial.println("Ningún color"); break;
      case 1: Serial.println("Rojo"); break;
      case 2: Serial.println("Verde"); break;
      case 3: Serial.println("Azul"); break;
    }
    delay(2500); // Para evitar rebotes
  }

  if (brightnessButtonPressed) {
    brightnessButtonPressed = false;
    switch (color) {
      case 1: // Rojo
        brightnessred = (brightnessred + 1) % 5;
        Serial.print("Brillo Rojo: "); Serial.println(brightnessred * 25);
        break;
      case 2: // Verde
        brightnessgreen = (brightnessgreen + 1) % 5;
        Serial.print("Brillo Verde: "); Serial.println(brightnessgreen * 25);
        break;
      case 3: // Azul
        brightnessblue = (brightnessblue + 1) % 5;
        Serial.print("Brillo Azul: "); Serial.println(brightnessblue * 25);
        break;
    }
  }

  updateLEDs();
  delay(100); // Para evitar saturar el monitor serial
}

// ➽──────❥ ❀⊱༺♡༻⊰❀ ➽──────❥
// Funciones Adicionales

void setupLEDs(void) {
  ledcSetup(PWM_Channel_Red, PWM_Freq, PWM_Resolution);
  ledcAttachPin(LED_RED, PWM_Channel_Red);
  ledcWrite(PWM_Channel_Red, 0);

  ledcSetup(PWM_Channel_Green, PWM_Freq, PWM_Resolution);
  ledcAttachPin(LED_GREEN, PWM_Channel_Green);
  ledcWrite(PWM_Channel_Green, 0);

  ledcSetup(PWM_Channel_Blue, PWM_Freq, PWM_Resolution);
  ledcAttachPin(LED_BLUE, PWM_Channel_Blue);
  ledcWrite(PWM_Channel_Blue, 0);
}

void setupColor(void) {
  pinMode(PB_Color, INPUT_PULLUP);
  attachInterrupt(PB_Color, colorButtonISR, FALLING);
}

void setupBrightness(void) {
  pinMode(PB_Bright, INPUT_PULLUP);
  attachInterrupt(PB_Bright, brightnessButtonISR, FALLING);
}

void IRAM_ATTR colorButtonISR(void) {
  uint32_t currentTime = millis();
  if (currentTime - lastColorButtonPress > delayTime) {
    colorButtonPressed = true;
    lastColorButtonPress = currentTime;
  }
}

void IRAM_ATTR brightnessButtonISR(void) {
  uint32_t currentTime = millis();
  if (currentTime - lastBrightnessButtonPress > delayTime) {
    brightnessButtonPressed = true;
    lastBrightnessButtonPress = currentTime;
  }
}

void updateLEDs(void) {
  const int brightnessLevels[] = {0, 16384, 32768, 49152, 65535};
  ledcWrite(PWM_Channel_Red, brightnessLevels[brightnessred]);
  ledcWrite(PWM_Channel_Green, brightnessLevels[brightnessgreen]);
  ledcWrite(PWM_Channel_Blue, brightnessLevels[brightnessblue]);
}