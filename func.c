/* Fichero con las funciones del programa  */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "global.h"
#include "estructuras.h"
#include "proto.h"


/* Funcion que comprueba los argumentos que se pasan al ejecutar el programa */
int comprobar_argumentos(int argc, char *argv[])
{
	int i; // contador
	ITOTAL = 500; 	// total de iteracciones que se van a hacer
	char c;
	// si no se le han pasado argumentos insertamos los valores por defecto
	if (argc == 1)
	{
		NUM_RUTAS = 3;
		// reservamos memoria para TAMANO
		TAMANO = (int *)calloc(NUM_RUTAS,sizeof(int));
	
		TAMANO[0] = 5;
		TAMANO[1] = 30;
		TAMANO[2] = 70;

		
		FEROMONA = 5; 	// nivel de feromona que deja una hormiga por defecto
		NUM_HORMIGAS = 10;

		PARAR = FALSO;
	}
	else if(argc > 1) // se le han pasado argumentos
	{
		if (strcmp(argv[1], "-m") == 0)
		{

			printf("Número de rutas: ");
			scanf("%d", &NUM_RUTAS);

			TAMANO = (int *)calloc(NUM_RUTAS,sizeof(int));

			for(i = 0; i< NUM_RUTAS; i++)
			{	
				printf("Distancia ruta[%d]: ",i+1 );
				scanf("%d", &TAMANO[i]);
			}

			printf("Nivel de feromona: ");
			scanf("%d", &FEROMONA);

			printf("Numero de hormigas: ");
			scanf("%d", &NUM_HORMIGAS);

			printf("Parar: (v/f): ");
			scanf(" %c", &c);
			if (c == 'v')
			{
				PARAR = VERDADERO;
			}
		}
		else if (argv[1][0] == '-')
		{
			
			if (comprobarCaracter(argv[1][1]) == VERDADERO)
			{
				NUM_RUTAS = (int)argv[1][1] - 48;

				TAMANO = (int *)calloc(NUM_RUTAS,sizeof(int));

				for ( i = 2; i < 2+NUM_RUTAS; i++)
				{
					TAMANO[i-2] = atoi(argv[i]);
				}

				FEROMONA = atoi(argv[2+NUM_RUTAS]);

				NUM_HORMIGAS = atoi(argv[3+NUM_RUTAS]);

				if (argv[argc-1][0] == 'p')
				{
					PARAR = VERDADERO;
				}
 			}
		}
		else
		{
			printf("Error en el paso de parametros\n");
			exit(1);
		}
	}
}

// devuelve verdadero si es un numero
int comprobarCaracter(char caracter)
{
	if(caracter < 48 || caracter > 57)
	{	
		return FALSO;
	}
	
	return VERDADERO;
}

// devuelve verdadero si es numero o falso si no es un número
int comprobarNumero(char cadena[])
{
	int tam, i; // longitud del array
	
	tam = strlen(cadena);
	
	for(i=0;i<tam;i++)
	{
		if(cadena[i] < 48 || cadena[i] > 57)
		{	
			return FALSO;
		}
	}
	
	return VERDADERO;
}

void crear_rutas(RUTA **lista_ruta)
{
	int i; // contador
	char a = 65;

	for (i=0; i < NUM_RUTAS ; i++)
	{
		// le pasamos la ruta d porque todavia no se ha iniciado
		insertar_despues_ruta(lista_ruta, sig_ruta(*lista_ruta), TAMANO[i], a);
		a++; // pasamos a la siguiente letra
	}
}

