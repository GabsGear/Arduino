#include <math.h>
#define MAX_VEL 255 // Velocidade maxima
#define MIN_VEL 10  // Velocidade minima
#define ENTRADAS 2
#define SAIDAS 2
#define NR_NEURON_O 6
#define CAMADAS_O 2

const int motor_1_a = 3;
const int motor_1_b = 5;
const int motor_2_a = 6;
const int motor_2_b = 9;
const int s1 = 11;
const int s2 = 12;

float w_e_o[ENTRADAS + 1][NR_NEURON_O] = {{1.12031, -0.291046, -1.66446, 0.844718, 1.06282, 1.30628},
										   {1.4758, 0.556192, -1.31411, 0.903254, -1.33468, 2.13375},
										   {3.78627, -4.47363, -0.495717, 0.46272, 1.66663, 1.27894}};

float w_o_o[NR_NEURON_O + 1][NR_NEURON_O] = {{-8.14493, 0.193826, 7.77479, -4.80743, -9.46272, -16.1411},
											  {57.4796, 14.3955, -10.9684, 5.33009, 4.87243, -56.2334},
											  {14.3955, 6.2596, -4.27177, 0.904142, 2.03766, -14.3774},
											  {-10.9684, -2.27177, 37.0396, -7.88513, -12.943, 15.9866},
											  {5.33009, -1.09586, -7.88513, 6.31403, 1.20342, -7.27762},
											  {2.87243, 2.03766, -14.943, 3.20342, 10.5122, -8.01566},
											  {-58.2334, -16.3774, 15.9866, -5.27762, -6.01566, 84.0493}};

float w_o_s[NR_NEURON_O + 1][SAIDAS] = {{-16.1816, 1.59551},
										 {-3.99657, -3.77226},
										 {9.95658, -1.61525},
										 {-12.3724, 8.53777},
										 {14.0221, 0.658199},
										 {3.42714, -5.20899},
										 {11.722, 14.3048}};

float saida_o[NR_NEURON_O][CAMADAS_O];
float saida_s[SAIDAS];
float sensor[ENTRADAS];
float speed[ENTRADAS];

float ler_sensor(int sensor);
float f_sigm1oid(float net);
void calcular_saidas(float entradas[ENTRADAS]);

void setup()
{
	Serial.begin(9600);
	pinMode(motor_1_a, OUTPUT);
	pinMode(motor_1_b, OUTPUT);
	pinMode(motor_2_a, OUTPUT);
	pinMode(motor_2_b, OUTPUT);
}

void loop()
{
	for(int i = 0; i < ENTRADAS; i++){
		sensor[i] = ler_sensor(i + 1);
		Serial.println("");
		Serial.print("s1: ");
		Serial.print(sensor[0]);
		Serial.print(" cm | s2: ");
		Serial.print(sensor[1]);
		Serial.print(" cm ");
		sensor[i] = mapSensor(i);
	}

	for(int i = 0; i < ENTRADAS; i++){
		speed[i] = mapSpeed(saida_s[i]*10); 
	}

	calcular_saidas(sensor);
	acelerar(speed[0], speed[1]);
	parar();
	delay(500);
	
}

void parar()
{
  	digitalWrite(motor_1_a, LOW);
  	digitalWrite(motor_1_b, LOW); 
	digitalWrite(motor_2_a, LOW);
  	digitalWrite(motor_2_b, LOW); 
}

float acelerar(float vel_m1, float vel_m2)
{
	analogWrite(motor_1_a, vel_m1);
	analogWrite(motor_2_a, vel_m2);
}

float ler_sensor(int sensor)
{
	if(sensor == 1)
		sensor = s1;
	else
		sensor = s2;
		
	// Emite sinal
	pinMode(sensor, OUTPUT);
	digitalWrite(sensor, HIGH);
	delayMicroseconds(100);
	digitalWrite(sensor, LOW);
	// Configura sensor para receber sinal
	pinMode(sensor, INPUT);
	int tempo = pulseIn(sensor, HIGH);
	return tempo / 2 / 29.4;
}

float f_sigmoid(float net)
{
	return 1 / (1 + exp(-net));
}

float mapSensor(int pos)
{
	sensor[pos] = float(mapfloat(sensor[pos], 3, 310, 0, 1));
}

float mapSpeed(float saida)
{
	return mapfloat(saida, 0, 10, MIN_VEL, MAX_VEL);
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void calcular_saidas(float entradas[ENTRADAS])
{
	int i, j;

	for (i = 0; i < NR_NEURON_O; i++)
	{
		saida_o[i][0] = 0.0;
		saida_o[i][0] += w_e_o[0][i] * 1;

		for (j = 1; j < ENTRADAS + 1; j++)
			saida_o[i][0] += w_e_o[j][i] * entradas[j - 1];

		saida_o[i][0] = f_sigmoid(saida_o[i][0]);
	}

	for (i = 0; i < NR_NEURON_O; i++)
	{
		saida_o[i][1] = 0.0;
		saida_o[i][1] += w_o_o[0][i] * 1;

		for (j = 1; j < NR_NEURON_O + 1; j++)
			saida_o[i][1] += w_o_o[j][i] * saida_o[j - 1][0];

		saida_o[i][1] = f_sigmoid(saida_o[i][1]);
	}

	for (i = 0; i < SAIDAS; i++)
	{
		saida_s[i] = 0.0;
		saida_s[i] += w_o_s[0][i] * 1;

		for (j = 1; j < NR_NEURON_O + 1; j++)
			saida_s[i] += w_o_s[j][i] * saida_o[j - 1][1];

		saida_s[i] = f_sigmoid(saida_s[i]);
	}
}
