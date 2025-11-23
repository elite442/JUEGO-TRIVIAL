#include "raylib.h"
#include "colasGuardar.h"

/* Implementacion de Tipo de Dato Personalizado */
typedef enum GameScreen { 
	MENU, 
	JUGAR, 
	STATS, 
	HISTORIAL, // Añadido Recien
	EXIT
}GameScreen;

/* Declaracion de Prototipos de Funciones */
void inicializarPantalla();
void definirValores();
void detectarBotones();
void dibujarInterfaz();
void reiniciarJuego();
void leerTextoRTA(tPilaResultados**, int*);
void guardarResultadosEnCola(tPilaResultados*, tColasGuardado**);


/* Declaracion de Variables Globales */
GameScreen pantallaActual;
tJuegoGuardar DtoGuardar;
tColasGuardado* guardarALaCola;
tPilaResultados* pila;
tPilaResultados* preguntaActual;


static float tiempoRestante = 5.0f;
static int numeroPregunta = 1;
static bool guardadoHecho = false;
bool exitGame;
int screenWidth = 1200;
int screenHeight = 720;
Vector2 mousePoint; 
/* Vector2 es un Tipo de Dato Personalizado definido por Raylib, y su definicion es la siguiente:
typedef struct Vector2{
       float x; ---> x representa la posicion horizontal
	   float y; ---> y representa la posicion vertical
}Vector2; */

Rectangle btnJugar, btnOpciones, btnStats, btnHistorial, btnSalir; 
Rectangle btnVolver;


/* Funcion Principal */
int main(){
	inicializar(&pila);
	inicializar(&preguntaActual);
	inicializarGuardado(&guardarALaCola);
	
    todasLasPreguntas(&pila);
	inicializarPantalla();
	
	preguntaActual = pila;
	
	while(!WindowShouldClose() && !exitGame){
		detectarBotones();
		dibujarInterfaz();
	}
	CloseWindow();
	
	return 0;
}

/* Implementacion de Funciones */
void definirValores(){
	pantallaActual = MENU;
	exitGame = false;
	
	mousePoint = (Vector2){ 0.0, 0.0};
	
	btnJugar = (Rectangle){ screenWidth/2 - 100, 200, 200, 50 };
    btnStats = (Rectangle){ screenWidth/2 - 100, 270, 200, 50 };
    btnHistorial = (Rectangle){ screenWidth/2 - 100, 340, 200, 50 };
    btnSalir = (Rectangle){ screenWidth/2 - 100, 410, 200, 50 };
    
    btnVolver = (Rectangle){ screenWidth/2 - 600, 0, 100, 50};

}

void inicializarPantalla(){
	// Inicializacion
    InitWindow(screenWidth, screenHeight, "Juego Trivial");
    definirValores();
    SetTargetFPS(60);
	
}

