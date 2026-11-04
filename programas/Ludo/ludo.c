/* LUDO: adaptacion del juego de cuatro fichas por jugador del proyecto PSeInt.
   Usa posiciones relativas 1..57; 1..51 es el circuito comun y 52..57 la meta. */
enum {
    LUDO_JUGADORES_MAX = 4,
    LUDO_FICHAS = 4,
    LUDO_LADO = 15,
    LUDO_CIRCUITO = 52,
    LUDO_ULTIMA_CASILLA_COMUN = 51,
    LUDO_META = 57,
    LUDO_DADO_MAX = 6,
    LUDO_CPU = 1,
    LUDO_AMIGOS = 2,
    LUDO_SALIR = 3,
    LUDO_FOTOGRAMAS_DADO = 8,
    LUDO_DADO_RAPIDO_MS = 50,
    LUDO_DADO_MEDIO_MS = 150,
    LUDO_DADO_LENTO_MS = 300,
    LUDO_RESULTADO_DADO_MS = 1000,
    LUDO_MOVIMIENTO_HUMANO_MS = 650,
    LUDO_MOVIMIENTO_CPU_MS = 550,
    LUDO_SALIDA_DE_CASA_MS = 1200,
    LUDO_PAUSA_CAPTURA_MS = 1700,
    LUDO_PAUSA_CPU_INICIO_MS = 1000,
    LUDO_PAUSA_CPU_DECISION_MS = 1000,
    LUDO_PAUSA_CPU_FIN_MS = 2000
};

typedef struct {
    int posicion;                 /* 0 = casa, 1..56 = camino, 57 = meta */
    bool enJuego;
} FichaTripleLudo;

typedef struct {
    FichaTripleLudo fichas[LUDO_FICHAS];
    bool computadora;
} JugadorTripleLudo;

typedef struct {
    JugadorTripleLudo jugadores[LUDO_JUGADORES_MAX];
    int cantidadJugadores;
    int ganador;
    bool activa;
} PartidaTripleLudo;

typedef struct { int fila; int columna; } CasillaTripleLudo;

/* Un camino unico de 52 casillas. Posiciones compartidas por todos los jugadores. */
/* El camino se define dentro de una funcion: no se usan variables globales. */
CasillaTripleLudo coordenadaCaminoTripleLudo(int posicionGlobal) {
    const CasillaTripleLudo camino[LUDO_CIRCUITO] = {
    {13,6},{12,6},{11,6},{10,6},{9,6},{8,5},{8,4},{8,3},{8,2},{8,1},{8,0},{7,0},{6,0},
    {6,1},{6,2},{6,3},{6,4},{6,5},{5,6},{4,6},{3,6},{2,6},{1,6},{0,6},{0,7},{0,8},
    {1,8},{2,8},{3,8},{4,8},{5,8},{6,9},{6,10},{6,11},{6,12},{6,13},{6,14},{7,14},
    {8,14},{8,13},{8,12},{8,11},{8,10},{8,9},{9,8},{10,8},{11,8},{12,8},{13,8},{14,8},{14,7},{14,6}
};
    return camino[posicionGlobal - 1];
}

bool esCasillaSeguraLudo(int posicionGlobal) {
    const int seguras[] = {1, 9, 14, 22, 27, 35, 40, 48};
    bool segura = false;
    for (int i = 0; i < (int)(sizeof seguras / sizeof seguras[0]); i++) {
        if (seguras[i] == posicionGlobal) segura = true;
    }
    return segura;
}

int posicionGlobalLudo(int jugador, int posicionRelativa) {
    /* Se almacenan jugadores en base 0 (PSeInt los numeraba desde 1). */
    return ((jugador * 13 + posicionRelativa - 1) % LUDO_CIRCUITO) + 1;
}

CasillaTripleLudo coordenadaFichaLudo(int jugador, int posicion) {
    CasillaTripleLudo coordenada = {-1, -1};
    if (posicion >= 1 && posicion <= LUDO_ULTIMA_CASILLA_COMUN) {
        coordenada = coordenadaCaminoTripleLudo(posicionGlobalLudo(jugador, posicion));
    } else if (posicion >= 52 && posicion <= LUDO_META) {
        int paso = posicion - 51;
        switch (jugador) {
            case 0: coordenada = (CasillaTripleLudo){14 - paso, 7}; break;
            case 1: coordenada = (CasillaTripleLudo){7, paso - 1}; break;
            case 2: coordenada = (CasillaTripleLudo){paso - 1, 7}; break;
            case 3: coordenada = (CasillaTripleLudo){7, 15 - paso}; break;
        }
    }
    return coordenada;
}

