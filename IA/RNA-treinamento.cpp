#include"mlp.h"

int main()
{
  srand(time(NULL));
  Mlp mlp;
  mlp.inicializa_sinapses();

  while (1)
  {

    mlp.menu();
  }

  return 0;
}
