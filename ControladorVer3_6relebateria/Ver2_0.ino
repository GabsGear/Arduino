#include <DS3231.h>

DS3231  rtc(SDA, SCL);

Time  t;
int analogInput = A0;
float vout = 0.0;
float vin = 0.0;
int value = 0.0;
float R1 = 30000.0; //Resistor 1
float R2 = 7500.0; //Resistor 2
float relac = (R2 / (R1 + R2));   //relaçao divisor de tensao
int alimentacao = 7;
float sTime = 7.55; //Hora inicial
float fTime = 18.00; //Hora final
float vMin = 10.5;
int sDay = 1;
int fDay = 5;

#define medidas 10 //Numero de leituras

void setup() {
  pinMode(analogInput, INPUT);
  pinMode(alimentacao, OUTPUT);
  digitalWrite(alimentacao, HIGH);
  Serial.begin(9600);
  rtc.begin();
  //SETAR A  DATA AQUI
  //rtc.setDOW(WEDNESDAY);   //DIA
  //rtc.setTime(8, 40, 10);   //Hora (24hr)
  //rtc.setDate(8, 10, 2016); //Data MM/DD/AA
}

float lePorta(uint8_t analogInput) {    //Le a porta analogica e retorna a media das medidas
  int sum = 0;
  for (int i = 0; i < medidas; i++) {
    sum += 1.0 * analogRead(analogInput);
    delay(5);
  }
  return sum / (float)medidas;
}

void mostraData() {
  t = rtc.getTime();
  Serial.print("Dia da semana ");
  Serial.print(t.dow, DEC);
  Serial.print(". \n");
  Serial.print(t.date, DEC);
  Serial.print("/");
  Serial.print(rtc.getMonthStr());
  Serial.print("/");
  Serial.print(t.year, DEC);
  Serial.println(".");
  Serial.print(t.hour, DEC);
  Serial.print(":");
  Serial.print(t.min, DEC);
  Serial.print(":");
  Serial.print(t.sec, DEC);
  Serial.println(".");
}

void loop() {
  value = lePorta(analogInput);    //Conversao e calculo do divisor de tensao
  vout = (value * 5.0) / 1024.0;
  vin = vout / relac;

  mostraData();

  Serial.print("Tensao em A0 = ");
  Serial.println(vin, 2);
  Serial.println("  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -");

  if (t.hour > sTime and t.hour < fTime and vin < vMin) {
    digitalWrite(alimentacao, LOW);    //Liga rele 1
    delay(3000);
    digitalWrite(alimentacao, HIGH);   //Desliga rele 1
  }
  delay(1000);
}