CasillaTripleLudo coordenadaCasaLudo(int jugador, int ficha) {
    const CasillaTripleLudo casas[4][4] = {
        {{12,1},{12,2},{13,1},{13,2}},
        {{1,1},{1,2},{2,1},{2,2}},
        {{1,12},{1,13},{2,12},{2,13}},
        {{12,12},{12,13},{13,12},{13,13}}
    };
    return casas[jugador][ficha];
}

void inicializarPartidaTripleLudo(PartidaTripleLudo *partida, int cantidad, bool vsComputadora) {
    memset(partida, 0, sizeof *partida);
    partida->cantidadJugadores = cantidad;
    partida->activa = true;
    partida->ganador = -1;
    if (vsComputadora) partida->jugadores[1].computadora = true;
}

void mostrarTableroTripleLudo(const PartidaTripleLudo *partida) {
    char tablero[LUDO_LADO][LUDO_LADO][5];
    for (int fila = 0; fila < LUDO_LADO; fila++) {
        for (int columna = 0; columna < LUDO_LADO; columna++) strcpy(tablero[fila][columna], " X  ");
    }
    for (int i = 0; i < LUDO_CIRCUITO; i++) {
        CasillaTripleLudo casilla = coordenadaCaminoTripleLudo(i + 1);
        strcpy(tablero[casilla.fila][casilla.columna], esCasillaSeguraLudo(i + 1) ? " S  " : " .  ");
    }
    for (int jugador = 0; jugador < LUDO_JUGADORES_MAX; jugador++) {
        for (int paso = 52; paso <= LUDO_META; paso++) {
            CasillaTripleLudo casilla = coordenadaFichaLudo(jugador, paso);
            strcpy(tablero[casilla.fila][casilla.columna], " .  ");
        }
        for (int ficha = 0; ficha < LUDO_FICHAS; ficha++) {
            CasillaTripleLudo casilla = coordenadaCasaLudo(jugador, ficha);
            strcpy(tablero[casilla.fila][casilla.columna], " .  ");
        }
    }
    for (int jugador = 0; jugador < partida->cantidadJugadores; jugador++) {
        for (int ficha = 0; ficha < LUDO_FICHAS; ficha++) {
            FichaTripleLudo actual = partida->jugadores[jugador].fichas[ficha];
            CasillaTripleLudo casilla = actual.enJuego
                ? coordenadaFichaLudo(jugador, actual.posicion)
                : coordenadaCasaLudo(jugador, ficha);
            if (casilla.fila >= 0 && casilla.fila < LUDO_LADO && casilla.columna >= 0 && casilla.columna < LUDO_LADO) {
                snprintf(tablero[casilla.fila][casilla.columna], sizeof tablero[casilla.fila][casilla.columna],
                         "%c.%c ", (char)('1' + jugador), (char)('1' + ficha));
            }
        }
    }
    printf("\n       TABLERO LUDO (S = casilla segura)\n");
    for (int fila = 0; fila < LUDO_LADO; fila++) {
        for (int columna = 0; columna < LUDO_LADO; columna++) printf("%s", tablero[fila][columna]);
        putchar('\n');
    }
    for (int jugador = 0; jugador < partida->cantidadJugadores; jugador++) {
        printf("Jugador %d:", jugador + 1);
        for (int ficha = 0; ficha < LUDO_FICHAS; ficha++) {
            FichaTripleLudo actual = partida->jugadores[jugador].fichas[ficha];
            if (!actual.enJuego) printf(" F%d=casa", ficha + 1);
            else if (actual.posicion == LUDO_META) printf(" F%d=META", ficha + 1);
            else printf(" F%d=%d", ficha + 1, actual.posicion);
        }
        putchar('\n');
    }
}

