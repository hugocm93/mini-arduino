/*Nesse mini-projeto, eu controlaria esse LED de acordo
  com a temperatura ou de acordo com um timer estático. O push-button
  e o LED RGB auxiliariam ao usuário escolher como vai ser controlado o
  LED. Ou pela temperatura ou por um timer fixo. Para o projeto
  final, eu usaria o relay com o ventilador, e o teclado numérico para
  definir um tempo personalizado, e o display para exibir opções e exibir
  a contagem regressiva do cronômetro. Extra: Threads e Bluetooth*/

#include<string.h>

#define LED 13
#define RGB_LED_R 12
#define RGB_LED_G 11
#define RGB_LED_B 10

#define SENSOR A0
#define BUTTON1 2

#define INTERVAL 3000
#define TIMEOUT  10000
#define THRESHOLD 27.0

unsigned long lastUpdateTime;
unsigned long runtime;
int oldValue;
int option;
bool lastState;
bool currentState;

char junk;
String inputString = "";

void rgb(char* color);
void buttonChanged(bool state);
float tempInCelsius(int sensorValue);

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(RGB_LED_R, OUTPUT);
  pinMode(RGB_LED_G, OUTPUT);
  pinMode(RGB_LED_B, OUTPUT);
  pinMode(SENSOR, INPUT);
  pinMode(BUTTON1, INPUT);

  oldValue = analogRead(SENSOR);
  lastUpdateTime = millis();
  option = -1;
  runtime = 0;
  lastState = LOW;
  currentState = LOW;
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

  if (Serial.available()) {
    inputString = "";
    while (Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      if (inChar != ' ') {
        inputString += inChar;        //make a string of the characters coming on serial
      }
    }
    Serial.println(inputString);
    while (Serial.available() > 0)
    {
      junk = Serial.read() ;  // clear the serial buffer
    }
    if (inputString == "1") {       //in case of 'a' turn the LED on
      buttonChanged(HIGH);
    }
  }

  //Serial.println(option);

  switch (option) {
    case 1:
      rgb("green");
      Serial.print("Temperature ");
      Serial.println(tempInCelsius(oldValue));
      if (tempInCelsius(oldValue) > THRESHOLD) {
        digitalWrite(LED, HIGH);
      }
      else {
        digitalWrite(LED, LOW);
      }
      break;

    case 2:
      rgb("blue");
      Serial.print("Timer ");
      Serial.println(millis() - runtime);
      if (millis() - runtime > TIMEOUT) {
        digitalWrite(LED, LOW);
        option = -1;
      }
      else {
        digitalWrite(LED, HIGH);
      }
      break;

    default:
      rgb("red");
      runtime = 0;
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
      runtime = 0;
    }
    else {
      option = 2;
      if (runtime == 0) {
        runtime = millis();
      }
    }
    currentState = !currentState;
  }
}

float tempInCelsius(int sensorValue) {
  float mV = (sensorValue) * (5000 / 1024.0);
  return (mV) / 10.;
}

