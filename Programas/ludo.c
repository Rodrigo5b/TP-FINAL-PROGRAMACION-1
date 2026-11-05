
#define LUDO_MAX_JUGADORES 4
#define LUDO_CANT_FICHAS 4
#define LUDO_LADO_TABLERO 15
#define LUDO_CASILLAS_CAMINO 52
#define LUDO_FIN_CAMINO 51
#define LUDO_INICIO_META 52
#define LUDO_META 57
#define LUDO_CARA_MAX 6
#define LUDO_SALIR 3
#define LUDO_CONTRA_PC 1
#define LUDO_CON_AMIGOS 2
#define LUDO_VOLVER_CANTIDAD 5
#define LUDO_CELDA_TEXTO 5
#define LUDO_FOTOGRAMAS_DADO 8
#define LUDO_DEMORA_DADO_MS 150
#define LUDO_DEMORA_PASO_MS 600
#define LUDO_DEMORA_AVISO_MS 1000
#define LUDO_DEMORA_PC_MS 1500
#define LUDO_SIN_GANADOR -1
#define LUDO_ACCION_ABANDONAR 0
#define LUDO_ACCION_SACAR 1
#define LUDO_ACCION_MOVER 2
#define LUDO_TIRADA_ABANDONADA 0
#define LUDO_TIRADA_NORMAL 1
#define LUDO_TIRADA_EXTRA 2
#define LUDO_TIRADA_GANADA 3

typedef struct {
    int posicion; /* 0: casa, 1..56: recorrido, 57: meta */
} FichaLudo;

typedef struct {
    FichaLudo fichas[LUDO_CANT_FICHAS];
    bool esComputadora;
} JugadorLudo;

typedef struct {
    JugadorLudo jugadores[LUDO_MAX_JUGADORES];
    int cantidadJugadores;
    int ganador; /* Indice base cero o LUDO_SIN_GANADOR */
    bool activa;
} PartidaLudo;

typedef struct {
    int fila;
    int columna;
} CoordenadaLudo;

typedef struct {
    int jugador;
    int ficha;
    int dado;
} MovimientoLudo;

typedef struct {
    char celdas[LUDO_LADO_TABLERO][LUDO_LADO_TABLERO][LUDO_CELDA_TEXTO];
    int ocupantes[LUDO_LADO_TABLERO][LUDO_LADO_TABLERO];
} TableroVisualLudo;

/* =============== 2. REGLAS Y ESTADO DEL JUEGO =============== */

void iniciarPartidaLudo(PartidaLudo *partida, int cantidad, bool contraPc) {
    partida->cantidadJugadores = cantidad;
    partida->ganador = LUDO_SIN_GANADOR;
    partida->activa = true;
    for (int jugador = 0; jugador < LUDO_MAX_JUGADORES; jugador++) {
        partida->jugadores[jugador].esComputadora = contraPc && jugador == 1;
        for (int ficha = 0; ficha < LUDO_CANT_FICHAS; ficha++) {
            partida->jugadores[jugador].fichas[ficha].posicion = 0;
        }
    }
}

bool estaEnCasaLudo(const FichaLudo *ficha) {
    return ficha->posicion == 0;
}

bool puedeAvanzarLudo(const FichaLudo *ficha) {
    return ficha->posicion > 0 && ficha->posicion < LUDO_META;
}

int buscarFichaEnCasaLudo(const JugadorLudo *jugador) {
    for (int i = 0; i < LUDO_CANT_FICHAS; i++) {
        if (estaEnCasaLudo(&jugador->fichas[i])) return i;
    }
    return -1;
}

int buscarFichaMovibleLudo(const JugadorLudo *jugador) {
    for (int i = 0; i < LUDO_CANT_FICHAS; i++) {
        if (puedeAvanzarLudo(&jugador->fichas[i])) return i;
    }
    return -1;
}

int posicionGlobalLudo(int jugador, int posicionRelativa) {
    return ((jugador * 13 + posicionRelativa - 1) % LUDO_CASILLAS_CAMINO) + 1;
}

bool esCasillaSeguraLudo(int global) {
    const int seguras[8] = {1, 9, 14, 22, 27, 35, 40, 48};
    for (int i = 0; i < 8; i++) {
        if (seguras[i] == global) return true;
    }
    return false;
}

