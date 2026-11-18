#define MAX_JUGADORES 4
#define CANT_FICHAS 4
#define LADO_TABLERO 15
#define CASILLAS_CAMINO 52
#define FIN_CAMINO 51
#define INICIO_META 52
#define META 57
#define CARA_MAX 6
#define TAMANO_CASA 6
#define FOTOGRAMAS_DADO 8

#define DEMORA_DADO_MS 150
#define DEMORA_PASO_MS 600
#define DEMORA_AVISO_MS 1000
#define DEMORA_PC_MS 1500

#define SIN_GANADOR -1
#define SIN_FICHA -1
#define SIN_JUGADOR -1
#define COLOR_SIN_FONDO 0
#define COLOR_CAMINO 5
#define COLOR_CENTRO 6

#define CONTRA_PC 1
#define CON_AMIGOS 2
#define SALIR 3
#define VOLVER_CANTIDAD 5
#define ACCION_ABANDONAR 0
#define ACCION_SACAR 1
#define ACCION_MOVER 2
#define TIRADA_ABANDONADA 0
#define TIRADA_NORMAL 1
#define TIRADA_EXTRA 2
#define TIRADA_GANADA 3

#define MODO_ANSI_WINDOWS 0x0004

typedef struct {
    int posiciones[CANT_FICHAS];
    bool esComputadora;
} Participante;

typedef struct {
    Participante jugadores[MAX_JUGADORES];
    int cantidadJugadores;
    int jugadorTurno;
    int dadoActual;
    int ganador;
    bool activa;
} EstadoPartida;

typedef struct {
    int fila;
    int columna;
} PosicionTablero;

typedef struct {
    int jugador;
    int ficha;
    int dado;
} MovimientoFicha;

typedef struct {
    bool puedeSacar;
    bool puedeMover;
} OpcionesTurno;

typedef struct {
    char simbolo;
    int fondo;
    int jugadorVisible;
} CasillaVisual;

typedef struct {
    CasillaVisual casillas[LADO_TABLERO][LADO_TABLERO];
} TableroVisual;

void iniciarPartida(EstadoPartida *partida, int cantidad, bool contraPc) {
    partida->cantidadJugadores = cantidad;
    partida->jugadorTurno = 0;
    partida->dadoActual = 0;
    partida->ganador = SIN_GANADOR;
    partida->activa = true;

    for (int jugador = 0; jugador < MAX_JUGADORES; jugador++) {
        partida->jugadores[jugador].esComputadora = contraPc && jugador == 1;
        for (int ficha = 0; ficha < CANT_FICHAS; ficha++) {
            partida->jugadores[jugador].posiciones[ficha] = 0;
        }
    }
}

bool estaEnCasa(int posicion) {
    return posicion == 0;
}

bool puedeAvanzar(int posicion) {
    return posicion > 0 && posicion < META;
}

int buscarFichaEnCasa(const Participante *jugador) {
    for (int ficha = 0; ficha < CANT_FICHAS; ficha++) {
        if (estaEnCasa(jugador->posiciones[ficha])) {
            return ficha;
        }
    }
    return SIN_FICHA;
}

int buscarFichaMovible(const Participante *jugador) {
    for (int ficha = 0; ficha < CANT_FICHAS; ficha++) {
        if (puedeAvanzar(jugador->posiciones[ficha])) {
            return ficha;
        }
    }
    return SIN_FICHA;
}

int calcularPosicionGlobal(int jugador, int posicionRelativa) {
    return ((jugador * 13 + posicionRelativa - 1) % CASILLAS_CAMINO) + 1;
}

bool esCasillaSegura(int posicionGlobal) {
    const int posicionesSeguras[8] = {1, 9, 14, 22, 27, 35, 40, 48};
    for (int i = 0; i < 8; i++) {
        if (posicionesSeguras[i] == posicionGlobal) {
            return true;
        }
    }
    return false;
}

