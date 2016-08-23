/**********
  #Programa para utilização do Arduino para medir tensões DC o circuito é constituido por um divisor de tensao, associação de dois resistores em série.
  #A referência do arduino é tomada em vOut e a fórmula para o divisor de tensão é vOut = vIn(R1)/(RI+R2) o programa realiza automaticamente
  #nas variaveis vin e vout. Basta mudar o valor dos resistores pelos qur deseja usar (jogando na formula atente-se que vOut não pode ultrapassar 5V
  #pode ocorrer queima do controlador. O programa esta pre configurado com uma associação de um resistor de 1M e um de 100k assim resultando em um alcanse de 0 a 55vDC
  #Autor: Gabriel Ghellere 19/08/2016
********/

#define voltageInputBattery A0 //Sensor A0
#define medidas 100

float vinA0 = 0.0;
int valueA0 = 0.0;
float R1 = 1000000; //Resistor 1
float R2 = 100000; //Resistor 2
float relac = (R2 / (R1 + R2));   //relaçao divisor de tensao

void setup() {
  pinMode(voltageInputBattery, INPUT);
  Serial.begin(9600);
}

float lePorta(uint8_t analogInput) {    //Le a porta analogica e retorna a media das medidas
  int sum = 0;
  for (int i = 0; i < medidas; i++) {
    sum += 1.0 * analogRead(analogInput);
    delay(1);
  }
  return sum / (float)medidas;
}

float calcTensao(float value) {
  float vout = 0.0;
  vout = (value * 5.0) / 1024.0;
  return vout / relac;
}

void loop() {
  //tensao;
  valueA0 = lePorta(voltageInputBattery);    //Conversao e calculo do divisor de tensao
  vinA0 = calcTensao(valueA0);

  Serial.print("Tensao em A0 (BATERIA) = ");
  Serial.println(vinA0, 2);
  delay (3000);
}