void dibujarDadoTripleLudo(int valor) {
    const char *caras[6][3] = {
        {"         ","    *    ","         "},
        {"  *      ","         ","      *  "},
        {"  *      ","    *    ","      *  "},
        {"  *   *  ","         ","  *   *  "},
        {"  *   *  ","    *    ","  *   *  "},
        {"  *   *  ","  *   *  ","  *   *  "}
    };
    printf("  +---------+\n  |%s|\n  |%s|\n  |%s|\n  +---------+\n",
           caras[valor - 1][0], caras[valor - 1][1], caras[valor - 1][2]);
}

/* Muestra un estado completo. Cada fotograma se dibuja sobre el mismo tablero. */
void mostrarEscenaTripleLudo(const PartidaTripleLudo *partida, const char *mensaje) {
    limpiarPantalla();
    printf("\n%s\n", mensaje);
    mostrarTableroTripleLudo(partida);
    fflush(stdout);
}

/* La cara final ya esta elegida: las otras caras solo simulan el giro. */
void animarDadoTripleLudo(const PartidaTripleLudo *partida, int jugador, int resultado) {
    char mensaje[100];
    int caraAnterior = 0;
    for (int fotograma = 1; fotograma <= LUDO_FOTOGRAMAS_DADO; fotograma++) {
        int cara = rand() % LUDO_DADO_MAX + 1;
        if (cara == caraAnterior) cara = cara % LUDO_DADO_MAX + 1;
        caraAnterior = cara;
        snprintf(mensaje, sizeof mensaje, "Jugador %d - el dado esta girando...", jugador + 1);
        mostrarEscenaTripleLudo(partida, mensaje);
        dibujarDadoTripleLudo(cara);
        fflush(stdout);
        int demora = LUDO_DADO_LENTO_MS;
        if (fotograma <= 4) demora = LUDO_DADO_RAPIDO_MS;
        else if (fotograma <= 6) demora = LUDO_DADO_MEDIO_MS;
        esperarMilisegundos(demora);
    }
    snprintf(mensaje, sizeof mensaje, "RESULTADO: Jugador %d saco un %d", jugador + 1, resultado);
    mostrarEscenaTripleLudo(partida, mensaje);
    dibujarDadoTripleLudo(resultado);
    esperarMilisegundos(LUDO_RESULTADO_DADO_MS);
}

int tirarDadoTripleLudo(const PartidaTripleLudo *partida, int jugador, bool computadora) {
    if (!computadora) {
        char entrada[20];
        if (!leerLinea("Presiona ENTER para lanzar el dado (0=abandonar partida): ",
                       entrada, sizeof entrada)) return 0;
        if (strcmp(entrada, "0") == 0) return 0;
    }
    int resultado = rand() % LUDO_DADO_MAX + 1;
    animarDadoTripleLudo(partida, jugador, resultado);
    return resultado;
}

bool hayFichaEnCasaTripleLudo(const JugadorTripleLudo *jugador) {
    for (int ficha = 0; ficha < LUDO_FICHAS; ficha++) {
        if (!jugador->fichas[ficha].enJuego) return true;
    }
    return false;
}

bool puedeMoverTripleLudo(const JugadorTripleLudo *jugador) {
    for (int ficha = 0; ficha < LUDO_FICHAS; ficha++) {
        if (jugador->fichas[ficha].enJuego && jugador->fichas[ficha].posicion < LUDO_META) return true;
    }
    return false;
}

/* Se verifican capturas solo al terminar el movimiento, no en el camino. */
int capturarFichasTripleLudo(PartidaTripleLudo *partida, int jugador, int posicion) {
    int capturas = 0;
    if (posicion < 1 || posicion > LUDO_ULTIMA_CASILLA_COMUN) return capturas;
    int global = posicionGlobalLudo(jugador, posicion);
    if (esCasillaSeguraLudo(global)) return capturas;

    for (int rival = 0; rival < partida->cantidadJugadores; rival++) {
        if (rival != jugador) {
            for (int ficha = 0; ficha < LUDO_FICHAS; ficha++) {
                FichaTripleLudo *contraria = &partida->jugadores[rival].fichas[ficha];
                if (contraria->enJuego && contraria->posicion >= 1 &&
                    contraria->posicion <= LUDO_ULTIMA_CASILLA_COMUN &&
                    posicionGlobalLudo(rival, contraria->posicion) == global) {
                    contraria->posicion = 0;
                    contraria->enJuego = false;
                    capturas++;
                }
            }
        }
    }
    return capturas;
}

