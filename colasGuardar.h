#include "raylib.h"
#include "pilaResultados.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* Implementacion de Tipos de Datos Personalizados */
typedef char tString[100];

typedef struct{
	tString pregunta;
	char correcta; // 1= A , 2=B, 3=C , 4=D 
	char respuesta;
}tJuegoGuardar;

typedef struct ndo{
	tJuegoGuardar datoGuardar;
	struct ndo* siguiente;	
}tNodoGuardado;

typedef struct {
	tNodoGuardado* principio;
	tNodoGuardado* final;	
}tColasGuardado;

/* Declaracion de Variable Global */
tPilaResultados* pila;

/* Declaracion de Prototipos de Funciones */
void inicializarGuardado(tColasGuardado**);
bool colaVacia(tColasGuardado*);
void vaciarCola(tColasGuardado**);
void agregarGuardado(tColasGuardado**, tJuegoGuardar);
void eliminarCola(tColasGuardado**, tJuegoGuardar);
void guardarResultadosEnCola(tPilaResultados*, tColasGuardado**);
void guardarPartidaEnArchivo(tColasGuardado*);
void mostrarPreguntasGuardadas(tColasGuardado*);
int totalCorrectas(tColasGuardado*);

/* Implementacion de Funciones */
void inicializarGuardado(tColasGuardado** guardarALaCola){  
	if (guardarALaCola == NULL) return;
    *guardarALaCola = (tColasGuardado*) malloc(sizeof(tColasGuardado));
    if (*guardarALaCola != NULL) {
        (*guardarALaCola)->principio = NULL;
        (*guardarALaCola)->final = NULL;
    }
}

bool colaVacia(tColasGuardado* guardarALaCola){
	/* Devuelve TRUE si la cola no existe o si el principio es NULL */
    return (guardarALaCola == NULL) || (guardarALaCola->principio == NULL);
}

void vaciarCola(tColasGuardado** cola){
    if (cola == NULL || *cola == NULL) return;
        tNodoGuardado *actual = (*cola)->principio;
        tNodoGuardado *aux;

    while (actual != NULL){
          aux = actual;
          actual = actual->siguiente;
          free(aux);
    }
    (*cola)->principio = NULL;
    (*cola)->final = NULL;
}


void agregarGuardado(tColasGuardado** guardarALaCola, tJuegoGuardar DtoGuardar){
if (guardarALaCola == NULL) return;

    /* Inicializar la cola */
    if (*guardarALaCola == NULL) {
        inicializarGuardado(guardarALaCola);
        if (*guardarALaCola == NULL) return;
    }

    tNodoGuardado* nuevoNdo = (tNodoGuardado*) malloc(sizeof(tNodoGuardado));
    if (nuevoNdo == NULL) return;
    nuevoNdo->datoGuardar = DtoGuardar;
    nuevoNdo->siguiente = NULL;

    if ((*guardarALaCola)->principio == NULL) {
        (*guardarALaCola)->principio = nuevoNdo;
        (*guardarALaCola)->final = nuevoNdo;
    } else {
        (*guardarALaCola)->final->siguiente = nuevoNdo;
        (*guardarALaCola)->final = nuevoNdo;
    }
}

void eliminarCola(tColasGuardado** guardarALaCola, tJuegoGuardar DtoGuardar){
/* Eliminar el primer nodo */
    if (guardarALaCola == NULL || *guardarALaCola == NULL) {
        printf("\nNo hay preguntas para quitar\n");
        return;
    }

    tNodoGuardado* nDoSupr = (*guardarALaCola)->principio;
    if (nDoSupr == NULL) {
        printf("\nNo hay preguntas para quitar\n");
        return;
    }

    if ((*guardarALaCola)->principio == (*guardarALaCola)->final) {
        /* solo un elemento: liberar y dejar la cola vacía */
        free(nDoSupr);
        (*guardarALaCola)->principio = NULL;
        (*guardarALaCola)->final = NULL;
        /* opcional: free la estructura en sí si querés:
           free(*guardarALaCola);
           *guardarALaCola = NULL;
        */
    } else {
        (*guardarALaCola)->principio = nDoSupr->siguiente;
        free(nDoSupr);
    }
}

/* Convierte la pila de resultados en elementos de la cola para mostrar en STATS.
   Recorre la pila y por cada nodo crea tJuegoGuardar con pregunta, correcta y respuesta. */
void guardarResultadosEnCola(tPilaResultados* pila, tColasGuardado** cola){
    if (pila == NULL) return;

    /* Recorremos la pila desde el nodo actual hacia abajo. */
    tPilaResultados* aux = pila;
    while (aux != NULL){
        tJuegoGuardar g;
        strcpy(g.pregunta, aux->datoJuego.pregunta);
        g.correcta = aux->datoJuego.correcto;   
        g.respuesta = aux->datoJuego.respuesta; 

        agregarGuardado(cola, g);
        aux = aux->siguiente;
    }
}