bool fichaOcupaCasillaLudo(const FichaLudo *ficha, int jugador, int global) {
    if (ficha->posicion < 1 || ficha->posicion > LUDO_FIN_CAMINO) return false;
    return posicionGlobalLudo(jugador, ficha->posicion) == global;
}

int capturarRivalLudo(JugadorLudo *rival, int numeroRival, int global) {
    int cantidad = 0;
    for (int i = 0; i < LUDO_CANT_FICHAS; i++) {
        FichaLudo *ficha = &rival->fichas[i];
        if (fichaOcupaCasillaLudo(ficha, numeroRival, global)) {
            ficha->posicion = 0;
            cantidad++;
        }
    }
    return cantidad;
}

int capturarEnPosicionLudo(PartidaLudo *partida, int jugador, int posicion) {
    if (posicion < 1 || posicion > LUDO_FIN_CAMINO) return 0;
    int global = posicionGlobalLudo(jugador, posicion);
    if (esCasillaSeguraLudo(global)) return 0;

    int capturas = 0;
    for (int rival = 0; rival < partida->cantidadJugadores; rival++) {
        if (rival != jugador) {
            capturas += capturarRivalLudo(&partida->jugadores[rival], rival, global);
        }
    }
    return capturas;
}

int sacarFichaDeCasaLudo(PartidaLudo *partida, int jugador) {
    JugadorLudo *actual = &partida->jugadores[jugador];
    int indice = buscarFichaEnCasaLudo(actual);
    if (indice >= 0) actual->fichas[indice].posicion = 1;
    return indice;
}

int calcularPasosLudo(const PartidaLudo *partida, const MovimientoLudo *movimiento) {
    int posicion = partida->jugadores[movimiento->jugador]
                          .fichas[movimiento->ficha].posicion;
    int destino = posicion + movimiento->dado;
    if (destino > LUDO_META) destino = LUDO_META;
    return destino - posicion;
}

/* Actualiza exactamente una casilla: NO escribe ni espera. */
bool avanzarUnaCasillaLudo(PartidaLudo *partida, const MovimientoLudo *movimiento) {
    FichaLudo *ficha = &partida->jugadores[movimiento->jugador]
                           .fichas[movimiento->ficha];
    if (!puedeAvanzarLudo(ficha)) return false;
    ficha->posicion++;
    return true;
}

bool todasLasFichasEnMetaLudo(const JugadorLudo *jugador) {
    for (int i = 0; i < LUDO_CANT_FICHAS; i++) {
        if (jugador->fichas[i].posicion != LUDO_META) return false;
    }
    return true;
}

/* Se decide el ganador exclusivamente a partir del estado. */
bool actualizarGanadorLudo(PartidaLudo *partida, int jugador) {
    if (!todasLasFichasEnMetaLudo(&partida->jugadores[jugador])) return false;
    partida->ganador = jugador;
    partida->activa = false;
    return true;
}

int obtenerDadoLudo(void) {
    return rand() % LUDO_CARA_MAX + 1;
}

/* =============== 3. TABLERO Y PRESENTACION =============== */

CoordenadaLudo coordenadaCaminoLudo(int global) {
    const CoordenadaLudo camino[LUDO_CASILLAS_CAMINO] = {
        {13,6},{12,6},{11,6},{10,6},{9,6},
        {8,5},{8,4},{8,3},{8,2},{8,1},{8,0},
        {7,0},{6,0},
        {6,1},{6,2},{6,3},{6,4},{6,5},
        {5,6},{4,6},{3,6},{2,6},{1,6},{0,6},
        {0,7},{0,8},
        {1,8},{2,8},{3,8},{4,8},{5,8},
        {6,9},{6,10},{6,11},{6,12},{6,13},{6,14},
        {7,14},{8,14},
        {8,13},{8,12},{8,11},{8,10},{8,9},
        {9,8},{10,8},{11,8},{12,8},{13,8},{14,8},
        {14,7},{14,6}
    };
    return camino[global - 1];
}