void anunciarCapturasTripleLudo(const PartidaTripleLudo *partida, int jugador, int capturas) {
    if (capturas <= 0) return;
    char mensaje[100];
    snprintf(mensaje, sizeof mensaje,
             "CAPTURA! Jugador %d envio %d ficha(s) rival(es) a casa.",
             jugador + 1, capturas);
    mostrarEscenaTripleLudo(partida, mensaje);
    esperarMilisegundos(LUDO_PAUSA_CAPTURA_MS);
}

void sacarFichaTripleLudo(PartidaTripleLudo *partida, int jugador) {
    for (int ficha = 0; ficha < LUDO_FICHAS; ficha++) {
        FichaTripleLudo *actual = &partida->jugadores[jugador].fichas[ficha];
        if (!actual->enJuego) {
            actual->enJuego = true;
            actual->posicion = 1;
            char mensaje[120];
            snprintf(mensaje, sizeof mensaje, "Jugador %d saco la ficha %d de casa.",
                     jugador + 1, ficha + 1);
            mostrarEscenaTripleLudo(partida, mensaje);
            esperarMilisegundos(LUDO_SALIDA_DE_CASA_MS);

            int capturas = capturarFichasTripleLudo(partida, jugador, 1);
            anunciarCapturasTripleLudo(partida, jugador, capturas);
            return;
        }
    }
}

/* Cambia la posicion una casilla por vez y redibuja el tablero en cada paso. */
void moverFichaTripleLudo(PartidaTripleLudo *partida, int jugador, int ficha, int dado) {
    FichaTripleLudo *actual = &partida->jugadores[jugador].fichas[ficha];
    int destino = actual->posicion + dado;
    if (destino > LUDO_META) destino = LUDO_META;
    int pasos = destino - actual->posicion;
    bool computadora = partida->jugadores[jugador].computadora;

    for (int paso = 1; paso <= pasos; paso++) {
        actual->posicion++;
        char mensaje[160];
        snprintf(mensaje, sizeof mensaje,
                 "%s: Jugador %d mueve ficha %d - paso %d/%d (casilla %d)",
                 computadora ? "CPU" : "MOVIMIENTO", jugador + 1, ficha + 1,
                 paso, pasos, actual->posicion);
        mostrarEscenaTripleLudo(partida, mensaje);
        esperarMilisegundos(computadora ? LUDO_MOVIMIENTO_CPU_MS :
                                         LUDO_MOVIMIENTO_HUMANO_MS);
    }

    int capturas = capturarFichasTripleLudo(partida, jugador, actual->posicion);
    anunciarCapturasTripleLudo(partida, jugador, capturas);
}

int elegirFichaTripleLudo(const PartidaTripleLudo *partida, int jugador, bool computadora) {
    const JugadorTripleLudo *actual = &partida->jugadores[jugador];
    if (computadora) {
        for (int ficha = 0; ficha < LUDO_FICHAS; ficha++) {
            if (actual->fichas[ficha].enJuego && actual->fichas[ficha].posicion < LUDO_META) return ficha;
        }
        return -1;
    }
    while (true) {
        int opcion = leerEnteroEntre(0, LUDO_FICHAS, "Elegir ficha para mover [1-4] (0=abandonar partida): ");
        if (opcion == 0) return -1;
        int indice = opcion - 1;
        if (actual->fichas[indice].enJuego && actual->fichas[indice].posicion < LUDO_META) return indice;
        printf("La ficha elegida esta en casa o ya llego a la meta.\n");
    }
}

bool cuatroFichasEnMetaTripleLudo(const JugadorTripleLudo *jugador) {
    for (int ficha = 0; ficha < LUDO_FICHAS; ficha++) {
        if (!jugador->fichas[ficha].enJuego || jugador->fichas[ficha].posicion != LUDO_META) return false;
    }
    return true;
}

