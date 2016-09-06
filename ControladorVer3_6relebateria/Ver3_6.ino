#include <Arduino.h>
#include <DS3231.h>
#define voltageInputBattery A0 //Sensor A0
#define voltageInput A1 //Sensor A1
#define voltageInputAUX A2 //Sensor A2
#define currentInput A14 //Amperimetro
#define rele1 22 //Porta rele
#define rele2 23 //Porta rele 2
#define rele3 24 //Porta rele 3
#define sTime 7.55 //Hora inicial
#define fTime 18.00 //Hora final
#define vMin 11 //Voltagem minima
#define cMin 5 //Corrente minima
#define medidas 10 //Numero de leituras
#define minutos 300000 //Tempo ligado


DS3231  rtc(SDA, SCL);

Time  t;

float vinA0 = 0.0;
float vinA1 = 0.0;
float vinA2 = 0.0;
float Current = 0.0;
int valueCurrent = 0.0;
int valueA0 = 0.0;
int valueA1 = 0.0;
int valueA2 = 0.0;
float R1 = 1000000.0; //Resistor 1
float R2 = 100000.0; //Resistor 2
float relac = (R2 / (R1 + R2));   //relaçao divisor de tensao
int mVperAmp = 66;// sensibilidade do sensor de corrente
int RawValue = 0;
int ACSoffset = 2500;
double Voltage = 0;
double Amps = 0;

void setup() {
  pinMode(voltageInputBattery, INPUT);
  pinMode(voltageInput, INPUT);
  pinMode(voltageInputAUX, INPUT);
  pinMode(currentInput, INPUT);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  digitalWrite(rele1, HIGH);
  digitalWrite(rele2, HIGH);
  digitalWrite(rele3, HIGH);
  analogReference(DEFAULT);
  Serial.begin(9600);
  rtc.begin();

  //SETAR A  DATA AQUI
  rtc.setDOW(TUESDAY);   //DIA
  rtc.setTime(8, 28, 10);   //Hora (24hr)
  rtc.setDate(6, 9, 2016); //Data MM/DD/AA
}

float lePorta(uint8_t analogInput) {    //Le a porta analogica e retorna a media das medidas
  int sum = 0;
  for (int i = 0; i < medidas; i++) {
    sum += 1.0 * analogRead(analogInput);
    delay(5);
  }
  return sum / (float)medidas;
}

float calcTensao(float value) {
  float vout = 0.0;
  vout = (value * 5.0) / 1024.0;
  return vout / relac;
}

float calcCorrente (float value) {
  Voltage = map(RawValue, 0, 1023, 0, 5000); //(RawValue / 1024.0) * 5000; // Gets you mV
  return ((Voltage - ACSoffset) / mVperAmp);
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
  //tensao;
  valueA0 = lePorta(voltageInputBattery);    //Conversao e calculo do divisor de tensao
  vinA0 = calcTensao(valueA0);

  valueA1 = lePorta(voltageInput);
  vinA1 = calcTensao(valueA1);

  RawValue = lePorta(currentInput);
  Amps = calcCorrente(RawValue);

  mostraData();

  Serial.print("Tensao em A0 (BATERIA) = ");
  Serial.println(vinA0, 2);
  Serial.print("Tensao em A1 = ");
  Serial.println(vinA1, 2);
  Serial.print("Tensao em A2 = ");
  Serial.println (vinA2, 2);
  Serial.print("Corrente = ");
  Serial.println(Amps, 2);
  Serial.println("  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -");

  if (t.hour > sTime and t.hour < fTime and vinA0 < vMin) {
    digitalWrite(rele1, LOW);    //Liga rele 1
    digitalWrite(rele2, LOW);
    digitalWrite(rele3, LOW);
    
    Serial.print("Carregando...");

    //while ( Amps < 3) {
    /* if (t.hour > sTime and t.hour < fTime) {
       RawValue = lePorta(currentInput);
       Amps = calcCorrente(RawValue);
       Serial.println(RawValue);
       Serial.print("Corrente = ");
       Serial.println(Amps, 2);
       delay(5000);**/
    delay(minutos);
    /*}
      else {
      break;
      }
      }*/
    digitalWrite(rele1, HIGH);   //Desliga rele 1
    digitalWrite(rele2, HIGH);
    digitalWrite(rele3, HIGH);
    Serial.print("Carregamento finalizado");
  }
  delay (3000);
}
