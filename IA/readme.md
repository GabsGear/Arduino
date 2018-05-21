```
#REDES NEURAIS COM ARDUINO
```

Este projeto usa redes neurais convolucionarias para a tomada de decisão do movimento de um carro com 
dois sensores e dois motores para que o carro desvie de qualquer obstáculo.

A rede foi desenvolvida e treinada em C++ e após isso o código é passado para o arduino onde 
as saídas (motores) são calculadas em função dos pesos treinados.

Para compilar o treinamento use:
```
  g++ -std=c++11 RNA-treinamento.cpp mlp.cpp -o teste
  ./teste
```

O circuito montado é o seguinte:

![alt text](https://github.com/GabsGear/Arduino/blob/master/IA/circuit.png?raw=true)