bool fichaOcupaCasilla(int posicion, int jugador, int global) {
    bool estaEnCamino = posicion >= 1 && posicion <= FIN_CAMINO;
    bool ocupaLaCasilla = false;

    if (estaEnCamino) {
        ocupaLaCasilla = calcularPosicionGlobal(jugador, posicion) == global;
    }
    return ocupaLaCasilla;
}

int capturarRival(Participante *rival, int numeroRival, int global) {
    int capturas = 0;
    for (int ficha = 0; ficha < CANT_FICHAS; ficha++) {
        if (fichaOcupaCasilla(rival->posiciones[ficha], numeroRival, global)) {
            rival->posiciones[ficha] = 0;
            capturas++;
        }
    }
    return capturas;
}

int capturarEnPosicion(EstadoPartida *partida, int jugador, int posicion) {
    int capturas = 0;
    bool estaEnCamino = posicion >= 1 && posicion <= FIN_CAMINO;

    if (estaEnCamino) {
        int global = calcularPosicionGlobal(jugador, posicion);
        bool casillaSegura = esCasillaSegura(global);

        if (casillaSegura == false) {
            for (int rival = 0; rival < partida->cantidadJugadores; rival++) {
                if (rival != jugador) {
                    capturas += capturarRival(&partida->jugadores[rival], rival, global);
                }
            }
        }
    }
    return capturas;
}

int sacarFichaDeCasa(EstadoPartida *partida, int jugador) {
    Participante *actual = &partida->jugadores[jugador];
    int ficha = buscarFichaEnCasa(actual);
    if (ficha != SIN_FICHA) {
        actual->posiciones[ficha] = 1;
    }
    return ficha;
}

int calcularPasos(const EstadoPartida *partida, const MovimientoFicha *movimiento) {
    int posicion = partida->jugadores[movimiento->jugador].posiciones[movimiento->ficha];
    int destino = posicion + movimiento->dado;
    if (destino > META) {
        destino = META;
    }
    return destino - posicion;
}

bool avanzarUnaCasilla(EstadoPartida *partida, const MovimientoFicha *movimiento) {
    int *posicion = &partida->jugadores[movimiento->jugador].posiciones[movimiento->ficha];
    bool avanzo = false;

    if (puedeAvanzar(*posicion)) {
        (*posicion)++;
        avanzo = true;
    }
    return avanzo;
}

bool todasLasFichasEnMeta(const Participante *jugador) {
    for (int ficha = 0; ficha < CANT_FICHAS; ficha++) {
        if (jugador->posiciones[ficha] != META) {
            return false;
        }
    }
    return true;
}

bool actualizarGanador(EstadoPartida *partida, int jugador) {
    if (!todasLasFichasEnMeta(&partida->jugadores[jugador])) {
        return false;
    }
    partida->ganador = jugador;
    partida->activa = false;
    return true;
}

int obtenerDado(void) {
    return rand() % CARA_MAX + 1;
}