void procesarTurnoTripleLudo(PartidaTripleLudo *partida, int jugador) {
    bool computadora = partida->jugadores[jugador].computadora;
    bool turnoExtra;
    do {
        char mensaje[150];
        snprintf(mensaje, sizeof mensaje, "========== TURNO JUGADOR %d %s ==========",
                 jugador + 1, computadora ? "(COMPUTADORA)" : "");
        mostrarEscenaTripleLudo(partida, mensaje);
        if (computadora) esperarMilisegundos(LUDO_PAUSA_CPU_INICIO_MS);

        int dado = tirarDadoTripleLudo(partida, jugador, computadora);
        if (dado == 0) {
            partida->activa = false;
            printf("Partida abandonada.\n");
            return;
        }

        bool hayEnCasa = hayFichaEnCasaTripleLudo(&partida->jugadores[jugador]);
        bool hayEnTablero = puedeMoverTripleLudo(&partida->jugadores[jugador]);
        turnoExtra = dado == LUDO_DADO_MAX;
        if (turnoExtra) printf("Saco un 6: tiene un turno extra.\n");

        bool sacar = turnoExtra && hayEnCasa;
        if (sacar && hayEnTablero && !computadora) {
            sacar = leerEnteroEntre(1, 2, "1. Sacar ficha  2. Mover ficha: ") == 1;
        }

        if (sacar) {
            if (computadora) {
                mostrarEscenaTripleLudo(partida, "La PC eligio sacar una ficha de su casa.");
                esperarMilisegundos(LUDO_PAUSA_CPU_DECISION_MS);
            }
            sacarFichaTripleLudo(partida, jugador);
        } else if (hayEnTablero) {
            int ficha = elegirFichaTripleLudo(partida, jugador, computadora);
            if (ficha >= 0) {
                if (computadora) {
                    snprintf(mensaje, sizeof mensaje,
                             "La PC eligio mover su ficha %d con el dado %d.", ficha + 1, dado);
                    mostrarEscenaTripleLudo(partida, mensaje);
                    esperarMilisegundos(LUDO_PAUSA_CPU_DECISION_MS);
                }
                moverFichaTripleLudo(partida, jugador, ficha, dado);
            } else {
                partida->activa = false;
                turnoExtra = false;
                printf("Partida abandonada.\n");
            }
        } else {
            printf("No hay fichas que puedan moverse en este turno.\n");
            turnoExtra = false;
        }

        if (partida->activa && cuatroFichasEnMetaTripleLudo(&partida->jugadores[jugador])) {
            partida->ganador = jugador;
            partida->activa = false;
            snprintf(mensaje, sizeof mensaje,
                     "GANADOR: Jugador %d llevo sus cuatro fichas a la meta!", jugador + 1);
            mostrarEscenaTripleLudo(partida, mensaje);
        }

        if (partida->activa) {
            if (computadora) {
                if (turnoExtra) printf("La PC volvera a tirar el dado por haber sacado un 6.\n");
                else printf("La PC termino su turno. Ahora juega el siguiente jugador.\n");
                esperarMilisegundos(LUDO_PAUSA_CPU_FIN_MS);
            } else if (turnoExtra) {
                esperarEnter("Sacaste un 6. Presiona ENTER para tu turno extra...");
            } else {
                esperarEnter("Presiona ENTER para terminar tu turno...");
            }
        }
    } while (turnoExtra && partida->activa);
}

int jugarPartidaTripleLudo(int cantidad, bool vsPc) {
    PartidaTripleLudo partida;
    inicializarPartidaTripleLudo(&partida, cantidad, vsPc);
    while (partida.activa) {
        for (int jugador = 0; jugador < partida.cantidadJugadores; jugador++) {
            if (partida.activa) procesarTurnoTripleLudo(&partida, jugador);
        }
    }
    if (partida.ganador >= 0) esperarEnter("Presiona ENTER para volver al menu de Ludo...");
    if (partida.ganador < 0) return 0;
    return partida.ganador == 0 ? 1 : -1;
}

int ludo(void) {
    int modo = 0;
    int ultimoResultado = 0;
    while (modo != LUDO_SALIR) {
        limpiarPantalla();
        printf("\n=================== LUDO ===================\n");
        printf("1. Jugar contra la PC\n2. Jugar con amigos\n3. Volver al menu anterior\n");
        modo = leerEnteroEntre(LUDO_CPU, LUDO_SALIR, "Selecciona una opcion: ");
        if (modo == LUDO_CPU) {
            ultimoResultado = jugarPartidaTripleLudo(2, true);
        } else if (modo == LUDO_AMIGOS) {
            int jugadores = leerEnteroEntre(2, 5, "Cantidad de jugadores (2-4; 5=volver): ");
            if (jugadores != 5) {
                ultimoResultado = jugarPartidaTripleLudo(jugadores, false);
            }
        }
    }
    return ultimoResultado;
}
