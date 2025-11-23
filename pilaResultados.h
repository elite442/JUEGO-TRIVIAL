#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* Implementacion de Tipos de Datos Personalizados */
typedef char tString[100];

typedef struct{
	tString pregunta;
	tString opcionA;
	tString	opcionB;
	tString opcionC;
	tString opcionD;
	char correcto; // 1= A , 2=B, 3=C , 4=D 
	char respuesta;
	
}tResultadosTrivial;

typedef struct nodo{
	tResultadosTrivial datoJuego;
	struct nodo* siguiente;	
}tPilaResultados;

/* Declaracion de Prototipos de Funciones */
void inicializar(tPilaResultados**);
bool pilaPreguntarVacia(tPilaResultados*);
void vaciarPila(tPilaResultados**); // Añadido Recien
void todasLasPreguntas(tPilaResultados**);
void insertarPreguntas(tPilaResultados**, tResultadosTrivial); //, tString, tString, tString , tString, tString, char
void pregunta(tResultadosTrivial*, tString, tString, tString , tString, tString, char);
void mostrarPreguntas(tPilaResultados*, int);
void guardarRespuestadicha(tPilaResultados*, char);


/* Implementacion de Funciones */
void inicializar(tPilaResultados** pPreguntas){
	*pPreguntas = NULL;
}

bool pilaPreguntarVacia(tPilaResultados* pPreguntas){
	return (pPreguntas == NULL);
}

void vaciarPila(tPilaResultados** pila){
	tPilaResultados* aux;
	while(*pila != NULL){
		aux = *pila;
		*pila = aux->siguiente;
		free(aux);
	}
}

void insertarPreguntas(tPilaResultados** pila, tResultadosTrivial pJuego){
	
	tPilaResultados* nuevoNdo = (tPilaResultados*)malloc (sizeof(tPilaResultados));
	nuevoNdo->datoJuego = pJuego;
	
	if(pilaPreguntarVacia(*pila)){
		nuevoNdo->siguiente = NULL;
	}else{
		nuevoNdo->siguiente = *pila;
	}
	
	*pila = nuevoNdo;
	
}

void pregunta(tResultadosTrivial* juego , tString preguntas, tString a, tString b, tString c, tString d, char resCorrec){
	
	strcpy(juego->pregunta, preguntas);
	strcpy(juego->opcionA, a);
	strcpy(juego->opcionB, b);
	strcpy(juego->opcionC, c);
	strcpy(juego->opcionD, d);
	juego->correcto = resCorrec;
	
}

void todasLasPreguntas(tPilaResultados** pila){
	tResultadosTrivial juego;
	
	pregunta(&juego ,"Quien pinto la noche estrellada?", "A) Van Gogh", "B) Monet", "C) Dali", "D) Renoir", 'A');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"En que pais se originaron los juegos olimpicos?", "A) Estados Unidos", "B) China", "C) Paraguay", "D) Grecia", 'D');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Cual es el material mas ligero?", "A) Hierro", "B) Litio", "C) Aluminio", "D) Plata", 'B');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Quien formulo la teoria de la relatividad?", "A) Isaac Newton", "B) Stephen Hawking", "C) Nikola Tesla", "D) Albert Einstein", 'D');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Cual es el oceano mas grande del mundo?", "A) Pacifico", "B) Atlantico", "C) Indico", "D) Artico", 'A');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Cual es el valor de pi aproximado?", "A) 2.14", "B) 3.1565", "C) 3.14", "D) 4.13", 'C');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Cual es la capital de Italia?", "A) Milan", "B) Roma", "C) Florencia", "D) Venecia", 'B');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Cual es el planeta mas cercano al sol?", "A) Venus", "B) Mercurio", "C) Marte", "D) Tierra", 'B');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Cual es la capital de Argentina?", "A) Bs As", "B) Salta", "C) Corrientes", "D) Cordoba", 'A');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Que tipo de formato debo usar para fragmentos cortos de codigo?", "A) Triple comillas (```)", "B) Una comilla (`)", "C) Texto en mayusculas (TEXTO)", "D) Texto en cursiva (texto)", 'B');
	insertarPreguntas(pila, juego);

	pregunta(&juego ,"Para que se utiliza #include?", "A) Declarar una variable global", "B) Incluir archivos o librerias de cabecera" , "C) Iniciar la funcion principal del programa", "D) Terminar la ejecucion del programa", 'B');
	insertarPreguntas(pila, juego);

	pregunta (&juego ,"Que pais gano la primera Copa del mundo de futbol?", "A) Brasil", "B) Alemania", "C) Uruguay", "D) Argentina", 'C');
	insertarPreguntas(pila, juego);

	pregunta(&juego ,"En que ciudad nacio mozart?", "A) Paris", "B) Amsterdam", "C) Venecia", "D) Salzburgo", 'D');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Quien pinto la mona lisa?", "A) Vicente van Gogh", "B) Pablo Picasso", "C) Leonardo da Vinci", "D) Claudia Monet", 'C');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Cuando comenzo la Primera Guerra Mundial?", "A) 1920", "B) 1918", "C) 1914", "D) 1916", 'C');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Que funcion se utiliza para imprimir texto en c?", "A) scanf()", "B) gets()", "C) printf()", "D) print()", 'C');
	insertarPreguntas(pila, juego);
	
	pregunta(&juego ,"Que tipo de bucle se ejecuta una sola vez?", "A) for", "B) do-while", "C) while", "D) switch", 'D');
	insertarPreguntas(pila, juego);

	pregunta(&juego ,"Que operador logico significa 'Y' en c?", "A) ||",  "B) !", "C) &&", "D) ==", 'C');
	insertarPreguntas(pila, juego);
	 
	pregunta(&juego ,"Cual es la pelicula mas taquillera de la historia?", "A) Superman", "B) Avatar", "C) Avengers: Endgame", "D) Jurassic World", 'B');
	insertarPreguntas(pila, juego);

	pregunta(&juego ,"Cual es el elemento Quimico mas abundante en la tierra?", "A) Hidrogeno", "B) Hierro", "C) Oxigeno", "D) Silicio", 'C');
	insertarPreguntas(pila, juego);
}

void mostrarPreguntas(tPilaResultados* preguntaActual, int i){
	
	
	if (preguntaActual == NULL){
		return;
	}

    int y = 150;
	
    DrawText(TextFormat("Pregunta %d: %s", i, preguntaActual->datoJuego.pregunta), 50, y, 20, BLACK);
    y += 35;

    DrawText(TextFormat("%s", preguntaActual->datoJuego.opcionA), 60, y, 20, DARKGRAY);
    y += 30;

    DrawText(TextFormat("%s", preguntaActual->datoJuego.opcionB), 60, y, 20, DARKGRAY);
    y += 30;

    DrawText(TextFormat("%s", preguntaActual->datoJuego.opcionC), 60, y, 20, DARKGRAY);
    y += 30;

    DrawText(TextFormat("%s", preguntaActual->datoJuego.opcionD), 60, y, 20, DARKGRAY);
   
}

void guardarRespuestadicha(tPilaResultados* pila, char respDicha){
	if (pila != NULL) {
        pila->datoJuego.respuesta = respDicha;
    }
	
}
