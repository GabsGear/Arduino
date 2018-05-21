#include "mlp.h"

// void Mlp::statusBar(double percentage)
// {
//     int val = (int) (percentage * 100);
//     int lpad = (int) (percentage * PBWIDTH);
//     int rpad = PBWIDTH - lpad;
//     printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
//     fflush (stdout);
// }

void Mlp::inicializa_sinapses()
{
    std::cout << ("inicializando sinapses") << std::endl;
    for (int i = 0; i < ENTRADAS + 1; i++)
        for (int j = 0; j < NR_NEURON_O; j++)
            w_e_o[i][j] = gera_nr_aleatorios();

    for (int i = 0; i < NR_NEURON_O + 1; i++)
        for (int j = 0; j < NR_NEURON_O; j++)
            w_o_o[i][j] = gera_nr_aleatorios();

    for (int i = 0; i < NR_NEURON_O + 1; i++)
        for (int j = 0; j < SAIDAS; j++)
            w_o_s[i][j] = gera_nr_aleatorios();
}

int Mlp::gera_nr_aleatorios()
{
    int numeros[2] = {-1, 1};
    return (numeros[rand() % 2]);
}

void Mlp::mostrar_sinapses()
{
    for (int i = 0; i < ENTRADAS + 1; i++)
    {
        for (int j = 0; j < NR_NEURON_O; j++)
            std::cout << "w[" << i << "]"
                      << "[" << j << "]:" << w_e_o[i][j] << std::endl;
        std::cout << std::endl;
    }

    for (int i = 0; i < NR_NEURON_O + 1; i++)
    {
        for (int j = 0; j < NR_NEURON_O; j++)
            std::cout << "w[" << i << "]"
                      << "[" << j << "]:" << w_o_o[i][j] << std::endl;
        std::cout << std::endl;
    }

    for (int i = 0; i < NR_NEURON_O + 1; i++)
    {
        for (int j = 0; j < SAIDAS; j++)
            std::cout << "w[" << i << "]"
                      << "[" << j << "]:" << w_o_s[i][j] << std::endl;
        std::cout << std::endl;
    }
}

double Mlp::f_sigmoid(double net)
{
    return 1 / (1 + exp(-net));
}

void Mlp::calcular_saidas(double entradas[ENTRADAS])
{
    for (int i = 0; i < NR_NEURON_O; i++)
    {
        saida_o[i][0] = 0.0;
        saida_o[i][0] += w_e_o[0][i] * 1;

        for (int j = 1; j < ENTRADAS + 1; j++)
            saida_o[i][0] += w_e_o[j][i] * entradas[j - 1];

        saida_o[i][0] = f_sigmoid(saida_o[i][0]);
    }

    for (int i = 0; i < NR_NEURON_O; i++)
    {
        saida_o[i][1] = 0.0;
        saida_o[i][1] += w_o_o[0][i] * 1;

        for (int j = 1; j < NR_NEURON_O + 1; j++)
            saida_o[i][1] += w_o_o[j][i] * saida_o[j - 1][0];

        saida_o[i][1] = f_sigmoid(saida_o[i][1]);
    }

    for (int i = 0; i < SAIDAS; i++)
    {
        saida_s[i] = 0.0;
        saida_s[i] += w_o_s[0][i] * 1;

        for (int j = 1; j < NR_NEURON_O + 1; j++)
            saida_s[i] += w_o_s[j][i] * saida_o[j - 1][1];

        saida_s[i] = f_sigmoid(saida_s[i]);
    }
}

void Mlp::treinar_RNA()
{
    double entradas[ENTRADAS];

    for (int i = 1; i <= EPOCAS; i++)
    {
        std::cout << ".";
        for (int j = 0; j < NR_AMOSTRAS; j++)
        {
            for (int k = 0; k < ENTRADAS; k++)
                entradas[k] = cj_treinamento[j][k];

            calcular_saidas(entradas);
            calcular_delta_saida(cj_treinamento[j][2], cj_treinamento[j][3]);
            calcular_gradiente_oculta();
            calcular_delta_oculta();
            calcular_gradiente_oculta2();
            calcular_delta_oculta2();
            ajustar_pesos_sinapticos(entradas);
        }
    }
    std::cout << "FIM DO TREINO" << std::endl;
}

double Mlp::calcular_erro(double desejado, double saida)
{
    return desejado - saida;
}

