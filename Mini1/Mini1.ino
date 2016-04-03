/*Nesse mini-projeto, eu controlaria esse LED verde de acordo
  com a temperatura ou de de acordo com um timer estático. O push-button
  e o LED RGB auxiliariam ao usuário escolher como vai ser controlado o
  LED verde. Ou pela temperatura ou por um timer fixo. Para o projeto
  final, eu usaria o relay com o ventilador, e o teclado numérico para
  definir um tempo personalizado, e o display para exibir opções e exibir
  a contagem regressiva do cronômetro. Extra: Threads*/

#include<string.h>

#define GREEN_LED 13
#define RGB_LED_R 12
#define RGB_LED_G 11
#define RGB_LED_B 10

#define SENSOR A0
#define BUTTON1 2

#define INTERVAL 3000
#define TIMEOUT 60000
#define THRESHOLD 45

unsigned long lastUpdateTime;
int oldValue;
int option;
int runtime;
bool lastState;
bool currentState;

void rgb(char* color);
void buttonChanged(bool state);

void setup() {
  Serial.begin(9600);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RGB_LED_R, OUTPUT);
  pinMode(RGB_LED_G, OUTPUT);
  pinMode(RGB_LED_B, OUTPUT);
  pinMode(SENSOR, INPUT);
  pinMode(BUTTON1, INPUT);

  oldValue = analogRead(SENSOR);
  lastUpdateTime = millis();
  option = -1;
  runtime = -1;
  lastState = LOW;
  state = LOW;
}

void loop() {
  /*Somente atualiza de tempos em tempos*/
  if (millis() >= lastUpdateTime + INTERVAL) {
    oldValue = analogRead(SENSOR);
    lastUpdateTime = millis();
  }

  bool aux = digitalRead(BUTTON1);
  if (aux != lastState) {
    lastState = aux;
    buttonChanged(aux);
  }

  Serial.println(option);

  switch (option) {
    case 1:
      rgb("red");
      if (oldValue > THRESHOLD) {
        digitalWrite(GREEN_LED, HIGH);
      }
      else {
        digitalWrite(GREEN_LED, LOW);
      }
      break;

    case 2:
      rgb("blue");
      if (runtime > TIMEOUT + millis()) {
        digitalWrite(GREEN_LED, LOW);
        option = -1;
      }
      else {
        digitalWrite(GREEN_LED, HIGH);
      }
      break;

    default:
      rgb("green");
  }

}

void rgb(char* color) {
  if (!strcmp(color, "green")) {
    digitalWrite(RGB_LED_R, LOW);
    digitalWrite(RGB_LED_G, HIGH);
    digitalWrite(RGB_LED_B, LOW);
  }
  else if (!strcmp(color, "blue")) {
    digitalWrite(RGB_LED_R, LOW);
    digitalWrite(RGB_LED_G, LOW);
    digitalWrite(RGB_LED_B, HIGH);
  }
  else if (!strcmp(color, "red")) {
    digitalWrite(RGB_LED_R, HIGH);
    digitalWrite(RGB_LED_G, LOW);
    digitalWrite(RGB_LED_B, LOW);
  }
}

void buttonChanged(bool state) {
  if (state) {
    if (currentState) {
      option = 1;
      runtime = -1;
    }
    else {
      option = 2;
      if (runtime == -1) {
        runtime = millis();
      }
    }
    currentState = !currentState;
  }
}