CoordenadaLudo coordenadaCasaLudo(int jugador, int ficha) {
    const CoordenadaLudo casas[LUDO_MAX_JUGADORES][LUDO_CANT_FICHAS] = {
        {{12,1},{12,2},{13,1},{13,2}},
        {{1,1},{1,2},{2,1},{2,2}},
        {{1,12},{1,13},{2,12},{2,13}},
        {{12,12},{12,13},{13,12},{13,13}}
    };
    return casas[jugador][ficha];
}

CoordenadaLudo coordenadaMetaLudo(int jugador, int posicion) {
    int paso = posicion - LUDO_FIN_CAMINO;
    CoordenadaLudo punto = {7, 7};
    if (jugador == 0) { punto.fila = 14 - paso; punto.columna = 7; }
    if (jugador == 1) { punto.fila = 7; punto.columna = paso; }
    if (jugador == 2) { punto.fila = paso; punto.columna = 7; }
    if (jugador == 3) { punto.fila = 7; punto.columna = 14 - paso; }
    return punto;
}

CoordenadaLudo coordenadaFichaLudo(const PartidaLudo *partida, int jugador, int ficha) {
    int posicion = partida->jugadores[jugador].fichas[ficha].posicion;
    if (posicion == 0) return coordenadaCasaLudo(jugador, ficha);
    if (posicion <= LUDO_FIN_CAMINO) {
        return coordenadaCaminoLudo(posicionGlobalLudo(jugador, posicion));
    }
    return coordenadaMetaLudo(jugador, posicion);
}

void iniciarTableroVisualLudo(TableroVisualLudo *tablero) {
    for (int fila = 0; fila < LUDO_LADO_TABLERO; fila++) {
        for (int columna = 0; columna < LUDO_LADO_TABLERO; columna++) {
            strcpy(tablero->celdas[fila][columna], " X  ");
            tablero->ocupantes[fila][columna] = 0;
        }
    }
}

void pintarCasillaLudo(TableroVisualLudo *tablero, CoordenadaLudo punto, const char *texto) {
    strcpy(tablero->celdas[punto.fila][punto.columna], texto);
}

void marcarCaminoLudo(TableroVisualLudo *tablero) {
    for (int posicion = 1; posicion <= LUDO_CASILLAS_CAMINO; posicion++) {
        CoordenadaLudo punto = coordenadaCaminoLudo(posicion);
        pintarCasillaLudo(tablero, punto, esCasillaSeguraLudo(posicion) ? " S  " : " .  ");
    }
}

void marcarCasillasFinalesLudo(TableroVisualLudo *tablero) {
    for (int jugador = 0; jugador < LUDO_MAX_JUGADORES; jugador++) {
        for (int paso = LUDO_INICIO_META; paso <= LUDO_META; paso++) {
            pintarCasillaLudo(tablero, coordenadaMetaLudo(jugador, paso), " .  ");
        }
        for (int ficha = 0; ficha < LUDO_CANT_FICHAS; ficha++) {
            pintarCasillaLudo(tablero, coordenadaCasaLudo(jugador, ficha), " .  ");
        }
    }
}

void agregarFichaVisualLudo(TableroVisualLudo *tablero, CoordenadaLudo punto,
                            int jugador, int ficha) {
    int cantidad = ++tablero->ocupantes[punto.fila][punto.columna];
    char *celda = tablero->celdas[punto.fila][punto.columna];
    if (cantidad == 1) {
        celda[0] = (char)('1' + jugador);
        celda[1] = '.';
        celda[2] = (char)('1' + ficha);
    } else {
        celda[0] = cantidad < 10 ? (char)('0' + cantidad) : '*';
        celda[1] = '*';
        celda[2] = ' ';
    }
    celda[3] = ' ';
    celda[4] = '\0';
}

void ubicarFichasVisualesLudo(const PartidaLudo *partida, TableroVisualLudo *tablero) {
    for (int jugador = 0; jugador < partida->cantidadJugadores; jugador++) {
        for (int ficha = 0; ficha < LUDO_CANT_FICHAS; ficha++) {
            CoordenadaLudo punto = coordenadaFichaLudo(partida, jugador, ficha);
            agregarFichaVisualLudo(tablero, punto, jugador, ficha);
        }
    }
}

