/* Estructuras que usa el programa */
// declaramos la estructura hormiga
struct hormiga
{
	int id;
	char ruta;		// indica en que ruta se encuentra la hormiga en cada momento
	int posicion; 	// iniciamos a -1 porque al principio no esta en ninguna posicion
	int salida; 	// cuando sale
	int direccion; 	// si va acia la colonia o hacia la "comida"
					// si es VERDADERO va en positivo (->) - FALSO negativo (<-)
	char forma;
}; 
typedef struct hormiga HORMIGA;


// estructura de los nodos de la estructura de las hormigas
struct nodo_lista
{
	HORMIGA mi_hormiga;
	struct nodo_lista *siguiente;
};
typedef struct nodo_lista NODO;

// estructura de la ruta
struct ruta
{
	int tam;	// guardamos el tamaño de la ruta
	char nom; 	// nombre e identificador de la ruta
	int *rut; 	// array por el que van a ir las hormiga
	struct ruta *enlace;
};
typedef struct ruta RUTA;