/**********************************************************
*	
*	Algoritmo de Optimización de las Hormigas
*	
*	Este algoritmos simula un número n de rutas en las que
*	se introducen hormigas artificiles para en contrar la 
*	ruta más corta, simulando el comportamiento natural.
*	
*	(c) Aarón Portales Rodrigo - 2016
*	
*********************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "global.h"
#include "estructuras.h"
#include "proto.h"


int main(int argc, char *argv[])
{
	// declaramos semilla para el rand
	time_t tt;
	tt=time(NULL);
	srand(tt); 

	RUTA *lista_ruta = NULL; // creamos las rutas
	NODO *lista = NULL; // iniciamos la lista de las hormigas
	int i; // contador para el for
	int p; // para parar

	comprobar_argumentos(argc, argv);

	// creamos las rutas
	crear_rutas(&lista_ruta);

	// creamos hormigas
	crear_hormigas( &lista); 

	recorrer(lista);

	// empiezan los turnos
	for(i = 0; i < ITOTAL; i++)
	{
		turno(i, lista_ruta, &lista);

		imprimir_hormigas(lista, lista_ruta);
		if(PARAR == VERDADERO)
		{
			scanf("%d", &p);
			if ( p == 1)
			{
				imprimir_feromonas(lista_ruta);
			}
			else
			{
				usleep(10000);
				system("clear");
			}
		}
		else
		{
			usleep(10000);
			system("clear");
		}
			
	}

	imprimir_hormigas(lista, lista_ruta);
	imprimir_feromonas(lista_ruta);
	recorrer(lista);

	return 0; 
}