/* Convierte estado en celdas. No modifica la partida ni imprime. */
void construirTableroVisualLudo(const PartidaLudo *partida, TableroVisualLudo *tablero) {
    iniciarTableroVisualLudo(tablero);
    marcarCaminoLudo(tablero);
    marcarCasillasFinalesLudo(tablero);
    ubicarFichasVisualesLudo(partida, tablero);
}

void mostrarResumenFichasLudo(const PartidaLudo *partida) {
    for (int jugador = 0; jugador < partida->cantidadJugadores; jugador++) {
        printf("Jugador %d:", jugador + 1);
        for (int ficha = 0; ficha < LUDO_CANT_FICHAS; ficha++) {
            int posicion = partida->jugadores[jugador].fichas[ficha].posicion;
            if (posicion == 0) printf(" F%d=casa", ficha + 1);
            else if (posicion == LUDO_META) printf(" F%d=META", ficha + 1);
            else printf(" F%d=%d", ficha + 1, posicion);
        }
        putchar('\n');
    }
}

void mostrarTableroLudo(const PartidaLudo *partida) {
    TableroVisualLudo tablero;
    construirTableroVisualLudo(partida, &tablero);
    printf("\n TABLERO LUDO (S=segura; 2*=dos fichas en una celda)\n");
    for (int fila = 0; fila < LUDO_LADO_TABLERO; fila++) {
        for (int columna = 0; columna < LUDO_LADO_TABLERO; columna++) {
            printf("%s", tablero.celdas[fila][columna]);
        }
        putchar('\n');
    }
    mostrarResumenFichasLudo(partida);
}

void mostrarEscenaLudo(const PartidaLudo *partida, const char *titulo) {
    limpiarPantalla();
    printf("\n%s\n", titulo);
    mostrarTableroLudo(partida);
    fflush(stdout);
}

void dibujarDadoLudo(int valor) {
    const char *caras[LUDO_CARA_MAX][3] = {
        {"         ",
         "    *    ",
         "         "},

         {"  *      ",
         "         ",
         "      *  "}, 

        {"  *      ",
         "    *    ",
         "      *  "},
            
        {"  *   *  ",
         "         ",
         "  *   *  "},
            
        {"  *   *  ",
         "    *    ",
         "  *   *  "},
            
        {"  *   *  ",
         "  *   *  ",
         "  *   *  "}
    };
    printf("  +---------+\n  |%s|\n  |%s|\n  |%s|\n  +---------+\n",
           caras[valor - 1][0], caras[valor - 1][1], caras[valor - 1][2]);
}

/* =============== 4. ENTRADA Y ANIMACIONES =============== */

void animarDadoLudo(const PartidaLudo *partida, int jugador, int resultado) {
    int caraAnterior = 0;
    for (int fotograma = 1; fotograma <= LUDO_FOTOGRAMAS_DADO; fotograma++) {
        int cara = obtenerDadoLudo();
        if (cara == caraAnterior) cara = cara % LUDO_CARA_MAX + 1;
        caraAnterior = cara;
        mostrarEscenaLudo(partida, "EL DADO ESTA GIRANDO...");
        printf("Jugador %d\n", jugador + 1);
        dibujarDadoLudo(cara);
        esperarMilisegundos(LUDO_DEMORA_DADO_MS);
    }
    mostrarEscenaLudo(partida, "RESULTADO DEL DADO");
    printf("Jugador %d saco un %d\n", jugador + 1, resultado);
    dibujarDadoLudo(resultado);
    esperarMilisegundos(LUDO_DEMORA_AVISO_MS);
}

bool pedirLanzamientoLudo(void) {
    char respuesta[20];
    while (true) {
        if (!leerLinea("ENTER para tirar el dado (0=abandonar): ", respuesta, sizeof respuesta)) return false;
        if (strcmp(respuesta, "0") == 0) return false;
        if (respuesta[0] == '\0') return true;
        printf("Solo ENTER para tirar o 0 para salir.\n");
    }
}

/* Lee opciones de Ludo sin depender de otros modulos del proyecto.
   Devuelve false cuando no hay mas entrada (EOF). */
