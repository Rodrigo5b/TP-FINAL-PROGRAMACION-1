// PROTOTIPOS
void llamarTitulo(void);
void limpiarPantallaLUDO(void); // <-- Nueva función
int tirarDado(int jugador, int posicion);
int avanzarPieza(int posicion, int valorDado, int jugadorNro);
int procesarTurnoJugador(int idJugador, int posicionActual, bool *salioDeCasa);
void ganador(bool juegoActivo, int posicionTabla[], int POSICION_META, int i);
int ludo() {
    const int MIN_JUGADORES = 2;
    const int MAX_JUGADORES = 4;
    const int POSICION_META = 57;

    int cantidadJugadores = 0;
    bool juegoActivo = true;

    bool estadoJugador[4] = {false, false, false, false};
    int posicionTabla[4] = {0, 0, 0, 0};

    srand(time(NULL));
    
    limpiarPantalla();
    llamarTitulo();

    cantidadJugadores = leerEnteroEntre(MIN_JUGADORES, MAX_JUGADORES, "Elija la cantidad de jugadores [2] [3] [4]");



    while (juegoActivo) {
        for (int i = 0; i < cantidadJugadores && juegoActivo; i++) {
            
            // 1. Limpiamos la pantalla al iniciar el turno del jugador
            limpiarPantalla();
            
            // 2. Volvemos a mostrar el título para mantener la estética
            llamarTitulo();

            printf("----------------------------------------------------------------\n");
            
            // 3. Procesamos el turno
            posicionTabla[i] = procesarTurnoJugador(i + 1, posicionTabla[i], &estadoJugador[i]);

            // Evaluamos victoria
            if (posicionTabla[i] >= POSICION_META) {
                juegoActivo = false;
                printf("\n====================================\n");
                printf("  ¡GANO EL JUGADOR NRO: %d!\n", i + 1);
                printf("====================================\n");
            } else {
                // Pausa antes de pasar al siguiente turno/limpiar pantalla
                leerCaracter("\nPresione ENTER para finalizar su turno y pasar al siguiente...");
            }
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////
/////////////////////////  IMPLEMENTACIONES  //////////////////////////////
///////////////////////////////////////////////////////////////////////////
void ganador(bool juegoActivo, int posicionTabla[], int POSICION_META, int i){
        if (posicionTabla[i] >= POSICION_META) {
        juegoActivo = false;
        printf("\n====================================\n");
        printf("  ¡GANO EL JUGADOR NRO: %d!\n", i + 1);
        printf("====================================\n");
    } else {
        // Pausa antes de pasar al siguiente turno/limpiar pantalla
        leerCaracter("\nPresione ENTER para finalizar su turno y pasar al siguiente...");
    }
}

void llamarTitulo(void) {
    printf("  _        _    _   _____     ____  \n");
    printf(" | |      | |  | | |  __ \\   / __ \\ \n");
    printf(" | |      | |  | | | |  | | | |  | |\n");
    printf(" | |      | |  | | | |  | | | |  | |\n");
    printf(" | |____  | |__| | | |__| | | |__| |\n");
    printf(" |______|  \\____/  |_____/   \\____/ \n\n");
}

int tirarDado(int jugador, int posicion) {
    int minDado = 1, maxDado = 6;
    leerCaracter("Presione ENTER para tirar el dado: ");
    int resultado = (rand() % (maxDado - minDado + 1)) + minDado;
    printf("-> El jugador %d obtuvo un: %d\n", jugador, resultado);
    return resultado;
}

int avanzarPieza(int posicion, int valorDado, int jugadorNro) {
    int nuevaPosicion = posicion + valorDado;
    printf("-> El jugador %d avanza a la posicion: %d\n", jugadorNro, nuevaPosicion);
    return nuevaPosicion;
}

int procesarTurnoJugador(int idJugador, int posicionActual, bool *salioDeCasa) {
    printf("Turno del jugador: %d | Posicion actual: %d\n", idJugador, posicionActual);
    
    int dado = tirarDado(idJugador, posicionActual);

    if (!(*salioDeCasa)) {
        if (dado == 6) {
            *salioDeCasa = true;
            printf("¡Exito! El jugador %d saco un 6 y sale de la casa.\n", idJugador);
            return procesarTurnoJugador(idJugador, posicionActual, salioDeCasa);
        } else {
            printf("El jugador %d debe obtener un 6 para salir de la casa.\n", idJugador);
            return posicionActual;
        }
    }

    posicionActual = avanzarPieza(posicionActual, dado, idJugador);

    if (dado == 6 && posicionActual < 57) {
        printf("¡Obtuviste un 6! Tiras el dado nuevamente.\n");
        posicionActual = procesarTurnoJugador(idJugador, posicionActual, salioDeCasa);
    }

    return posicionActual;
}