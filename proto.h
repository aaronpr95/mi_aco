/* Fichero con los protoripos */

// PROTOTIOS DE LA LISTA DE LAS HORMIGAS
int lista_vacia(NODO *inicio);
void recorrer(NODO *inicio);
void acceder_nodo(NODO *inicio, int i);
NODO * buscar(NODO *inicio, int valor);
void insertar_despues(NODO **inicio, NODO *p, int valor, char ruta, char a);
NODO * sig(NODO *inicio);
int comprobar_argumentos(int argc, char *argv[]);
RUTA * sig_ruta(RUTA *inicio);
int lista_vacia_ruta(RUTA *inicio);
void recorrer_ruta(RUTA *inicio);
RUTA * buscar_ruta(RUTA *inicio, char nombre);
void insertar_despues_ruta(RUTA **inicio, RUTA *p, int tam, char a);


// PROTOTIPOS 
char elegir_ruta(RUTA *lista_ruta, int pos, char ruta);
void imprimir_vector(char array[], int n);
void crear_hormigas(NODO **lista);
void turno(int iteraccion, RUTA *lista_ruta, NODO **lista);
int salida();
int cambio_direccion(char ruta, int posicion, int direccion, RUTA *lista_ruta);
void insertar_feromona(char ruta, int pos, RUTA *lista_ruta);
void evaporar_feromonas(RUTA *lista_ruta);
void imprimir_feromonas(RUTA *lista_ruta);
void imprimir_hormigas(NODO *lista, RUTA *lista_ruta);
void aux(char ruta[], int tam);
int comprobarNumero(char cadena[]);
int comprobarCaracter(char caracter);