bool leerOpcionLudo(int minimo, int maximo, const char *mensaje, int *opcion) {
    char linea[64];
    int numero;
    char sobrante;

    while (leerLinea(mensaje, linea, sizeof linea)) {
        if (sscanf(linea, " %d %c", &numero, &sobrante) == 1 &&
            numero >= minimo && numero <= maximo) {
            *opcion = numero;
            return true;
        }
        printf("Opcion invalida. Ingrese un numero entre %d y %d.\n", minimo, maximo);
    }
    return false;
}

/* La interfaz solo elige un numero; la regla verifica si es posible moverlo. */
int pedirFichaLudo(const JugadorLudo *jugador) {
    while (true) {
        int opcion;
        if (!leerOpcionLudo(0, LUDO_CANT_FICHAS,
                             "Elegir ficha [1-4] (0=abandonar): ", &opcion)) return -1;
        if (opcion == 0) return -1;
        int indice = opcion - 1;
        if (puedeAvanzarLudo(&jugador->fichas[indice])) return indice;
        printf("Esa ficha esta en casa o ya llego a la meta.\n");
    }
}

int pedirAccionLudo(bool puedeSacar, bool puedeMover) {
    if (puedeSacar && puedeMover) {
        int opcion;
        if (!leerOpcionLudo(LUDO_ACCION_ABANDONAR, LUDO_ACCION_MOVER,
                             "1. Sacar ficha  2. Mover ficha  0. Abandonar: ", &opcion)) {
            return LUDO_ACCION_ABANDONAR;
        }
        return opcion;
    }
    if (puedeSacar) return LUDO_ACCION_SACAR;
    return LUDO_ACCION_MOVER;
}

void anunciarCapturasLudo(const PartidaLudo *partida, int jugador, int capturas) {
    if (capturas <= 0) return;
    mostrarEscenaLudo(partida, "CAPTURA!");
    printf("Jugador %d envio %d ficha(s) rival(es) a casa.\n", jugador + 1, capturas);
    esperarMilisegundos(LUDO_DEMORA_AVISO_MS);
}

void anunciarFichaFueraDeCasaLudo(const PartidaLudo *partida, int jugador, int ficha) {
    mostrarEscenaLudo(partida, "FICHA FUERA DE CASA");
    printf("Jugador %d saco la ficha %d de casa.\n", jugador + 1, ficha + 1);
    esperarMilisegundos(LUDO_DEMORA_AVISO_MS);
}

void mostrarPasoLudo(const PartidaLudo *partida, const MovimientoLudo *movimiento,
                     int paso, int total) {
    bool esPc = partida->jugadores[movimiento->jugador].esComputadora;
    mostrarEscenaLudo(partida, esPc ? "MOVIMIENTO PC" : "MOVIMIENTO");
    int posicion = partida->jugadores[movimiento->jugador]
                          .fichas[movimiento->ficha].posicion;
    printf("Jugador %d, ficha %d: paso %d/%d (posicion %d)\n",
           movimiento->jugador + 1, movimiento->ficha + 1, paso, total, posicion);
    esperarMilisegundos(LUDO_DEMORA_PASO_MS);
}

/* Coordina el movimiento visual, pero deja las reglas en funciones distintas. */
void animarMovimientoLudo(PartidaLudo *partida, const MovimientoLudo *movimiento) {
    int pasos = calcularPasosLudo(partida, movimiento);
    for (int paso = 1; paso <= pasos; paso++) {
        avanzarUnaCasillaLudo(partida, movimiento);
        mostrarPasoLudo(partida, movimiento, paso, pasos);
    }
    int posicionFinal = partida->jugadores[movimiento->jugador]
                              .fichas[movimiento->ficha].posicion;
    int capturas = capturarEnPosicionLudo(partida, movimiento->jugador, posicionFinal);
    anunciarCapturasLudo(partida, movimiento->jugador, capturas);
}

void anunciarDecisionPcLudo(const PartidaLudo *partida, const char *mensaje) {
    mostrarEscenaLudo(partida, mensaje);
    esperarMilisegundos(LUDO_DEMORA_PC_MS);
}