/* Funcion que imprime las hormigas */
void imprimir_hormigas(NODO *lista, RUTA *lista_ruta)
{	
	// se crean vectores del tamaño de las rutas
	int i, j; // contador
	char *a[NUM_RUTAS];
	NODO *p;
	RUTA *z;
	int aux;

	for(i = 0; i< NUM_RUTAS; i++)
	{
		a[i] = (char *)calloc(TAMANO[i], sizeof(char));
	}

	// iniciamos los vectores
	for (i = 0; i < NUM_RUTAS; i++)
	{
		//printf("i - %d\n",i );
		for (j=0; j < TAMANO[i]; j++)
		{
			//printf("j - %d\n",j );
			a[i][j] = ' ';
		}
	}

	p = lista;

	// colocamos las hormigas en su posicion
	while(p != NULL)
	{
		z = buscar_ruta(lista_ruta, p->mi_hormiga.ruta);
		if (z != NULL)
		{
			aux =z->nom;
			aux -= 65;
			a[aux][p->mi_hormiga.posicion] = p->mi_hormiga.forma;
 		}

		p = p->siguiente;
	}

	z = lista_ruta;
	// imprimimos los vectores
	for (i = 0; i < NUM_RUTAS; i++)
	{	
		printf("%c->[", z->nom);
		for (j=0; j < TAMANO[i]; j++)
		{
			if(j == TAMANO[i] - 1) printf("%c]\n",a[i][j] );
			else printf("%c.",a[i][j] );
		}
		z = z->enlace;
	}


}

void imprimir_feromonas(RUTA *lista_ruta)
{
	int i; // contador
	RUTA *p; // puntero a nodo ruta

	p = lista_ruta;
	while( p != NULL)
	{	
		printf(" %c -> [", p->nom);
		for(i = 0 ; i < p->tam ;i++)
		{
			if(i == p->tam-1)
			{
				printf("%d]", p->rut[i]);
			}
			else
			{
				printf("%d,", p->rut[i]);
			}
		}
		puts("");

		p = p->enlace;
	}
}

/* La funcion turno gestiona todos los movimientos evaporaciones de cada vez
* 	Iteraciones  - turno en el que nos encontramos
* 	Todas las rutas 
* 	NODO puntero a la lista de hormigas */
void turno(int iteraccion, RUTA *lista_ruta, NODO **lista)
{	
	NODO *p;
	p = *lista; // para ir recorriendo la lista
	int pos; // para guardar en que posicion esta la hormiga en cada momento
	RUTA *z; // para guardar puntero a ruta

	// recorremos la lista de hormigas hasta que se llegue al final y ya no haya más
	while( p != NULL )
	{	
		// guardamos en pos la posicion en la que está la hormiga actual
		pos = p->mi_hormiga.posicion;

		// comprobamos si la hormiga le toca salir
		// si le toca salir ponemos su parámetro de salida con valor a la constante SALIR
		if (p->mi_hormiga.salida == iteraccion) p->mi_hormiga.salida = SALIR;

		// si la hormiga ya ha salido
		if (p->mi_hormiga.salida == SALIR)
		{	
			if (p->mi_hormiga.ruta == 'd') // está todavia sin salir (es el turno en el que justo sale)
			{
				p->mi_hormiga.ruta = elegir_ruta(lista_ruta, p->mi_hormiga.posicion, p->mi_hormiga.ruta);
				p->mi_hormiga.posicion = 0;

				// insertamos feromona
				insertar_feromona(p->mi_hormiga.ruta, p->mi_hormiga.posicion, lista_ruta);

			}
			else	// si la hormiga esta transitando normal, que no esta saliendo
			{
				// comprobar direccion y si hay que cambiar
				if (cambio_direccion(p->mi_hormiga.ruta, pos, p->mi_hormiga.direccion, lista_ruta) == VERDADERO)
				{
					//printf("Cambio de pos -> Dir = %d, ruta = %c, pos=%d ---------------------------\n", p->mi_hormiga.direccion,p->mi_hormiga.ruta,pos);
					// comprobar la nueva ruta
					p->mi_hormiga.ruta = elegir_ruta(lista_ruta, p->mi_hormiga.posicion, p->mi_hormiga.ruta);

					//hay que comprobar cual es la posicion de esa ruta y la colocamos ahi
					z = buscar_ruta(lista_ruta, p->mi_hormiga.ruta);
					if (p->mi_hormiga.direccion == VERDADERO)
					{
						p->mi_hormiga.posicion = z->tam-1;
					}
					else
					{
						p->mi_hormiga.posicion = 0;
					}

					// cambiar la direccion
					if (p->mi_hormiga.direccion == VERDADERO) p->mi_hormiga.direccion = FALSO;
					else p->mi_hormiga.direccion = VERDADERO;
					
					insertar_feromona(p->mi_hormiga.ruta, pos, lista_ruta);
				}
				else
				{
					// avance normal
					if (p->mi_hormiga.direccion == VERDADERO)
					{
						p->mi_hormiga.posicion += 1; // se avanza una posicion
						insertar_feromona(p->mi_hormiga.ruta, p->mi_hormiga.posicion, lista_ruta);
					}
					else
					{
						p->mi_hormiga.posicion -= 1; // se avanza una posicion
						insertar_feromona(p->mi_hormiga.ruta, p->mi_hormiga.posicion, lista_ruta);
					}
				}
			}

			
		}
		// si no le toca salir a la hormiga se pasa a la siguiente

		p = p->siguiente;
	}

	// despues de haber ejecutado todas las hormigas, evaporar las feromonas
	evaporar_feromonas(lista_ruta);
}