void detectarBotones(){
	// --- Actualizacion del Mouse ---
    mousePoint = GetMousePosition();

    // --- Detección de botones solamente para el menu---
    if(pantallaActual == MENU){
		if (CheckCollisionPointRec(mousePoint, btnJugar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
	   		pantallaActual = JUGAR;
	   		vaciarCola(&guardarALaCola);
    	}
		if (CheckCollisionPointRec(mousePoint, btnStats) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    		pantallaActual = STATS;
    	}
    	if (CheckCollisionPointRec(mousePoint, btnHistorial) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            pantallaActual = HISTORIAL;
        }
		if (CheckCollisionPointRec(mousePoint, btnSalir) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			exitGame = true;
    	}
    }
     //detecta el boton de volver
    if (CheckCollisionPointRec(mousePoint, btnVolver) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    		reiniciarJuego();
           
    }
}

void dibujarInterfaz(){
	// --- Dibujo ---
    BeginDrawing();
    ClearBackground(RAYWHITE);

            // Título
    switch(pantallaActual){
    	case MENU:
        		DrawText("JUEGO TRIVIAL", screenWidth/2 - MeasureText("JUEGO TRIVIAL", 40)/2, 100, 40, DARKBLUE);

        				// Botones
    			DrawRectangleRec(btnJugar, CheckCollisionPointRec(mousePoint, btnJugar) ? SKYBLUE : LIGHTGRAY);
        		DrawRectangleRec(btnStats, CheckCollisionPointRec(mousePoint, btnStats) ? SKYBLUE : LIGHTGRAY);
        		DrawRectangleRec(btnHistorial, CheckCollisionPointRec(mousePoint, btnHistorial) ? SKYBLUE : LIGHTGRAY);
        		DrawRectangleRec(btnSalir, CheckCollisionPointRec(mousePoint, btnSalir) ? RED : LIGHTGRAY);

        				// Bordes
       			DrawRectangleLinesEx(btnJugar, 2, DARKBLUE);
        		DrawRectangleLinesEx(btnStats, 2, DARKBLUE);
        		DrawRectangleLinesEx(btnHistorial, 2, DARKBLUE);
        		DrawRectangleLinesEx(btnSalir, 2, DARKBLUE);

        				// Textos de botones
        		DrawText("JUGAR", btnJugar.x + 60, btnJugar.y + 15, 20, BLACK);
    			DrawText("ESTADISTICAS", btnStats.x + 10, btnStats.y + 15, 20, BLACK);
    			DrawText("HISTORIAL", btnHistorial.x + 45, btnHistorial.y + 15, 20, BLACK);
       			DrawText("SALIR", btnSalir.x + 70, btnSalir.y + 15, 20, BLACK);
     		
        	break;
        
        case JUGAR:
        	ClearBackground(GRAY);
    		
        	if(preguntaActual != NULL){
        		mostrarPreguntas(preguntaActual, numeroPregunta);
				leerTextoRTA(&preguntaActual, &numeroPregunta);
								
        	}else{
        		if(preguntaActual == NULL){
        			DrawText("FIN DEL JUEGO!", 400, 200, 40, DARKGREEN);
        			DrawText("Presiona ENTER para volver al menu", 300, 260, 30, BLACK);
        			if (!guardadoHecho) {
                       /* guardar resultados de la pila en la cola solo 1 vez */
                       guardarResultadosEnCola(pila, &guardarALaCola);
                       guardarPartidaEnArchivo(guardarALaCola);
                       guardadoHecho = true;
                    }
					if (IsKeyPressed(KEY_ENTER)) {
            			reiniciarJuego();
        			}					
        		}
        	}
        		
        	
       		DrawRectangleRec(btnVolver, CheckCollisionPointRec(mousePoint, btnVolver) ? RED : LIGHTGRAY);
       		DrawText("<-VOLVER", btnVolver.x + 5, btnVolver.y + 10, 18, BLACK);
		
        	if(IsKeyPressed(KEY_ESCAPE)){
        	    reiniciarJuego();
        	}
        	break;
        	
        case STATS:
        	ClearBackground(RAYWHITE);
        	
    		DrawText("Usa la rueda del mouse para desplazarte", 20, 60, 20, GRAY);
    		
            int totCorr = totalCorrectas(guardarALaCola);
            int totPreg = 0;
            if (guardarALaCola != NULL) {
            tNodoGuardado* aux = guardarALaCola->principio;
            while (aux) { totPreg++; aux = aux->siguiente; }
            }
            
            // Calcular porcentaje
			float porcentaje;
			if(totPreg == 0){
				porcentaje = 0;
			}else {
				porcentaje = (totCorr * 100.0f) / totPreg;
			}
			
			// Mostrar resumen
            DrawText(TextFormat("Correctas: %d / %d (%.1f%%)", totCorr, totPreg, porcentaje), 50, 80, 25, DARKGREEN);
    		
    		// Mostrar la lista (esta función ya es segura ante NULL)
			mostrarPreguntasGuardadas(guardarALaCola);

			DrawRectangleRec(btnVolver, CheckCollisionPointRec(mousePoint, btnVolver) ? RED : LIGHTGRAY);
       		DrawText("<-VOLVER", btnVolver.x + 5, btnVolver.y + 10, 18, BLACK);
		
        	if(IsKeyPressed(KEY_ESCAPE)){
            	pantallaActual = MENU;
        	}
        	
        break;
        
        case HISTORIAL:
            ClearBackground(RAYWHITE);

            DrawText("HISTORIAL DE PARTIDAS", 40, 70, 30, DARKBLUE);
            DrawText("Usa la rueda del mouse para desplazarte", 40, 100, 20, GRAY);

            mostrarHistorialPartidas();

            DrawRectangleRec(btnVolver, CheckCollisionPointRec(mousePoint, btnVolver) ? RED : LIGHTGRAY);
            DrawText("<-VOLVER", btnVolver.x + 5, btnVolver.y + 10, 18, BLACK);

            if(IsKeyPressed(KEY_ESCAPE)){
            	pantallaActual = MENU;
            }
                
        break;
    }
    EndDrawing();
}

void leerTextoRTA(tPilaResultados** PregActual, int* numeroPregunta){

    static char respuesta = '\0';
    const float tiempoMax = 5.0f;

    tiempoRestante -= GetFrameTime();

    DrawText(TextFormat("Tiempo: %.1f", tiempoRestante), 950, 100, 30, RED);

    float anchoBarra = (tiempoRestante / tiempoMax) * 300;
    DrawRectangle(50, 100, anchoBarra, 25, RED);
    DrawRectangleLines(50, 100, 300, 25, BLACK);

    if (tiempoRestante < 0)
        tiempoRestante = 0;

    if (IsKeyPressed(KEY_A)) respuesta = 'A';
    if (IsKeyPressed(KEY_B)) respuesta = 'B';
    if (IsKeyPressed(KEY_C)) respuesta = 'C';
    if (IsKeyPressed(KEY_D)) respuesta = 'D';

    DrawText(TextFormat("Tu respuesta: %c", respuesta), 50, 500, 20, DARKGREEN);

    // --- ENTER ---
    if (respuesta != '\0' && IsKeyPressed(KEY_ENTER)) {

        guardarRespuestadicha(*PregActual, respuesta);

        *PregActual = (*PregActual)->siguiente;
        (*numeroPregunta)++;
    	
        respuesta = '\0';
        tiempoRestante = tiempoMax;
    }

    // --- TIEMPO AGOTADO ---
    if (tiempoRestante == 0) {

        guardarRespuestadicha(*PregActual, respuesta);
		
        *PregActual = (*PregActual)->siguiente;
    	(*numeroPregunta)++;
    	
    	
        respuesta = '\0';
        tiempoRestante = tiempoMax;
    }
					
}

void reiniciarJuego(){
	guardadoHecho = false;
	
	vaciarPila(&pila);
	
	inicializar(&pila);
	todasLasPreguntas(&pila);
	preguntaActual = pila;
	
	tiempoRestante = 5.0f;
	numeroPregunta = 1;
	pantallaActual = MENU;
}

