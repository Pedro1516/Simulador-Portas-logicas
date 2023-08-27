
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct
{
	int **entrada;

	int quant_entradas;

	int tipo;

	int saida;
} Porta;

Porta *gerar_NOT(Porta *valor_entrada)
{
	// criando a porta not
	Porta *porta_logica = malloc(sizeof(Porta));

	porta_logica->entrada = (int **)malloc(sizeof(int *));
	porta_logica->entrada[0] = &(valor_entrada->saida);

	// O valor para NOT sera 0
	porta_logica->tipo = 0;
	porta_logica->quant_entradas = 1;

	return porta_logica;
}

Porta *gerar_AND(int num_conexoes, Porta *valor_entrada1, Porta *valor_entrada2, ...)
{
	// criando a porta and
	Porta *porta_logica = malloc(sizeof(Porta));
	porta_logica->tipo = 1; // o valor para AND sera 1
	porta_logica->quant_entradas = num_conexoes;

	va_list entradas;
	va_start(entradas, valor_entrada2);

	porta_logica->entrada = (int **)malloc(sizeof(int *) * num_conexoes);
	porta_logica->entrada[0] = &(valor_entrada1->saida);
	porta_logica->entrada[1] = &(valor_entrada2->saida);

	if (num_conexoes > 2)
	{
		for (int i = 2; i < num_conexoes; i++)
		{
			Porta *atual = va_arg(entradas, Porta *);
			porta_logica->entrada[i] = &(atual->saida);
		}
	}
	va_end(entradas);

	return porta_logica;
}

Porta *gerar_OR(int num_conexoes, Porta *valor_entrada1, Porta *valor_entrada2, ...)
{
	Porta *porta_logica = malloc(sizeof(Porta));
	porta_logica->tipo = 2;
	porta_logica->quant_entradas = num_conexoes;

	porta_logica->entrada = (int **)malloc(sizeof(int *) * num_conexoes);
	porta_logica->entrada[0] = &valor_entrada1->saida;
	porta_logica->entrada[1] = &valor_entrada2->saida;

	if (num_conexoes > 2)
	{
		va_list entradas;
			va_start(entradas, valor_entrada2);

		for (int i = 2; i < num_conexoes; i++)
		{
			Porta *atual = va_arg(entradas, Porta *);
			porta_logica->entrada[i] = &(atual->saida);
		}
	}
	return porta_logica;
}

void update(Porta **portas)
{
	for (size_t i = 0; i < 4; i++) //DEPOIS PRECISO TROCAR ESSE 4 POR UMA VARIÁVEL DE TAMANHO DA REDE DE PORTAS 
	{
		switch (portas[i]->tipo)
		{
		case 0:
			portas[i]->saida = !(*portas[i]->entrada[0]);
			break;
		case 1:
			portas[i]->saida = 1;

			for (size_t j = 0; j < portas[i]->quant_entradas; j++)
			{
				//Se pelo menos uma entrada for 0,  a saida sera 0
				if ((*portas[i]->entrada[j]) == 0)
				{
					portas[i]->saida = 0;
					break;
				}
			}

			break;
		case 2:
			portas[i]->saida = 0;

			for (size_t j = 0; j < portas[i]->quant_entradas; j++)
			{
				
				//Se pelo menos uma entrada for 1,  a saida sera 1
				if ((*portas[i]->entrada[j]) == 1)
				{
					portas[i]->saida = 1;
					break;
				}
			}

			break;
		}
	}
}


void user_cria_porta(){
	
}


int main()
{
	Porta sinalA;
	sinalA.saida = 1;

	Porta **rede_portas = (Porta **)malloc(sizeof(Porta *) * 10);

//Area de criacao de portas
	Porta *porta1 = gerar_NOT(&sinalA);
	rede_portas[0] = porta1;

	Porta *porta2 = gerar_NOT(porta1);
	rede_portas[1] = porta2;

	Porta *porta3 = gerar_AND(3, &sinalA, porta1, porta2);
	rede_portas[2] = porta3;

	Porta *porta4 = gerar_OR(2, &sinalA, porta2);
	rede_portas[3] = porta4;
	
	update(rede_portas);
//

	printf("Entrada A \nSaida: %d\n\n", sinalA.saida);
	printf("Porta NOT \nEntrada: %d \nSaida: %d\n\n", *porta1->entrada[0], porta1->saida);
	printf("Porta NOT \nEntrada: %d \nSaida: %d\n\n", *porta2->entrada[0], porta2->saida);

	printf("Porta AND \nEntrada: %d", *porta3->entrada[0]);

	for (size_t i = 1; i < porta3->quant_entradas; i++)
	{
		printf(", %d", *porta3->entrada[i]);
	}

	printf("\nSaida: %d\n\n", porta4->saida);
	
	printf("Porta OR \nEntrada: %d", *porta4->entrada[0]);

	for (size_t i = 1; i < porta4->quant_entradas; i++)
	{
		printf(", %d", *porta4->entrada[i]);
	}

	printf("\nSaida: %d\n", porta4->saida);





	return 0;
}