/* Funcion que realiza la evaporacion de las feormonas quitando una unidad a cada posicion */
void evaporar_feromonas(RUTA *lista_ruta)
{
	int i; // contador
	RUTA *p; // puntero a nodo ruta

	p = lista_ruta;
	while( p != NULL)
	{	
		for(i = 0 ; i < p->tam ;i++)
		{
			if (p->rut[i] != 0) p->rut[i] -= 1;
		}

		p = p->enlace;
	}
}

/* funcion que nos devuelve VERDADERO si se ha llegado a final de ruta y hay que elegir otra vez 
* 	ruta 		- ruta actual
* 	posicion 	- posicion actual de la hormiga
*	direccion   - direccion que lleva la hormiga */
int cambio_direccion(char ruta, int posicion, int direccion, RUTA *lista_ruta)
{	
	RUTA *p;

	p = buscar_ruta(lista_ruta, ruta);

	/* Habrá cambio de direccion cuando la hormiga esté en la parte de salida direccion izq 
	y cuando esté en el final del vector direccion derecha */
	if ( (posicion == 0 && direccion == FALSO) || (posicion == p->tam - 1 && direccion == VERDADERO) )
		return VERDADERO;
	else
		return FALSO;
}


/* Insertar feromona
* 	a - b - c
* 	ruta 		- ruta actual de la hormiga
* 	posicion */
void insertar_feromona(char ruta, int pos, RUTA *lista_ruta)
{
	RUTA *p;

	p = buscar_ruta(lista_ruta, ruta);
	if (p != NULL)
	{
		p->rut[pos] += FEROMONA;
	}
	else
	{
		printf("ERROR ruta incorreccleata(insertar_feromona), ruta %c, pos %d\n", ruta, pos);
		//exit(0);
	}
}

/* Funcion que devuelve en cuantos turnos tiene que salir la hormiga */
int salida()
{
	int turnos;
	turnos = rand()%100;
	return turnos;
}


