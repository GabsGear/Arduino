/*******
#CONTROLADOR DE CARGA PARA PAINEL FOTOVOLTÁICO 
#Autor: Gabriel Ghellere
#
#Vide README.txt.
*******/


#include <SD.h>
#include <Arduino.h>
#include <DS3231.h>

#define voltageInputBattery A0 //Sensor A0
#define voltageInput A1 //Sensor A1
#define voltageInputAUX A2 //Sensor A2
#define currentInput A4 //Amperimetro
#define rele1 22 //Porta rele
#define rele2 23 //Porta rele 2
#define rele3 24 //Porta rele 3
#define sTime 7.55 //Hora inicial
#define fTime 18.00 //Hora final
#define vMin 11 //Voltagem minima
#define cMin 5 //Corrente minima
#define medidas 10 //Numero de leituras
#define minutos 300000 //Tempo ligado (ms)
#define sdport 53 //Porta CSK Adaptador SD

File myFile;

/**O arduino ATMEGA possui duas entradas especificas para comunicacao SDA E SLC, portanto se for utili
zar o arduino UNO as portas correspondentes serão A4 e A5 respectivamenet**/

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
float relac = (R2 / (R1 + R2));   //Esse divisor de tensoes atua com o range de 0 a 55V tomar cuidado, para alterar calcular um novo divisor tendo em mente que a saida do Arduino não pode ultrapassar 5v
int mVperAmp = 66;// sensibilidade do sensor de corrente vide datasheet
int RawValue = 0; //valor da leitura analogica da corrente 
int ACSoffset = 2500; //constante da corrente (vide datasheet)
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
	Serial.begin(9600);//inicia porta serial
	rtc.begin(); //inicia o relogio
	Serial.println("Iniciando sd"); //Inicializacao do cartao sd 
	pinMode(sdport, OUTPUT);  

	if (SD.begin(sdport))
		Serial.println("SD iniciado \n");

	//SETAR A  DATA AQUI
	//rtc.setDOW(MONDAY);   //DIA
	//rtc.setTime(9, 26, 10);   //Hora (24hr)
	//rtc.setDate(9, 5, 2016); //Data MM/DD/AA
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
	//vout = map(value 0, 1023, 0, 55) ALTERNATIVO A LINHA ACIMA
	return vout / relac;
}

float calcCorrente(float value) {
	Voltage = map(RawValue, 0, 1023, 0, 5000); //(RawValue / 1024.0) * 5000; // Gets you mV
	return ((Voltage - ACSoffset) / mVperAmp);
}

void mostraData() {
	t = rtc.getTime();
	myFile = SD.open("log.txt", FILE_WRITE);

	/**A data sera impressa somente no cartao sd, para mostrar no serial utilizar Serial.print ao 
	invez de myFile.print**/

	if (myFile) { 
		myFile.print(t.dow, DEC);
		myFile.print(" ");
		myFile.print(t.date, DEC);
		myFile.print(" ");
		myFile.print(rtc.getMonthStr());
		myFile.print(" ");
		myFile.print(t.year, DEC);
		myFile.print(" ");
		myFile.print(t.hour, DEC);
		myFile.print(" ");
		myFile.print(t.min, DEC);
		myFile.print(" ");
		myFile.print(t.sec, DEC);
		myFile.close();
	}
	else Serial.println("Sem sd, os dados nao serao salvos no log");
}

void loop() {
	//tensao;
	valueA0 = lePorta(voltageInputBattery);    //Conversao e calculo do divisor de tensao
	vinA0 = calcTensao(valueA0);

	valueA1 = lePorta(voltageInput);
	vinA1 = calcTensao(valueA1);
	//corrente
	RawValue = lePorta(currentInput);
	Amps = calcCorrente(RawValue);

	mostraData();

	Serial.print("Tensao em A0 (BATERIA) = ");
	Serial.println(vinA0, 2);
	Serial.print("Tensao em A1 = ");
	Serial.println(vinA1, 2);
	Serial.print("Tensao em A2 = ");
	Serial.println(vinA2, 2);
	Serial.print("Corrente = ");
	Serial.println(Amps, 2);
	Serial.println("  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -");

	//GRAVANDO NO SD
	myFile = SD.open("log.txt", FILE_WRITE);
	if (myFile) {
		myFile.print(" ");
		myFile.print(vinA0, 2);
		myFile.print(" ");
		myFile.print(vinA1, 2);
		myFile.print(" ");
		myFile.print(vinA2, 2);
		myFile.print(" ");
		myFile.print(Amps, 2);
		myFile.println(" ");
		myFile.close();
	}

	/***O carregador agirá somente em horas e voltagem específicas 
	(sTime e fTime, e a voltagem acima de vMin***/

	if (t.hour > sTime and t.hour < fTime and vinA0 < vMin) {

		/**Abre rele que carrega a bateria e os reles de isolamento **/
		
		digitalWrite(rele1, LOW);    
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

		delay(minutos); //Tempo de carregamento

		/*}
		else {
		break;
		}
		}*/

		//Desliga o rele, finaliza carregamento

		digitalWrite(rele1, HIGH);  
		digitalWrite(rele2, HIGH);
		digitalWrite(rele3, HIGH);
		Serial.print("Carregamento finalizado");
	}
	delay(3000);
}