void mostrarFinTurnoLudo(bool computadora, bool repite) {
    if (computadora) {
        if (repite) printf("La PC repite turno por sacar un 6.\n");
        else printf("La PC termino su turno.\n");
        esperarMilisegundos(LUDO_DEMORA_PC_MS);
    } else if (repite) {
        esperarEnter("Sacaste un 6. ENTER para repetir...");
    } else {
        esperarEnter("ENTER para terminar el turno...");
    }
}

/* =============== 5. COORDINACION (SIN REGLAS DUPLICADAS) =============== */

int seleccionarFichaLudo(const PartidaLudo *partida, int jugador) {
    const JugadorLudo *actual = &partida->jugadores[jugador];
    if (actual->esComputadora) return buscarFichaMovibleLudo(actual);
    return pedirFichaLudo(actual);
}

int seleccionarAccionLudo(const PartidaLudo *partida, int jugador, int dado) {
    const JugadorLudo *actual = &partida->jugadores[jugador];
    bool puedeSacar = dado == LUDO_CARA_MAX && buscarFichaEnCasaLudo(actual) >= 0;
    bool puedeMover = buscarFichaMovibleLudo(actual) >= 0;
    if (actual->esComputadora) {
        if (puedeSacar) return LUDO_ACCION_SACAR;
        return LUDO_ACCION_MOVER;
    }
    return pedirAccionLudo(puedeSacar, puedeMover);
}

void ejecutarSalidaDeCasaLudo(PartidaLudo *partida, int jugador) {
    int ficha = sacarFichaDeCasaLudo(partida, jugador);
    if (ficha < 0) return;
    anunciarFichaFueraDeCasaLudo(partida, jugador, ficha);
    int capturas = capturarEnPosicionLudo(partida, jugador, 1);
    anunciarCapturasLudo(partida, jugador, capturas);
}

bool ejecutarEleccionDeMovimientoLudo(PartidaLudo *partida, int jugador, int dado) {
    int ficha = seleccionarFichaLudo(partida, jugador);
    if (ficha < 0) return false;
    MovimientoLudo movimiento = {jugador, ficha, dado};
    if (partida->jugadores[jugador].esComputadora) {
        mostrarEscenaLudo(partida, "DECISION DE LA PC");
        printf("La PC movera la ficha %d con un dado de %d.\n", ficha + 1, dado);
        esperarMilisegundos(LUDO_DEMORA_PC_MS);
    }
    animarMovimientoLudo(partida, &movimiento);
    return true;
}

bool realizarJugadaLudo(PartidaLudo *partida, int jugador, int dado) {
    int accion = seleccionarAccionLudo(partida, jugador, dado);
    if (accion == LUDO_ACCION_ABANDONAR) return false;
    if (accion == LUDO_ACCION_SACAR) {
        if (partida->jugadores[jugador].esComputadora) {
            anunciarDecisionPcLudo(partida, "La PC sacara una ficha de casa.");
        }
        ejecutarSalidaDeCasaLudo(partida, jugador);
        return true;
    }
    if (buscarFichaMovibleLudo(&partida->jugadores[jugador]) < 0) {
        mostrarEscenaLudo(partida, "No hay fichas disponibles para mover.");
        return true;
    }
    return ejecutarEleccionDeMovimientoLudo(partida, jugador, dado);
}

/* Gestiona una tirada: la entrada, el dado y la ejecucion se delegan.
   Devuelve un codigo que el turno interpreta sin reimplementar las reglas. */
int jugarTiradaLudo(PartidaLudo *partida, int jugador) {
    bool computadora = partida->jugadores[jugador].esComputadora;
    mostrarEscenaLudo(partida, "========== NUEVO TURNO ==========");
    printf("Jugador %d %s\n", jugador + 1, computadora ? "(PC)" : "");
    if (computadora) esperarMilisegundos(LUDO_DEMORA_PC_MS);
    if (!computadora && !pedirLanzamientoLudo()) return LUDO_TIRADA_ABANDONADA;

    int dado = obtenerDadoLudo();
    animarDadoLudo(partida, jugador, dado);
    if (!realizarJugadaLudo(partida, jugador, dado)) return LUDO_TIRADA_ABANDONADA;

    if (actualizarGanadorLudo(partida, jugador)) {
        mostrarEscenaLudo(partida, "PARTIDA TERMINADA");
        printf("GANADOR: Jugador %d llevo sus cuatro fichas a meta!\n", jugador + 1);
        return LUDO_TIRADA_GANADA;
    }
    return dado == LUDO_CARA_MAX ? LUDO_TIRADA_EXTRA : LUDO_TIRADA_NORMAL;
}

