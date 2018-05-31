#include <math.h>
#define MAX_VEL 255 // Velocidade maxima
#define MIN_VEL 10  // Velocidade minima
#define ENTRADAS 2
#define SAIDAS 2
#define NR_NEURON_O 6
#define CAMADAS_O 2

const int motor_1_a = 3; //input 1
const int motor_1_b = 5; //input 2
const int motor_2_a = 6; //input 3
const int motor_2_b = 9; //input 4
const int s1 = 11;
const int s2 = 12;

double w_e_o[ENTRADAS + 1][NR_NEURON_O] = {{-0.713013, 4.16431, -5.48521, -2.29616, -11.6229, 10.1455},
										   {-13.8452, 2.47936, 1.74057, -0.42644, 4.7151, -4.48398},
										   {5.09045, -16.2471, -0.0663805, -1.5426, 6.71265, 1.01201}};

double w_o_o[NR_NEURON_O + 1][NR_NEURON_O] = {{-0.955732, -7.00449, -5.51682, -19.1118, -31.7169, -20.7859},
											  {1599.18, -2.82879, 12.1979, -4.5816, 18.6024, 7.33838},
											  {-2.82879, 111.032, -1.01418, 2.04162, 20.9231, 7.60537},
											  {10.1979, -3.01418, 43.7009, -7.19078, 1.94591, -1.33867},
											  {-4.5816, 2.04162, -7.19078, 14.6589, 2.30546, 3.28592},
											  {20.6024, 22.9231, 3.94591, 2.30546, 181.974, -4.51999},
											  {7.33838, 7.60537, -1.33867, 3.28592, -6.51999, 53.4189}};

double w_o_s[NR_NEURON_O + 1][SAIDAS] = {{-34.9476, -18.0204},
										 {73.9261, 39.1748},
										 {-15.3795, 15.8028},
										 {-0.760588, 0.0840644},
										 {-23.6017, 1.96744},
										 {4.00761, 1.09898},
										 {-25.3971, -35.8595}};

double saida_o[NR_NEURON_O][CAMADAS_O];
double saida_s[SAIDAS];
double sensor[ENTRADAS];

double ler_sensor(int sensor);
double f_sigm1oid(double net);
void calcular_saidas(double entradas[ENTRADAS]);

void setup()
{
	Serial.begin(9600);
	pinMode(motor_1_a, OUTPUT);
	pinMode(motor_1_b, OUTPUT);
	pinMode(motor_2_a, OUTPUT);
	pinMode(motor_2_b, OUTPUT);
}

double vel_m1 = 0;
double vel_m2 = 0;

void loop()
{
	sensor[0] = ler_sensor(s1);
	sensor[1] = ler_sensor(s2);

	Serial.println("");
	Serial.print("s1: ");
	Serial.print(sensor[0]);
	Serial.print(" cm | s2: ");
	Serial.print(sensor[1]);
	Serial.println(" cm ");

	sensor[0] = mapSensor(0);
	sensor[1] = mapSensor(1);

	calcular_saidas(sensor);

	vel_m1 = mapSpeed(saida_s[0]);
	vel_m2 = mapSpeed(saida_s[1]);

  	Serial.print("m1: ");
	Serial.print(vel_m1);
  	Serial.print("    | m2: ");
	Serial.println(vel_m2);

	acelerar(vel_m1, vel_m2);
	delay(600);
	parar();
}

void parar()
{
	digitalWrite(motor_1_a, LOW);
	digitalWrite(motor_1_b, LOW);
	digitalWrite(motor_2_a, LOW);
	digitalWrite(motor_2_b, LOW);
}

double acelerar(double vel_m1, double vel_m2)
{
	analogWrite(motor_1_a, vel_m1);
	analogWrite(motor_2_a, vel_m2);
}

double ler_sensor(int sensor)
{
	// Emite sinal
	pinMode(sensor, OUTPUT);
	digitalWrite(sensor, HIGH);
	delayMicroseconds(100);
	digitalWrite(sensor, LOW);
	// Configura sensor para receber sinal
	pinMode(sensor, INPUT);
	int tempo = pulseIn(sensor, HIGH);
	return (double)tempo / 2 / 29.4;
}

double f_sigmoid(double net)
{
	return 1 / (1 + exp(-net));
}

double mapSensor(int pos)
{
	sensor[pos] = mapfloat(sensor[pos], 2, 333, 0, 1);
}

double mapSpeed(double saida)
{
	return mapfloat(saida, 0, 1, MIN_VEL, MAX_VEL);
}

double mapfloat(double x, double in_min, double in_max, double out_min, double out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void calcular_saidas(double entradas[ENTRADAS])
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
