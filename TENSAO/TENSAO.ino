/**********
  #Programa para utilização do Arduino para medir tensões DC o circuito é constituidopor um divisor de tensao, associação de dois resistores em série.
  #A referência do arduino é tomada em vOut e a fórmula para o divisor de tensão é vOut = vIn(R1)/(RI+R2) o programa realiza automaticamente
  #nas variaveis vin e vout. Basta mudar o valor dos resistores pelos qur deseja usar (jogando na formula atente-se que vOut não pode ultrapassar 5V
  #pode ocorrer queima do controlador. O programa esta pre configurado com uma associação de um resistor de 1M e um de 100k assim resultando em um alcanse de 0 a 55vDC
  #Autor: Gabriel Ghellere 19/08/2016
********/

//Numero de vezes que o programa fará a leitura da porta analogica de referencia
#define medidas 100

//Definindo porta analogica A0 de referencia
#define analogInput A0

float vout = 0.0;
float vin = 0.0;
int value = 0.0;
float R1 = 1000000; //Resistor 1
float R2 = 100000; //Resistor 2
float relac = (R2 / (R1 + R2));   //relaçao divisor de tensao (da fórmula...)

void setup() {
  Serial.begin(9600);
  pinMode(analogInput, INPUT);
}

float lePorta(uint8_t analogInput) {    //Le a porta analogica e retorna a media das medidas
  int sum = 0;
  for (int i = 0; i < medidas; i++) {
    sum += 1.0 * analogRead(analogInput);
    delay(1);
  }
  return sum / (float)medidas;
}

void loop() {
  value = lePorta(analogInput);    //Conversao e calculo do divisor de tensao
  /*A leitura analogica é  um numero de 0 a 1023 onde 0 = 0V na entrada e 1023 = 5V portanto esse valor deve ser convertido, usando uma regra de 3 simples
    (pode ser usada a função map atente-se de configurar Vin se optar por ultiliza-la no meu caso 55V)
  */
  vout = (value * 5.0) / 1024.0;
  //vout = map(value, 0, 1023, 0, 55); //Exemplo da função map
  vin = vout / relac;
  Serial.print("tensao = "); //Imprime na serial com delay de 1 segundo entre as medições
  Serial.println(vin, 3);
  delay(1000); 
}