void procesarTurnoLudo(PartidaLudo *partida, int jugador) {
    bool computadora = partida->jugadores[jugador].esComputadora;
    bool turnoExtra = true;
    while (turnoExtra && partida->activa) {
        int resultado = jugarTiradaLudo(partida, jugador);
        if (resultado == LUDO_TIRADA_ABANDONADA) {
            partida->activa = false;
            printf("Partida abandonada.\n");
        } else if (resultado == LUDO_TIRADA_GANADA) {
            turnoExtra = false;
        } else {
            turnoExtra = resultado == LUDO_TIRADA_EXTRA;
            mostrarFinTurnoLudo(computadora, turnoExtra);
        }
    }
}

int jugarPartidaLudo(int cantidad, bool contraPc) {
    PartidaLudo partida;
    iniciarPartidaLudo(&partida, cantidad, contraPc);
    while (partida.activa) {
        for (int jugador = 0; jugador < partida.cantidadJugadores; jugador++) {
            if (partida.activa) procesarTurnoLudo(&partida, jugador);
        }
    }
    if (partida.ganador >= 0) esperarEnter("ENTER para volver a Ludo...");
    if (partida.ganador < 0) return 0;
    return partida.ganador == 0 ? 1 : -1;
}

void mostrarTituloLudo(void) {
    printf(".-----------------------------------------------------------------.\n");
    printf("|                                                                 |\n");
    printf("|   __        __    __   _______    ______                        |\n");
    printf("|  |  |      |  |  |  | |       |  /      |                       |\n");
    printf("|  | $$      | $$  | $$ | $$$$$$$ |  $$$$$$|                     |\n");
    printf("|  | $$      | $$  | $$ | $$  | $$ | $$  | $$                     |\n");
    printf("|  | $$      | $$  | $$ | $$  | $$ | $$  | $$                     |\n");
    printf("|  | $$_____ | $$__/ $$ | $$__/ $$ | $$__/ $$                     |\n");
    printf("|  |$$$$$$$$| |$$$$$$/  |$$$$$$$/   |$$$$$$/                      |\n");
    printf("|                                                                 |\n");
    printf("'-----------------------------------------------------------------'\n");
}

/* Misma presentacion que Truco: numero y opcion dentro de un recuadro. */
void mostrarOpcionesLudo(void) {
    printf("\n                     1\n");
    printf("              +-----------------------+\n");
    printf("              |    JUGAR CONTRA PC    |\n");
    printf("              +-----------------------+\n\n");
    printf("                     2\n");
    printf("              +-----------------------+\n");
    printf("              |    JUGAR CON AMIGOS   |\n");
    printf("              +-----------------------+\n\n");
    printf("                     3\n");
    printf("              +-----------------------+\n");
    printf("              |         VOLVER        |\n");
    printf("              +-----------------------+\n\n");
}
void mostrarMenuLudo(void) {
    limpiarPantalla();
    mostrarTituloLudo();
    mostrarOpcionesLudo();
}
int ludo(void) {
    int modo = 0;
    int ultimoResultado = 0;
    while (modo != LUDO_SALIR) {
        mostrarMenuLudo();
        if (!leerOpcionLudo(1, LUDO_SALIR, "Selecciona una opcion: ", &modo)) return ultimoResultado;
        if (modo == LUDO_CONTRA_PC) {
            ultimoResultado = jugarPartidaLudo(2, true);
        } else if (modo == LUDO_CON_AMIGOS) {
            int jugadores;
            if (!leerOpcionLudo(2, LUDO_VOLVER_CANTIDAD,
                                 "Cantidad de jugadores (2-4; 5=volver): ", &jugadores)) return ultimoResultado;
            if (jugadores != LUDO_VOLVER_CANTIDAD) {
                ultimoResultado = jugarPartidaLudo(jugadores, false);
                }
        }
    }
    return ultimoResultado;
}