/* 	funcion de eleccion de la ruta segun el numero de feromonas de la hormiga
*  	Le pasamos la lista de rutas y en que se encuentra la hormiga y en que ruta está*/
char elegir_ruta(RUTA *lista_ruta, int pos, char ruta)
{
	int sumatorio = 0; 	// vamos a guardar el numero de feromonas totales
	char *probabilidad; // array de tamaño sumatorio donde vamos a introducir los valores
	int i, j; 				// contador
	int eleccion; 		// se va a guardar la eleccion para retornar la ruta
	RUTA *p;		// variable puntero a ruta auxiliar para algunas operaciones
	int contador = 0, inicio = 0; // variables para rellenar el vector probabilidad

	
	// array que guarda en cada posicion, el numero de feromonas que tiene cada posible casilla
	// siguiente que va a elegir la hormiga
	int feromonas_rutas[NUM_RUTAS];
	
	// hay que comprobar en que lado estamos
	// buscamos la ruta
	p = buscar_ruta(lista_ruta, ruta);
	
	// cuando no ha salido
	if (p == NULL)
	{	
		p = lista_ruta;

		for(i=0 ; i<NUM_RUTAS ; i++)
		{
			// le sumamos 1 por si alguna de las rutas tiene valor 0 no entraría en la posibilidad de ser elegida
			feromonas_rutas[i] = p->rut[0] + 1;
			p = p->enlace;
		}
	}
	// comprobamos pos para ver si estamos en inicio o en el final de la ruta
	else if (pos == 0)
	{
		p = lista_ruta;
		// estamos en el inicio de la ruta
		// guardamos las feromonas
		for(i=0 ; i<NUM_RUTAS ; i++)
		{
			// le sumamos 1 por si alguna de las rutas tiene valor 0 no entraría en la posibilidad de ser elegida
			feromonas_rutas[i] = p->rut[0] + 1;
			p = p->enlace;
		}
	}
	else
	{
		p = lista_ruta;
		// estamos en la parte final de la ruta
		// hay que comprobar los valores finales de la ruta
		// guardamos las feromonas
		for(i=0 ; i<NUM_RUTAS ; i++)
		{	
			// le sumamos 1 por si alguna de las rutas tiene valor 0 no entraría en la posibilidad de ser elegida
			feromonas_rutas[i] = p->rut[(p->tam)-1] + 1;
			p = p->enlace;
		}
	}

	// guardamos en sumatorio todos los valores de feromona
	for (i=0 ; i<NUM_RUTAS ; i++)
	{
		sumatorio += feromonas_rutas[i];
	}

	// guardamos memoria para todas las posiciones
	probabilidad = (char *)calloc(sumatorio, sizeof(char));

	// introducimos los valores
	p = lista_ruta;
	j = 0;
	while(p != NULL)
	{
		for(i = inicio; i < feromonas_rutas[j] + inicio; i++)
		{
			probabilidad[i] = p->nom;
		}
		// en inicio ponemos la siguiente posicion vacia del vector que es las que habia mas todas las 
		// que se acaban de introducir
		inicio += feromonas_rutas[j];

		// avanzamos a la siguiente ruta
		p = p->enlace;
		j++;
	}

	/*for(i = 0; i<NUM_RUTAS; i++) printf("%d - ",feromonas_rutas[i] );
	printf("]\n");
	imprimir_vector(probabilidad, sumatorio);*/

	// hacemos la eleccion de una posicion del vector
	eleccion = rand()%sumatorio;

	// devolvemos el valor
	return probabilidad[eleccion];
}

/* Funcion para crear un numero num de hormigas */
void crear_hormigas(NODO **lista)
{
	int i; // contador
	char a = 97;

	for (i=1; i <= NUM_HORMIGAS ; i++)
	{
		// le pasamos la ruta d porque todavia no se ha iniciado
		insertar_despues(lista, sig(*lista), i, 'd', a);
		a++; 
	}
}


void imprimir_vector(char array[], int n)
{	printf("imprimir\n");
	int i;
	for( i = 0; i< n ; i++) printf("%c,", array[i]);
	printf("]\n");

}

// FUNCIONES DE LAS LISTAS DE HORMIGAS ----------------------------------------------------------------
//este modulo coloca el puntero en la posición anterior a la que sea NULL,
// o sea, en la ultima ocupada
NODO * sig(NODO *inicio)
{
	NODO *p1, *p2;
	
	p1 = inicio;
	
	//se recorre la lista hasta encontrar el NULL
	if (inicio == NULL)
	{
		return inicio;
	}
	else
	{
		//se recorre la lista hasta encontrar el NULL
		while(p1!=NULL)
		{
			p2 = p1;
			p1 = p1->siguiente;
		}
		return p2;
	}
}

int lista_vacia(NODO *inicio)
{
	if (inicio == NULL) return VERDADERO;
	else return FALSO;
}


void recorrer(NODO *inicio)
{
	NODO *p;

	p = inicio;

	while( p != NULL)
	{
		
		printf("HORMIGA id=%d ruta=%c, pos=%d, salida=%d\n",p->mi_hormiga.id, p->mi_hormiga.ruta, p->mi_hormiga.posicion, p->mi_hormiga.salida );
		p =p->siguiente;
	}
}

/* 
Recibe - inicio puntero a nodo lista
Recibe - i entera 					*/
void acceder_nodo(NODO *inicio, int i)
{
	int j;
	NODO *p = NULL;

	// el primer elemento será el cero 0
	j=0;

	if (lista_vacia(inicio) != VERDADERO)
	{
		p = inicio;
		while(j<i)
		{
			p = p->siguiente;
			j++;

		}
	}
	else
	{
		printf("La lista esta vacia(acceder_nodo)\n");
	}	
}