void guardarPartidaEnArchivo(tColasGuardado* cola){
	if(cola == NULL || cola->principio == NULL) return;
	
	FILE* archivoPartidas = fopen("partidas.dat", "ab");
	
	if(archivoPartidas == NULL){
		printf("Error al abrir el archivo!\n");
		exit(EXIT_FAILURE);
	}
	printf("Archivo abierto para añadir!\n");
	
	static int numeroPartida = 1;
	
	fprintf(archivoPartidas, "PARTIDA %d\n", numeroPartida);
	
	tNodoGuardado* aux = cola->principio;
	
	while(aux != NULL){
		fprintf(archivoPartidas, "Pregunta: %s\n", aux->datoGuardar.pregunta);
		fprintf(archivoPartidas, "Correcta: %c\n", aux->datoGuardar.correcta);
		fprintf(archivoPartidas, "Respuesta: %c\n\n", aux->datoGuardar.respuesta);
		
		aux = aux->siguiente;
	}
	
	fprintf(archivoPartidas, "-------------------------------------\n");
	
	fclose(archivoPartidas);
	numeroPartida++;
}

void mostrarHistorialPartidas(){ // Añadido Recien
	FILE* archivoPartidas = fopen("partidas.dat", "r");
	
	if(archivoPartidas == NULL){
		DrawText("No existe historial todavia!", 50, 150, 25, RED);
		exit(EXIT_FAILURE);
	}
	
	// Buffer para leer líneas del archivo
    char linea[300];

    // Scroll con rueda del mouse
    static int scrollOffset = 0;
    scrollOffset += GetMouseWheelMove() * -30;

    // Area visible
    int areaX = 40;
    int areaY = 150;
    int areaW = 1100;
    int areaH = 500;

    // Dibujar encabezado
    DrawLine(40, 140, 1150, 140, BLACK);

    // Comienzo del recorte
    BeginScissorMode(areaX, areaY, areaW, areaH);

    // Posicion inicial para texto dentro del área
    int y = areaY + scrollOffset;

    // Leer cada línea del archivo y dibujarla ordenadamente
    while (fgets(linea, sizeof(linea), archivoPartidas)) {

        // Salto entre líneas (25 px)
        DrawText(linea, areaX + 10, y, 20, BLACK);
        y += 25;
    }

    EndScissorMode();

    fclose(archivoPartidas);
    
    // Evitar scroll infinito hacia arriba
    if(scrollOffset > 0) scrollOffset = 0;
}


void mostrarPreguntasGuardadas(tColasGuardado* guardarALaCola){
 /* Dibujar la lista en pantalla */
    if (guardarALaCola == NULL || guardarALaCola->principio == NULL) {
        DrawText("No hay estadisticas aun.", 50, 150, 25, DARKGRAY);
        return;
    }
    
    static int scrollOffset = 0;   // desplazar hacia abajo/arriba

// Calcular cantidad de nodos
    int cantidad = 0;
    tNodoGuardado* cont = guardarALaCola->principio;
    while(cont != NULL){
	      cantidad++;
	      cont = cont->siguiente;
    }

// Definir area visible
    int areaX = 40;
    int areaY = 150;
    int areaW = 1100;
    int areaH = 500;
    
// Manejo del scroll
    scrollOffset += GetMouseWheelMove() * -30;
    
    int totalHeight = cantidad * 40;
    int minScroll = -(totalHeight - areaH);
    int maxScroll = 0;
    
    if(scrollOffset < minScroll) scrollOffset = minScroll;
    if(scrollOffset > maxScroll) scrollOffset = maxScroll;

// Encabezado 
    DrawText("PREGUNTA", 50, 110, 20, DARKBLUE);
    DrawText("CORRECTA", 800, 110, 20, DARKBLUE);
    DrawText("RESPUESTA", 950, 110, 20, DARKBLUE);

    DrawLine(40, 140, 1150, 140, BLACK);

// Activar recorte
    BeginScissorMode(areaX, areaY, areaW, areaH);
    
// Dibujar elementos desplazados
tNodoGuardado *aux = guardarALaCola->principio;
int y = areaY + scrollOffset;
    
    while (aux != NULL) {
        DrawText(aux->datoGuardar.pregunta, areaX + 10, y, 20, BLACK);

        /* Mostrar letra correcta y tu respuesta; color verde si coincide */
        Color col = (aux->datoGuardar.correcta == aux->datoGuardar.respuesta) ? GREEN : RED;
        DrawText(TextFormat("\t\t%c", aux->datoGuardar.correcta), areaX + 780, y, 20, col);
        DrawText(TextFormat("\t%c", aux->datoGuardar.respuesta), areaX + 940, y, 20, col);

        y += 40;
        aux = aux->siguiente;
    }
    EndScissorMode();
}

int totalCorrectas(tColasGuardado* guardarALaCola){
	if (guardarALaCola == NULL) return 0;
    int tot = 0;
    tNodoGuardado* aux = guardarALaCola->principio;
    while (aux != NULL){
        if (aux->datoGuardar.correcta == aux->datoGuardar.respuesta) tot++;
        aux = aux->siguiente;
    }
    return tot;
}