PosicionTablero obtenerCoordenadaCamino(int global) {
    const PosicionTablero camino[CASILLAS_CAMINO] = {
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

PosicionTablero obtenerOrigenCasa(int jugador) {
    PosicionTablero origen = {0, 0};
    if (jugador == 0 || jugador == 3) {
        origen.fila = LADO_TABLERO - TAMANO_CASA;
    }
    if (jugador == 2 || jugador == 3) {
        origen.columna = LADO_TABLERO - TAMANO_CASA;
    }
    return origen;
}

PosicionTablero obtenerCoordenadaCasa(int jugador, int ficha) {
    PosicionTablero punto = obtenerOrigenCasa(jugador);
    punto.fila += TAMANO_CASA / 2 - 1 + ficha / 2;
    punto.columna += TAMANO_CASA / 2 - 1 + ficha % 2;
    return punto;
}

PosicionTablero obtenerCoordenadaMeta(int jugador, int posicion) {
    int paso = posicion - FIN_CAMINO;
    PosicionTablero punto = {7, 7};
    if (jugador == 0) { punto.fila = 14 - paso; punto.columna = 7; }
    if (jugador == 1) { punto.fila = 7; punto.columna = paso; }
    if (jugador == 2) { punto.fila = paso; punto.columna = 7; }
    if (jugador == 3) { punto.fila = 7; punto.columna = 14 - paso; }
    return punto;
}

PosicionTablero obtenerCoordenadaFicha(const EstadoPartida *partida, int jugador, int ficha) {
    int posicion = partida->jugadores[jugador].posiciones[ficha];
    if (posicion == 0) {
        return obtenerCoordenadaCasa(jugador, ficha);
    }
    if (posicion <= FIN_CAMINO) {
        return obtenerCoordenadaCamino(calcularPosicionGlobal(jugador, posicion));
    }
    return obtenerCoordenadaMeta(jugador, posicion);
}

void iniciarTableroVisual(TableroVisual *tablero) {
    for (int fila = 0; fila < LADO_TABLERO; fila++) {
        for (int columna = 0; columna < LADO_TABLERO; columna++) {
            CasillaVisual *casilla = &tablero->casillas[fila][columna];
            casilla->simbolo = ' ';
            casilla->fondo = COLOR_SIN_FONDO;
            casilla->jugadorVisible = SIN_JUGADOR;
        }
    }
}

void marcarCamino(TableroVisual *tablero) {
    for (int posicion = 1; posicion <= CASILLAS_CAMINO; posicion++) {
        PosicionTablero punto = obtenerCoordenadaCamino(posicion);
        CasillaVisual *casilla = &tablero->casillas[punto.fila][punto.columna];
        casilla->simbolo = esCasillaSegura(posicion) ? 'S' : ' ';
        casilla->fondo = COLOR_CAMINO;
    }
}

void pintarCasaJugador(TableroVisual *tablero, int jugador) {
    PosicionTablero origen = obtenerOrigenCasa(jugador);
    for (int fila = origen.fila; fila < origen.fila + TAMANO_CASA; fila++) {
        for (int columna = origen.columna; columna < origen.columna + TAMANO_CASA; columna++) {
            bool esBorde = fila == origen.fila || fila == origen.fila + TAMANO_CASA - 1 ||
                           columna == origen.columna || columna == origen.columna + TAMANO_CASA - 1;
            CasillaVisual *casilla = &tablero->casillas[fila][columna];
            casilla->fondo = esBorde ? jugador + 1 : COLOR_CAMINO;
            casilla->simbolo = ' ';
        }
    }
}

void marcarCasas(TableroVisual *tablero) {
    for (int jugador = 0; jugador < MAX_JUGADORES; jugador++) {
        pintarCasaJugador(tablero, jugador);
    }
}

void marcarCarrilesMeta(TableroVisual *tablero) {
    for (int jugador = 0; jugador < MAX_JUGADORES; jugador++) {
        for (int posicion = INICIO_META; posicion < META; posicion++) {
            PosicionTablero punto = obtenerCoordenadaMeta(jugador, posicion);
            tablero->casillas[punto.fila][punto.columna].fondo = jugador + 1;
            tablero->casillas[punto.fila][punto.columna].simbolo = ' ';
        }
    }
}

void marcarSalidas(TableroVisual *tablero) {
    for (int jugador = 0; jugador < MAX_JUGADORES; jugador++) {
        PosicionTablero salida = obtenerCoordenadaCamino(calcularPosicionGlobal(jugador, 1));
        tablero->casillas[salida.fila][salida.columna].fondo = jugador + 1;
    }
}

void marcarCentro(TableroVisual *tablero) {
    tablero->casillas[7][7].fondo = COLOR_CENTRO;
    tablero->casillas[7][7].simbolo = 'M';
}

void agregarFichaVisual(TableroVisual *tablero, PosicionTablero punto,
                        int jugador, int ficha) {
    CasillaVisual *casilla = &tablero->casillas[punto.fila][punto.columna];
    casilla->simbolo = (char)('1' + ficha);
    casilla->jugadorVisible = jugador;
}

void ubicarFichasJugador(const EstadoPartida *partida, TableroVisual *tablero,
                         int jugador) {
    for (int ficha = 0; ficha < CANT_FICHAS; ficha++) {
        PosicionTablero punto = obtenerCoordenadaFicha(partida, jugador, ficha);
        agregarFichaVisual(tablero, punto, jugador, ficha);
    }
}

void ubicarFichasVisuales(const EstadoPartida *partida, TableroVisual *tablero) {
    for (int jugador = 0; jugador < partida->cantidadJugadores; jugador++) {
        if (jugador != partida->jugadorTurno) {
            ubicarFichasJugador(partida, tablero, jugador);
        }
    }
    ubicarFichasJugador(partida, tablero, partida->jugadorTurno);
}

void construirTableroVisual(const EstadoPartida *partida, TableroVisual *tablero) {
    iniciarTableroVisual(tablero);
    marcarCamino(tablero);
    marcarCasas(tablero);
    marcarCarrilesMeta(tablero);
    marcarSalidas(tablero);
    marcarCentro(tablero);
    ubicarFichasVisuales(partida, tablero);
}

const char *obtenerFondoColor(int color) {
    if (color == 1) return "\033[30;101m";  
    if (color == 2) return "\033[30;102m";  
    if (color == 3) return "\033[30;103m";  
    if (color == 4) return "\033[30;104m";  
    if (color == COLOR_CAMINO) return "\033[30;107m";  
    if (color == COLOR_CENTRO) return "\033[30;107m";
    return "\033[0m";
}

void habilitarColores(void) {
#ifdef _WIN32
    HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo;
    if (consola != INVALID_HANDLE_VALUE && consola != NULL && GetConsoleMode(consola, &modo)) {
        SetConsoleMode(consola, modo | MODO_ANSI_WINDOWS);
    }
#endif
}

void mostrarCelda(const CasillaVisual *casilla) {
    int fondo = casilla->fondo;
    if (casilla->jugadorVisible != SIN_JUGADOR) {
        fondo = casilla->jugadorVisible + 1;
    }
    if (fondo == COLOR_SIN_FONDO) {
        printf("   ");
    } else {
        printf("%s %c \033[0m", obtenerFondoColor(fondo), casilla->simbolo);
    }
}

void mostrarFila(const TableroVisual *tablero, int fila) {
    for (int columna = 0; columna < LADO_TABLERO; columna++) {
        mostrarCelda(&tablero->casillas[fila][columna]);
    }
    putchar('\n');
}

void mostrarTablero(const EstadoPartida *partida) {
    TableroVisual tablero;
    construirTableroVisual(partida, &tablero);
    printf("\nTABLERO LUDO (S=segura, M=meta, fichas 1..4)\n");
    for (int jugador = 0; jugador < partida->cantidadJugadores; jugador++) {
        printf("%s Jugador %d \033[0m  ", obtenerFondoColor(jugador + 1), jugador + 1);
    }
    putchar('\n');
    for (int fila = 0; fila < LADO_TABLERO; fila++) {
        mostrarFila(&tablero, fila);
    }
}

void dibujarCaraDado(int valor) {
    switch (valor) {
        case 1:
            printf("|         |\n");
            printf("|    *    |\n");
            printf("|         |\n");
                    
            return;
        case 2:
            printf("|  *      |\n");
            printf("|         |\n");
            printf("|      *  |\n");
                    
            return;
        case 3:
            printf("|  *      |\n");
            printf("|    *    |\n");
            printf("|      *  |\n");
                    
            return;
        case 4:
            printf("|  *   *  |\n");
            printf("|         |\n");
            printf("|  *   *  |\n");
                   
            return;
        case 5:
            printf("|  *   *  |\n");
            printf("|    *    |\n");
            printf("|  *   *  |\n");
                   
            return;
        case 6:
            printf("|  *   *  |\n");
            printf("|  *   *  |\n");
            printf("|  *   *  |\n");
                    
            return;
        default:
            printf("|         |\n");
            printf("| DADO ?  |\n");
            printf("|         |\n");
                    
            return;
    }
}

void dibujarDado(int valor) {
    printf("+---------+\n");
    dibujarCaraDado(valor);
    printf("+---------+\n");
}

void mostrarDadoDelTurno(const EstadoPartida *partida) {
    int jugador = partida->jugadorTurno;
    printf("\nTurno del jugador %d%s:\n", jugador + 1,
           partida->jugadores[jugador].esComputadora ? " (PC)" : "");
    if (partida->dadoActual > 0) {
        printf("El jugador %d saco un %d:\n", jugador + 1, partida->dadoActual);
        dibujarDado(partida->dadoActual);
    }
}

void mostrarEscena(const EstadoPartida *partida, const char *titulo) {
    limpiarPantalla();
    printf("\n%s\n", titulo);
    mostrarTablero(partida);
    mostrarDadoDelTurno(partida);
    fflush(stdout);
}

void mostrarTitulo(void) {
    printf(".-----------------------------------------------------------------.\n");
    printf("|                                                                 |\n");
    printf("|   __        __    __   _______    ______                        |\n");
    printf("|  |  |      |  |  |  | |       |  /      |                       |\n");
    printf("|  | $$      | $$  | $$ | $$$$$$$ |  $$$$$$|                      |\n");
    printf("|  | $$      | $$  | $$ | $$  | $$ | $$  | $$                     |\n");
    printf("|  | $$      | $$  | $$ | $$  | $$ | $$  | $$                     |\n");
    printf("|  | $$_____ | $$__/ $$ | $$__/ $$ | $$__/ $$                     |\n");
    printf("|  |$$$$$$$$| |$$$$$$/  |$$$$$$$/   |$$$$$$/                      |\n");
    printf("|                                                                 |\n");
    printf("'-----------------------------------------------------------------'\n");
}

void mostrarOpciones(void) {
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

void mostrarMenu(void) {
    limpiarPantalla();
    mostrarTitulo();
    mostrarOpciones();
}

void mostrarFinTurno(bool computadora, bool repite) {
    if (computadora) {
        if (repite) printf("La PC repite turno por sacar un 6.\n");
        else printf("La PC termino su turno.\n");
        esperarMilisegundos(DEMORA_PC_MS);
    } else if (repite) {
        esperarEnter("Sacaste un 6. ENTER para repetir...");
    } else {
        esperarEnter("ENTER para terminar el turno...");
    }
}

void anunciarCapturas(const EstadoPartida *partida, int capturas) {
    if (capturas > 0) {
        mostrarEscena(partida, "CAPTURA!");
        printf("Jugador %d envio %d ficha(s) rivales a casa.\n", partida->jugadorTurno + 1, capturas);
        esperarMilisegundos(DEMORA_AVISO_MS);
    }
}

void anunciarFichaFueraDeCasa(const EstadoPartida *partida, int ficha) {
    mostrarEscena(partida, "FICHA FUERA DE CASA");
    printf("Jugador %d saco la ficha %d de casa.\n", partida->jugadorTurno + 1, ficha + 1);
    esperarMilisegundos(DEMORA_AVISO_MS);
}

 
bool validarOpcion(const char *texto, int minimo, int maximo, int *opcion) {
    int numero;
    char caracterExtra;
    int cantidadLeida = sscanf(texto, " %d %c", &numero, &caracterExtra);

    if (cantidadLeida != 1) {
        return false;
    }
    if (numero < minimo) {
        return false;
    }
    if (numero > maximo) {
        return false;
    }

    *opcion = numero;
    return true;
}

bool leerOpcion(int minimo, int maximo, const char *mensaje, int *opcion) {
    char respuesta[64];
    bool hayEntrada = true;

    while (hayEntrada) {
        hayEntrada = leerLinea(mensaje, respuesta, sizeof respuesta);
        if (hayEntrada) {
            bool opcionCorrecta = validarOpcion(respuesta, minimo, maximo, opcion);
            if (opcionCorrecta) {
                return true;
            }
            printf("Opcion invalida. Ingrese un numero entre %d y %d.\n", minimo, maximo);
        }
    }
    return false;
}

bool pedirLanzamiento(void) {
    char respuesta[20];
    bool respuestaCorrecta = false;

    while (respuestaCorrecta == false) {
        bool pudoLeer = leerLinea("ENTER para tirar el dado (0=abandonar): ",
                                  respuesta, sizeof respuesta);
        if (pudoLeer == false) {
            return false;
        }
        if (strcmp(respuesta, "0") == 0) {
            return false;
        }
        if (respuesta[0] == '\0') {
            respuestaCorrecta = true;
        } else {
            printf("Solo ENTER para tirar o 0 para salir.\n");
        }
    }
    return true;
}

int pedirFicha(const Participante *jugador) {
    int fichaSeleccionada = SIN_FICHA;
    bool continuarPreguntando = true;

    while (continuarPreguntando) {
        int opcion = 0;
        bool pudoLeer = leerOpcion(0, CANT_FICHAS,
                    "Elija una ficha [1-4] (0=abandonar): ", &opcion);

        if (pudoLeer == false) {
            continuarPreguntando = false;
        } else if (opcion == ACCION_ABANDONAR) {
            continuarPreguntando = false;
        } else {
            int indiceFicha = opcion - 1;
            bool estaDisponible = puedeAvanzar(jugador->posiciones[indiceFicha]);

            if (estaDisponible) {
                fichaSeleccionada = indiceFicha;
                continuarPreguntando = false;
            } else {
                printf("Esa ficha esta en casa o ya llego a la meta.\n");
            }
        }
    }
    return fichaSeleccionada;
}

int pedirAccion(const OpcionesTurno *opciones) {
    bool puedeElegir = opciones->puedeSacar && opciones->puedeMover;
    if (puedeElegir) {
        int opcion = ACCION_ABANDONAR;
        bool pudoLeer = leerOpcion(ACCION_ABANDONAR, ACCION_MOVER,
                    "1. Sacar ficha  2. Mover ficha  0. Abandonar: ", &opcion);
        if (pudoLeer == false) {
            return ACCION_ABANDONAR;
        }
        return opcion;
    }
    if (opciones->puedeSacar) {
        return ACCION_SACAR;
    }
    return ACCION_MOVER;
}

void animarDado(const EstadoPartida *partida) {
    int caraAnterior = 0;
    for (int fotograma = 0; fotograma < FOTOGRAMAS_DADO; fotograma++) {
        int cara = obtenerDado();
        if (cara == caraAnterior) {
            cara = cara % CARA_MAX + 1;
        }
        caraAnterior = cara;
        mostrarEscena(partida, "EL DADO ESTA GIRANDO...");
        dibujarDado(cara);
        esperarMilisegundos(DEMORA_DADO_MS);
    }
}

void mostrarPaso(const EstadoPartida *partida, const MovimientoFicha *movimiento,
                     int paso, int total) {
    bool esPc = partida->jugadores[movimiento->jugador].esComputadora;
    mostrarEscena(partida, esPc ? "MOVIMIENTO PC" : "MOVIMIENTO");
    int posicion = partida->jugadores[movimiento->jugador].posiciones[movimiento->ficha];
    printf("Jugador %d, ficha %d: paso %d/%d (posicion %d)\n",
           movimiento->jugador + 1, movimiento->ficha + 1, paso, total, posicion);
    esperarMilisegundos(DEMORA_PASO_MS);
}

void animarMovimiento(EstadoPartida *partida, const MovimientoFicha *movimiento) {
    int pasos = calcularPasos(partida, movimiento);
    for (int paso = 1; paso <= pasos; paso++) {
        avanzarUnaCasilla(partida, movimiento);
        mostrarPaso(partida, movimiento, paso, pasos);
    }
}

int seleccionarFicha(const EstadoPartida *partida, int jugador) {
    const Participante *actual = &partida->jugadores[jugador];
    if (actual->esComputadora) {
        return buscarFichaMovible(actual);
    }
    return pedirFicha(actual);
}

OpcionesTurno calcularOpcionesTurno(const Participante *jugador, int dado) {
    OpcionesTurno opciones;
    opciones.puedeSacar = dado == CARA_MAX &&
                          buscarFichaEnCasa(jugador) != SIN_FICHA;
    opciones.puedeMover = buscarFichaMovible(jugador) != SIN_FICHA;
    return opciones;
}

int elegirAccionPc(const OpcionesTurno *opciones) {
    return opciones->puedeSacar ? ACCION_SACAR : ACCION_MOVER;
}

int seleccionarAccion(const EstadoPartida *partida, int jugador, int dado) {
    const Participante *actual = &partida->jugadores[jugador];
    OpcionesTurno opciones = calcularOpcionesTurno(actual, dado);
    if (actual->esComputadora) {
        return elegirAccionPc(&opciones);
    }
    return pedirAccion(&opciones);
}

void ejecutarSalidaDeCasa(EstadoPartida *partida, int jugador) {
    int ficha = sacarFichaDeCasa(partida, jugador);
    if (ficha != SIN_FICHA) {
        anunciarFichaFueraDeCasa(partida, ficha);
        int capturas = capturarEnPosicion(partida, jugador, 1);
        anunciarCapturas(partida, capturas);
    }
}

bool ejecutarEleccionDeMovimiento(EstadoPartida *partida, int jugador, int dado) {
    int ficha = seleccionarFicha(partida, jugador);
    if (ficha == SIN_FICHA) {
        return false;
    }
    MovimientoFicha movimiento = {jugador, ficha, dado};
    if (partida->jugadores[jugador].esComputadora) {
        mostrarEscena(partida, "DECISION DE LA PC");
        printf("La PC movera la ficha %d con un dado de %d.\n", ficha + 1, dado);
        esperarMilisegundos(DEMORA_PC_MS);
    }
    animarMovimiento(partida, &movimiento);
    int posicionFinal = partida->jugadores[jugador].posiciones[ficha];
    int capturas = capturarEnPosicion(partida, jugador, posicionFinal);
    anunciarCapturas(partida, capturas);
    return true;
}

bool realizarJugada(EstadoPartida *partida, int jugador, int dado) {
    int accion = seleccionarAccion(partida, jugador, dado);
    if (accion == ACCION_ABANDONAR) {
        return false;
    }
    if (accion == ACCION_SACAR) {
        if (partida->jugadores[jugador].esComputadora) {
            mostrarEscena(partida, "La PC sacara una ficha de casa.");
            esperarMilisegundos(DEMORA_PC_MS);
        }
        ejecutarSalidaDeCasa(partida, jugador);
        return true;
    }
    if (buscarFichaMovible(&partida->jugadores[jugador]) == SIN_FICHA) {
        mostrarEscena(partida, "NO HAY MOVIMIENTOS DISPONIBLES");
        printf("No puede salir de base: necesita sacar un 6.\n");
        return true;
    }
    return ejecutarEleccionDeMovimiento(partida, jugador, dado);
}

int jugarTirada(EstadoPartida *partida, int jugador) {
    bool esPc = partida->jugadores[jugador].esComputadora;
    partida->dadoActual = 0;
    mostrarEscena(partida, "========== NUEVO TURNO ==========");

    if (esPc) {
        esperarMilisegundos(DEMORA_PC_MS);
    } else {
        bool quiereLanzar = pedirLanzamiento();
        if (quiereLanzar == false) {
            return TIRADA_ABANDONADA;
        }
    }

    int dado = obtenerDado();
    animarDado(partida);
    partida->dadoActual = dado;
    mostrarEscena(partida, "RESULTADO DEL DADO");
    esperarMilisegundos(DEMORA_AVISO_MS);

    bool jugadaRealizada = realizarJugada(partida, jugador, dado);
    if (jugadaRealizada == false) {
        return TIRADA_ABANDONADA;
    }

    bool ganoLaPartida = actualizarGanador(partida, jugador);
    if (ganoLaPartida) {
        mostrarEscena(partida, "PARTIDA TERMINADA");
        printf("GANADOR: Jugador %d llevo sus cuatro fichas a meta!\n", jugador + 1);
        return TIRADA_GANADA;
    }

    if (dado == CARA_MAX) {
        return TIRADA_EXTRA;
    }
    return TIRADA_NORMAL;
}

void procesarTurno(EstadoPartida *partida, int jugador) {
    partida->jugadorTurno = jugador;
    bool esPc = partida->jugadores[jugador].esComputadora;
    bool continuarTurno = true;

    while (continuarTurno) {
        int resultado = jugarTirada(partida, jugador);
        if (resultado == TIRADA_ABANDONADA) {
            partida->activa = false;
            continuarTurno = false;
            printf("Partida abandonada.\n");
        } else if (resultado == TIRADA_GANADA) {
            continuarTurno = false;
        } else {
            bool tieneTurnoExtra = resultado == TIRADA_EXTRA;
            mostrarFinTurno(esPc, tieneTurnoExtra);
            continuarTurno = tieneTurnoExtra;
        }
    }
}

int jugarPartida(int cantidad, bool contraPc) {
    if (cantidad < 2 || cantidad > MAX_JUGADORES) {
        return 0;
    }
    EstadoPartida partida;
    iniciarPartida(&partida, cantidad, contraPc);
    int jugador = 0;
    while (partida.activa) {
        procesarTurno(&partida, jugador);
        jugador = (jugador + 1) % partida.cantidadJugadores;
    }
    if (partida.ganador >= 0) {
        esperarEnter("ENTER para volver a Ludo...");
    }
    if (partida.ganador < 0) {
        return 0;
    }
    return partida.ganador == 0 ? 1 : -1;
}

bool jugarConAmigos(int *ultimoResultado) {
    int cantidadJugadores = 0;
    bool pudoLeer = leerOpcion(2, VOLVER_CANTIDAD,
                 "Cantidad de jugadores (2-4; 5=volver): ", &cantidadJugadores);
    if (pudoLeer == false) {
        return false;
    }
    if (cantidadJugadores != VOLVER_CANTIDAD) {
        *ultimoResultado = jugarPartida(cantidadJugadores, false);
    }
    return true;
}

bool ejecutarOpcionMenu(int opcion, int *ultimoResultado) {
    if (opcion == CONTRA_PC) {
        *ultimoResultado = jugarPartida(2, true);
        return true;
    }
    if (opcion == CON_AMIGOS) {
        return jugarConAmigos(ultimoResultado);
    }
    return false;
}

int ludo(void) {
    habilitarColores();
    int ultimoResultado = 0;
    bool menuActivo = true;

    while (menuActivo) {
        mostrarMenu();
        int opcion = SALIR;
        bool pudoLeer = leerOpcion(1, SALIR, "Selecciona una opcion: ", &opcion);
        if (pudoLeer) {
            menuActivo = ejecutarOpcionMenu(opcion, &ultimoResultado);
        } else {
            menuActivo = false;
        }
    }
    return ultimoResultado;
}