void Mlp::menu()
{
    int opcao, i;
    double entradas[ENTRADAS];

    std::cout << ("Rede Neural Perceptron de Multiplas Camadas\n");
    std::cout << ("-----------------------------------------------\n\n");
    std::cout << ("1.Treinar a rede\n");
    std::cout << ("2.Usar a rede\n");
    std::cout << ("3.Ver pesos sinapticos\n");
    std::cout << ("4.Sair\n");
    std::cout << ("Opcao? ");
    std::cin >> opcao;

    switch (opcao)
    {

    case 1:
        inicializa_sinapses();
        treinar_RNA();
        salvaSinapses();
        break;

    case 2:
        for (i = 0; i < ENTRADAS; i++)
        {
            std::cout << ("Entrada %d: ", i + 1);
            scanf("%lf", &entradas[i]);
        }
        calcular_saidas(entradas);
        for (i = 0; i < SAIDAS; i++)
            std::cout << ("\nMotor %d: %.5f\n", i + 1, saida_s[i]);
        std::cout << ("\n\n\n");
        break;

    case 3:
        mostrar_sinapses();
        break;

    case 4:
        exit(0);

    default:
        menu();
    }
}

void Mlp::calcular_delta_saida(double desejado, double desejado2)
{
    delta_saida[0] = calcular_erro(desejado, saida_s[0]) * (1 - saida_s[0] * saida_s[0]);
    delta_saida[1] = calcular_erro(desejado2, saida_s[1]) * (1 - saida_s[1] * saida_s[1]);
}

void Mlp::calcular_gradiente_oculta()
{
    int i, j;

    for (i = 0; i < SAIDAS; i++)
        for (j = 1; j < NR_NEURON_O + 1; j++)
            gradiente_oculta[j - 1][1] = delta_saida[i] * w_o_s[j][i];
}

void Mlp::calcular_delta_oculta()
{
    for (int i = 0; i < NR_NEURON_O; i++)
        delta_oculta[i][1] = gradiente_oculta[i][1] * saida_o[i][1] * (1 - saida_o[i][1]);
}

void Mlp::calcular_gradiente_oculta2()
{
    for (int i = 0; i < SAIDAS; i++)
        for (int j = 1; j < NR_NEURON_O + 1; j++)
            gradiente_oculta[j - 1][0] = delta_oculta[i][1] * w_o_o[j][i];
}

void Mlp::calcular_delta_oculta2()
{
    for (int i = 0; i < NR_NEURON_O; i++)
        delta_oculta[i][0] = gradiente_oculta[i][0] * saida_o[i][0] * (1 - saida_o[i][0]);
}

void Mlp::ajustar_pesos_sinapticos(double entradas[ENTRADAS])
{
    for (int i = 0; i < SAIDAS; i++)
    {
        w_o_s[0][i] = w_o_s[0][i] + TX_APRENDIZADO * delta_saida[i] * 1;

        for (int j = 1; j < NR_NEURON_O + 1; j++)
            w_o_s[j][i] = w_o_s[j][i] + TX_APRENDIZADO * delta_saida[i] * saida_o[j - 1][1];
    }

    for (int i = 0; i < NR_NEURON_O; i++)
    {
        w_o_o[0][i] = w_o_o[0][i] + TX_APRENDIZADO * delta_oculta[i][1] * 1;

        for (int j = 1; j < NR_NEURON_O + 1; j++)
            w_o_o[j][i] = w_o_o[j][i] + TX_APRENDIZADO * delta_oculta[i][1] * delta_oculta[j - 1][1];
    }

    for (int i = 0; i < NR_NEURON_O; i++)
    {
        w_e_o[0][i] = w_e_o[0][i] + TX_APRENDIZADO * delta_oculta[i][0] * 1;

        for (int j = 1; j < ENTRADAS + 1; j++)
            w_e_o[j][i] = w_e_o[j][i] + TX_APRENDIZADO * delta_oculta[i][0] * entradas[j - 1];
    }
}

void Mlp::salvaSinapses()
{
    std::ofstream logfile;
    logfile.open("pesos.txt");

    logfile << "Camada de entrada w_e_o" << std::endl;
    for (int i = 0; i < ENTRADAS + 1; i++)
    {
        for (int j = 0; j < NR_NEURON_O; j++)
        {
            logfile << w_e_o[i][j] << " ";
            logfile << ", ";
        }
        logfile << std::endl;
    }

    logfile << "\n\nCamada Intermediaria w_o_o" << std::endl;
    for (int i = 0; i < NR_NEURON_O + 1; i++)
    {
        for (int j = 0; j < NR_NEURON_O; j++)
        {
            logfile << w_o_o[i][j] << " ";
            logfile << ", ";
        }
        logfile << std::endl;
    }

    logfile << "\n\nCamada de saida w_e_s" << std::endl;
    for (int i = 0; i < NR_NEURON_O + 1; i++)
    {
        for (int j = 0; j < SAIDAS; j++)
        {
            logfile << w_o_s[i][j] << " ";
            logfile << ", ";
        }
        logfile << std::endl;
    }
    logfile.close();
}