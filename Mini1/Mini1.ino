/*Nesse mini-projeto, eu controlaria esse LED verde de acordo
com a temperatura ou de de acordo com um timer estático. O push-button
e o LED RGB auxiliariam ao usuário escolher como vai ser controlado o
LED verde. Ou pela temperatura ou por um timer fixo. Para o projeto 
final, eu usaria o relay com o ventilador, e o teclado numérico para 
definir um tempo personalizado, e o display para exibir opções e exibir 
a contagem regressiva do cronômetro.*/

#define GREEN_LED 12
#define RGB_LED 11

#define SENSOR A0
#define BUTTON1 2

unsigned long lastTime = millis();
int oldValue = analogRead(SENSOR);


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  /*Somente atualiza de 3 em 3 segundos*/
  int sensorValue = oldValue;
  if(millis() >= lastTime + INTERVAL){
    sensorValue = analogRead(SENSOR);
    Serial.println(sensorValue);

    lastTime = millis();
    oldValue = sensorValue;
  }

}