/*
recibe inicio 	- puntero a nodo
recibe valor	- de tipo t : valor que se busca*/
NODO * buscar(NODO *inicio, int valor)
{
	NODO *p;

	p = inicio;

	while( p != NULL)
	{
		if (p->mi_hormiga.id == valor)
		{
			return p;
		}

		p = p->siguiente;
	}

	return NULL;
}


/*
Transforma inicio - puntero a nodo lista
Recibe p - puntero a NODO
REcibe elemento - de tipo t*/
void insertar_despues(NODO **inicio, NODO *p, int valor, char ruta, char a)
{
	NODO *nuevo;
	nuevo = (NODO *)malloc(sizeof(NODO));

	// comprobamos que se ha reservado memoria
	if (nuevo == NULL)
	{
		printf("No se ha podido reservar memoria para un nuevo nodo\n");
	}
	else
	{
		// damos valor a la hormiga
		nuevo->mi_hormiga.id = valor;
		nuevo->mi_hormiga.ruta = ruta;
		nuevo->mi_hormiga.posicion = -1;
		nuevo->mi_hormiga.salida = salida();
		nuevo->mi_hormiga.direccion = VERDADERO;
		nuevo->mi_hormiga.forma = a;

		if (lista_vacia(*inicio) == VERDADERO)
		{
			*inicio = nuevo;
			nuevo->siguiente = NULL;
		}
		else
		{
			nuevo->siguiente = p->siguiente;
			p->siguiente = nuevo;
		}
	}
}

// FUNCIONES DE LAS LISTAS DE HORMIGAS ----------------------------------------------------------------
//este modulo coloca el puntero en la posición anterior a la que sea NULL,
// o sea, en la ultima ocupada
RUTA * sig_ruta(RUTA *inicio)
{
	RUTA *p1, *p2;
	
	p1 = inicio;
	
	//se recorre la lista hasta encontrar el NULL
	if (inicio == NULL)
	{
		return inicio;
	}
	else
	{
		//se recorre la lista hasta encontrar el NULL
		while(p1!=NULL)
		{
			p2 = p1;
			p1 = p1->enlace;
		}
		return p2;
	}
}

int lista_vacia_ruta(RUTA *inicio)
{
	if (inicio == NULL) return VERDADERO;
	else return FALSO;
}


void recorrer_ruta(RUTA *inicio)
{
	RUTA *p;

	p = inicio;

	while( p != NULL)
	{
		// tratar
		//printf("HORMIGA id=%d ruta=%c, pos=%d, salida=%d\n",p->mi_hormiga.id, p->mi_hormiga.ruta, p->mi_hormiga.posicion, p->mi_hormiga.salida );
		p =p->enlace;
	}
}

/*
recibe inicio 	- puntero a RUTA
recibe valor	- de tipo t : valor que se busca*/
RUTA * buscar_ruta(RUTA *inicio, char nombre)
{
	RUTA *p;

	p = inicio;

	while( p != NULL)
	{
		if (p->nom == nombre)
		{
			return p;
		}

		p = p->enlace;
	}

	return NULL;
}


/*
Transforma inicio - puntero a RUTA lista
Recibe p - puntero a RUTA
REcibe elemento - de tipo t*/
void insertar_despues_ruta(RUTA **inicio, RUTA *p, int tam, char a)
{
	RUTA *nuevo;
	nuevo = (RUTA *)malloc(sizeof(RUTA));

	// comprobamos que se ha reservado memoria
	if (nuevo == NULL)
	{
		printf("No se ha podido reservar memoria para un nuevo RUTA\n");
	}
	else
	{
		// damos valor a la ruta
		nuevo->nom = a;
		nuevo->rut = (int *)calloc(tam,sizeof(int));
		nuevo->tam = tam;

		if (lista_vacia_ruta(*inicio) == VERDADERO)
		{
			*inicio = nuevo;
			nuevo->enlace = NULL;
		}
		else
		{
			nuevo->enlace = p->enlace;
			p->enlace = nuevo;
		}
	}
}