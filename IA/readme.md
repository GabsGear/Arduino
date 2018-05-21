#  <center>UFSC - Redes neurais com arduino </center>

Este projeto usa redes neurais convolucionarias para a tomada de decisão do movimento de um carro com 
dois sensores e dois motores para que o carro desvie de qualquer obstáculo.

A rede foi desenvolvida e treinada em C++ e após isso o código é passado para o arduino onde 
as saídas (motores) são calculadas em função dos pesos treinados.

O treinamento é realizado com multi layer perceptrons como o exemplo abaixo:

![alt text](https://1.bp.blogspot.com/-Xal8aZ5MDL8/WlJm8dh1J9I/AAAAAAAAAo4/uCj6tt4T3T0HHUY4uexNuq2BXTUwcChqACLcBGAs/s1600/Multilayer-Perceptron.jpg)

Nosso algorítmo de treinamento segue a seguinte configuração:

```
  #define ENTRADAS 2
  #define SAIDAS 2
  #define NR_AMOSTRAS 9
  #define NR_NEURON_O 6
  #define CAMADAS_O 2
  #define EPOCAS 500000
  #define TX_APRENDIZADO 0.7
```

Para compilar o treinamento use:
```
  g++ -std=c++11 RNA-treinamento.cpp mlp.cpp -o teste
  ./teste
```

O circuito montado é o seguinte:

![alt text](https://github.com/GabsGear/Arduino/blob/master/IA/circuit.png?raw=true)
