#define CANT_CARTAS_MAZO 40
#define CANT_CARTAS_JUGADOR 3
#define CANT_MANOS 3

typedef struct {
    int valorEnvido;
    int jerarquiaTruco;
} ValoresCarta;

typedef struct {
    int numero;
    char palo[15];
    ValoresCarta valores;
    bool disponible;
} Carta;

typedef struct {
    int numeroMano;
    int resultadoJugador;
    int resultadoCpu;
} ResultadoMano;

int seleccionarDificultadCPU(void);
void mostrarFinPartida(int pJugador, int pCPU, int pMaximos);
void dibujarCartasOcultasCPU(int cantidad);
void obtenerAbrevPalo(const char palo[], char abrev[]);
void dibujarMesaHistorial(const int hJNros[], char hJPalos[][15], const int hCNros[], char hCPalos[][15]);
void dibujarCartasJugador(const int nros[], char palos[][15], const bool disp[]);
int mostrarMenuPrincipalTruco(void);
void mostrarReglas(void);
void comoSeJuega(void);
void valoresDelTruco(void);
void valoresDelEnvido(void);
void empates(void);
void informacionDelTruco(void);
void configurarPartida(int *puntajeMaximo);
void crearMazo(Carta mazo[]);
void barajarMazo(Carta mazo[]);
void intercambiarCartas(Carta *primeraCarta, Carta *segundaCarta);
void repartirCartas(Carta mazo[], Carta jCartas[], Carta cCartas[], int *indiceMazo);
int calcularEnvido(const Carta cartas[]);
bool decidirEnvidoCPU(int envidoC, int dificultadCPU);
bool decidirTrucoCPU(int fuerzaC, int dificultadCPU);
int elegirCartaCPU(const Carta cCartas[], const bool dispC[], int jerJugador, int responde, int dificultadCPU);
int fuerzaManoCPU(const Carta cCartas[]);
void jugarRonda(Carta jCartas[], Carta cCartas[], int *puntosJ, int *puntosC, int pMaximos, int dificultadCPU, int manoInicial);
int determinarGanadorRonda(const int resultadoMano[], int ganadasJ, int ganadasC, int manoInicial);
void mostrarMatrizResultados(const int resultados[CANT_MANOS][2]);
void ordenarCartasPorJerarquia(Carta cartas[], int cantidadCartas);
int buscarCartaPorNumero(const Carta cartas[], int cantidadCartas, int numeroBuscado);
int buscarCartaPorPaloYNumero(const Carta cartas[], int cantidadCartas, const char paloBuscado[], int numeroBuscado);
void renderizarMesa(int puntosJ, int puntosC, int pMaximos, const int hJNros[], char hJPalos[][15], const int hCNros[], char hCPalos[][15], const Carta jCartas[], const bool dispJ[], int mano, int ganadasJ, int ganadasC);
void gestionarEnvido(int iniciador, int tipoInicial, int envidoJ, int envidoC, int *puntosJ, int *puntosC, int pMaximos, int *envidoCantado, int dificultadCPU, int manoInicial, const int hJNros[], char hJPalos[][15], const int hCNros[], char hCPalos[][15], const Carta jCartas[], const bool dispJ[], int mano, int ganadasJ, int ganadasC);
void gestionarTruco(int iniciador, int *estTruco, int *ptsTruco, int *ultimoCantoTruco, int *puntosJ, int *puntosC, int pMaximos, bool *alMazo, int dificultadCPU, const Carta cCartas[], const int hJNros[], char hJPalos[][15], const int hCNros[], char hCPalos[][15], const Carta jCartas[], const bool dispJ[], int mano, int ganadasJ, int ganadasC);
void procesarTurnoJugador(int *jCarta, bool dispJ[], int hJNros[], char hJPalos[][15], int *turno, bool *primerCarta, int *accion, int *estTruco, int *ptsTruco, int *ultimoCantoTruco, int *puntosJ, int *puntosC, bool *alMazo, int pMaximos, int envidoJ, int envidoC, int *envidoCantado, int dificultadCPU, int manoInicial, const int hCNros[], char hCPalos[][15], const Carta cCartas[], const Carta jCartas[], int mano, int ganadasJ, int ganadasC);
void procesarTurnoCPU(int *cCarta, bool dispC[], int hCNros[], char hCPalos[][15], int *turno, bool *primerCarta, bool *alMazo, int jCarta, int envidoC, int envidoJ, int *envidoCantado, int dificultadCPU, int manoInicial, int *puntosJ, int *puntosC, int *estTruco, int *ptsTruco, int *ultimoCantoTruco, const Carta cCartas[], const Carta jCartas[], int mano, int pMaximos, const int hJNros[], char hJPalos[][15], const bool dispJ[], int ganadasJ, int ganadasC);
void procesarResultadoRonda(int mano, int jCarta, int cCarta, const Carta jCartas[], const Carta cCartas[], int *ganador, int *ganadasJ, int *ganadasC, int resultadoMano[], int resultadosManosMatriz[CANT_MANOS][2], int *jugadorLider);

int truco(void) {
    int pMaximos = 15;
    int pJugador = 0;
    int pCPU = 0;
    int mazoIdx = 0;
    int dificultadCPU = 1;
    int manoInicial = 0;
    int opcionMenu = 0;
    bool salirTruco = false;

    srand((unsigned int)time(NULL));

    do {
        pJugador = 0;
        pCPU = 0;
        manoInicial = rand() % 2;
        opcionMenu = mostrarMenuPrincipalTruco();

        if (opcionMenu == 1) {
            configurarPartida(&pMaximos);
            dificultadCPU = seleccionarDificultadCPU();

            while (pJugador < pMaximos && pCPU < pMaximos) {
                Carta mazo[CANT_CARTAS_MAZO];
                Carta jCartas[CANT_CARTAS_JUGADOR];
                Carta cCartas[CANT_CARTAS_JUGADOR];

                crearMazo(mazo);
                barajarMazo(mazo);
                mazoIdx = 0;
                repartirCartas(mazo, jCartas, cCartas, &mazoIdx);
                jugarRonda(jCartas, cCartas, &pJugador, &pCPU, pMaximos, dificultadCPU, manoInicial);

                if (pJugador >= pMaximos || pCPU >= pMaximos) {
                    break;
                }

                manoInicial = 1 - manoInicial;
            }
            mostrarFinPartida(pJugador, pCPU, pMaximos);
        } else if (opcionMenu == 2) {
            mostrarReglas();
        } else if (opcionMenu == 3) {
            printf("Saliendo del Truco...\n");
            pausarSeguro();
            limpiarPantalla();
            salirTruco = true;
        }
    } while (!salirTruco);

    return 0;
}

int seleccionarDificultadCPU(void) {
    int dificultad = 1;
    limpiarPantalla();
    printf("========== DIFICULTAD DE LA CPU ==========\n");
    printf("1. Principiante: juega casi al azar y canta poco\n");
    printf("2. Facil: prioriza cartas razonables\n");
    printf("3. Dificil: calcula respuestas y conserva cartas fuertes\n");
    printf("4. Experta: responde con la carta minima que gana y evalua apuestas\n");
    dificultad = leerEnteroEntre(1, 4, "Ingrese opcion: ");
    limpiarPantalla();
    return dificultad;
}

void mostrarFinPartida(int pJugador, int pCPU, int pMaximos) {
    limpiarPantalla();
    printf("========================================================================================\n");
    printf("|                                  FIN DE LA PARTIDA                                   |\n");
    printf("========================================================================================\n\n");
    if (pJugador >= pMaximos) {
        printf("               ¡FELICIDADES! HAS GANADO LA PARTIDA DE TRUCO\n\n");
    } else {
        printf("               LA CPU HA GANADO LA PARTIDA. ¡MEJOR SUERTE LA PROXIMA!\n\n");
    }
    printf("               Puntaje Final -> Jugador: %d  |  CPU: %d (a %d puntos)\n\n", pJugador, pCPU, pMaximos);
    printf("========================================================================================\n");
    pausarSeguro();
}

void dibujarCartasOcultasCPU(int cantidad) {
    if (cantidad <= 0) {
        printf("        (Sin cartas en mano)\n");
        return;
    }
    int i;
    for (i = 0; i < cantidad; i++) printf("        +-------+   ");
    printf("\n");
    for (i = 0; i < cantidad; i++) printf("        | ##### |   ");
    printf("\n");
    for (i = 0; i < cantidad; i++) printf("        | #CPU# |   ");
    printf("\n");
    for (i = 0; i < cantidad; i++) printf("        | ##### |   ");
    printf("\n");
    for (i = 0; i < cantidad; i++) printf("        +-------+   ");
    printf("\n");
}

void obtenerAbrevPalo(const char palo[], char abrev[]) {
    strcpy(abrev, "   ");
    if (strcmp(palo, "Espada") == 0) strcpy(abrev, "Esp");
    else if (strcmp(palo, "Basto") == 0) strcpy(abrev, "Bas");
    else if (strcmp(palo, "Oro") == 0) strcpy(abrev, "Oro");
    else if (strcmp(palo, "Copa") == 0) strcpy(abrev, "Cop");
}

void dibujarMesaHistorial(const int hJNros[], char hJPalos[][15], const int hCNros[], char hCPalos[][15]) {
    int b;
    char abrevJ[10], abrevC[10];
    printf("\n");
    printf("|--------------------------------- CARTAS EN LA MESA ----------------------------------|\n\n");
    printf("           --- MANO 1 ---            --- MANO 2 ---            --- MANO 3 ---\n");
    printf("          [Vos]     [CPU]           [Vos]     [CPU]           [Vos]     [CPU]\n");

    printf("        ");
    for (b = 0; b < 3; b++) {
        if (hJNros[b] != 0) printf("+-------+ "); else printf("          ");
        if (hCNros[b] != 0) printf("+-------+");   else printf("         ");
        if (b < 2) printf("      ");
    }
    printf("\n");

    printf("        ");
    for (b = 0; b < 3; b++) {
        if (hJNros[b] != 0) {
            if (hJNros[b] < 10) printf("| %d     | ", hJNros[b]);
            else                printf("| %d    | ", hJNros[b]);
        } else {
            printf("          ");
        }
        if (hCNros[b] != 0) {
            if (hCNros[b] < 10) printf("| %d     |", hCNros[b]);
            else                printf("| %d    |", hCNros[b]);
        } else {
            printf("         ");
        }
        if (b < 2) printf("      ");
    }
    printf("\n");

    printf("        ");
    for (b = 0; b < 3; b++) {
        if (hJNros[b] != 0) {
            obtenerAbrevPalo(hJPalos[b], abrevJ);
            printf("|  %s  | ", abrevJ);
        } else {
            printf("          ");
        }
        if (hCNros[b] != 0) {
            obtenerAbrevPalo(hCPalos[b], abrevC);
            printf("|  %s  |", abrevC);
        } else {
            printf("         ");
        }
        if (b < 2) printf("      ");
    }
    printf("\n");

    printf("        ");
    for (b = 0; b < 3; b++) {
        if (hJNros[b] != 0) {
            if (hJNros[b] < 10) printf("|     %d | ", hJNros[b]);
            else                printf("|    %d | ", hJNros[b]);
        } else {
            printf("          ");
        }
        if (hCNros[b] != 0) {
            if (hCNros[b] < 10) printf("|     %d |", hCNros[b]);
            else                printf("|    %d |", hCNros[b]);
        } else {
            printf("         ");
        }
        if (b < 2) printf("      ");
    }
    printf("\n");

    printf("        ");
    for (b = 0; b < 3; b++) {
        if (hJNros[b] != 0) printf("+-------+ "); else printf("          ");
        if (hCNros[b] != 0) printf("+-------+");   else printf("         ");
        if (b < 2) printf("      ");
    }
    printf("\n----------------------------------------------------------------------------------------\n");
}

void dibujarCartasJugador(const int nros[], char palos[][15], const bool disp[]) {
    int i;
    char abrev[10];

    for (i = 0; i < 3; i++) {
        if (disp[i]) printf("          [ %d ]     ", i + 1);
        else         printf("                    ");
    }
    printf("\n");

    for (i = 0; i < 3; i++) {
        if (disp[i]) printf("        +-------+   ");
        else         printf("                    ");
    }
    printf("\n");

    for (i = 0; i < 3; i++) {
        if (disp[i]) {
            if (nros[i] < 10) printf("        | %d     |   ", nros[i]);
            else              printf("        | %d    |   ", nros[i]);
        } else {
            printf("                    ");
        }
    }
    printf("\n");

    for (i = 0; i < 3; i++) {
        if (disp[i]) {
            obtenerAbrevPalo(palos[i], abrev);
            printf("        |  %s  |   ", abrev);
        } else {
            printf("                    ");
        }
    }
    printf("\n");

    for (i = 0; i < 3; i++) {
        if (disp[i]) {
            if (nros[i] < 10) printf("        |     %d |   ", nros[i]);
            else              printf("        |    %d |   ", nros[i]);
        } else {
            printf("                    ");
        }
    }
    printf("\n");

    for (i = 0; i < 3; i++) {
        if (disp[i]) printf("        +-------+   ");
        else         printf("                    ");
    }
    printf("\n");
}

int mostrarMenuPrincipalTruco(void) {
    limpiarPantalla();
    printf("   -- \n");
    printf("  |  |  \n");
    printf(" _| $$_      ______   __    __   _______    ______\n");
    printf("|   $$ |    /      | |  |  |  | /       |  /      | \n");
    printf(" |$$$$$$   |  $$$$$$ | $$  | $$|  $$$$$$$ | $$$$$$  \n");
    printf("  | $$ __  | $$   |$$| $$  | $$| $$       |$$  | $$\n");
    printf("  | $$|  | | $$      | $$__/ $$| $$_____  |$$__/ $$\n");
    printf("   |$$  $$ | $$       |$$    $$ |$$     |  $$    $$\n");
    printf("    |$$$$   |$$        |$$$$$$   |$$$$$$$   $$$$$$  \n");
    printf("                        1                         \n");
    printf("                  -------------                   \n");
    printf("                 |    JUGAR    |                  \n");
    printf("                  -------------                   \n\n");
    printf("                        2                         \n");
    printf("                  -------------                   \n");
    printf("                 |    REGLAS   |                  \n");
    printf("                  -------------                   \n\n");
    printf("                        3                         \n");
    printf("                  -------------                   \n");
    printf("                 |    SALIR    |                  \n");
    printf("                  -------------                   \n\n");
    printf(" SELECCIONA UNA DE LAS OPCIONES PARA EMPEZAR A JUGAR O VOLVER AL MENU INICIAL.(1-3)\n\n");
    return leerEnteroEntre(1, 3, "Seleccione opcion: ");
}

void mostrarReglas(void) {
    int opcionReglas = 0;
    do {
        limpiarPantalla();
        printf("=== REGLAS DEL TRUCO ===\n");
        printf("1. Generales y Valores\n");
        printf("2. Volver\n");
        opcionReglas = leerEnteroEntre(1, 2, "Seleccione una opcion: ");
        if (opcionReglas == 1) {
            informacionDelTruco();
            pausarSeguro();
        }
    } while (opcionReglas != 2);
}

void comoSeJuega(void) {
    printf("El objetivo es llegar a la cantidad de puntos acordada (15 o 30 puntos)\n");
    printf("Se juega con 40 cartas espanolas\n");
    printf("Se usan los numeros: 1, 2, 3, 4, 5, 6, 7, 10, 11 y 12.\nNo se utilizan los 8 ni 9.\n");
    printf("Los palos son: Espada, Basto, Oro y Copa.\n\n");
    printf("Cada jugador recibe 3 cartas y se juegan hasta 3 manos.\n");
    printf("Los puntos se consiguen principalmente mediante estas manos,\n");
    printf("los jugadores pueden cantar envido o truco para sumar mas puntos mediante las manos\n\n");
}

void valoresDelTruco(void) {
    printf("El valor de las cartas para determinar quien gana una mano\n");
    printf("En el juego las cartas siguen el siguiente valor.\n");
    printf("1 de Espada > 1 de Basto > 7 de Espada > 7 de Oro > 3 > 2 > 1 de Copa/Oro \n");
    printf("> 12 > 11 > 10 > 7 de Basto/Copa > 6 > 5 > 4.\n\n");
    printf("El jugador o equipo que gana 2 de las 3 manos gana el Truco.\n\n");
    printf("VALORES DEL TRUCO\n| Apuesta     | Si se gana | \n| ----------- | ---------: |\n");
    printf("| Sin Truco   |    1 punto |\n| Truco       |   2 puntos |\n| Retruco     |   3 puntos |\n| Vale Cuatro |   4 puntos |\n\n");
}

void valoresDelEnvido(void) {
    printf("ENVIDO\n\nSe puede cantar antes de jugar la primera carta\n");
    printf("Y si es en equipo lo pueden cantar unicamente los ultimos dos jugadores de la ronda\n\n");
    printf("Se comparan los puntos de envido de las cartas.\n");
    printf("Dos cartas del mismo palo: se suman sus valores + 20.\n");
    printf("Si no hay dos del mismo palo, se toma la carta de mayor valor para el envido.\n");
    printf("Las cartas 10, 11 y 12 valen 0 para el envido.\n");
    printf("| Apuesta      |Si se quiere|\n| -----------  | ---------: |\n");
    printf("| Sin envido   |   0 puntos |\n| Envido       |   2 puntos |\n| Real Envido  |   3 puntos |\n");
    printf("| Falta Envido | Los puntos |\n|              | faltantes  |\n|              | segun la   |\n|              | situacion  |\n|              | del partido|\n\n");
}

void empates(void) {
    printf("PARDAS O EMPATE\nSi dos jugadores tiran cartas del mismo valor, hay parda.\n");
    printf("Para un Truco de 3 manos:\nParda en la primera: la segunda mano decide si alguien gana.\n");
    printf("Parda en la segunda: gana quien haya ganado la primera.\n");
    printf("Parda en la tercera: gana quien haya ganado la primera o segunda.\n");
    printf("Si las tres manos terminan empatadas, gana el jugador que tenia ventaja por las reglas de las pardas.\n\n");
    printf("IRSE AL MAZO\nEn cualquier momento de una apuesta, el jugador puede irse al mazo.\n");
    printf("El rival gana automaticamente los puntos que correspondan a la apuesta que estaba vigente.\n");
}

void informacionDelTruco(void) {
    limpiarPantalla();
    printf("                               __  \n");
    printf("                              |  | \n");
    printf("  ______    ______    ______  | $$  ______    _______ \n");
    printf(" /      |  /      |  /      | | $$ |      |  /       | \n");
    printf("|  $$$$$$||  $$$$$$||  $$$$$$|| $$  |$$$$$$||  $$$$$$$\n");
    printf("| $$   |$$| $$    $$| $$  | $$| $$ /      $$ |$$    | \n");
    printf("| $$      | $$$$$$$$| $$__| $$| $$|  $$$$$$$ _|$$$$$$ \n");
    printf("| $$       |$$     | |$$    $$| $$ |$$    $$|       $$\n");
    printf(" |$$        |$$$$$$$ _|$$$$$$$ |$$  |$$$$$$$ |$$$$$$$ \n");
    printf("                    |  |__| $$ \n");
    printf("                     |$$    $$ \n");
    printf("                       |$$$$$$  \n\n\n");
    printf("========== COMO SE JUEGA ==========\n\n");
    comoSeJuega();
    printf("========== VALORES DEL TRUCO ==========\n\n");
    valoresDelTruco();
    printf("========== VALORES DEL ENVIDO ==========\n\n");
    valoresDelEnvido();
    printf("========== PARDAS Y OTRAS REGLAS ==========\n\n");
    empates();
}

void configurarPartida(int *puntajeMaximo) {
    char puntos[10];
    do {
        limpiarPantalla();
        printf("                                 __  \n");
        printf("                                |  |    \n");
        printf("    ______   __    __  _______ _| $$_    ______    _______         ______               __  __    __   ______    ______    ______ \n");
        printf("   /      | |  |  |  ||       |   $$ |  /      |  /       |       |      |             |  ||  |  |  | /      |  |      |  /      | \n");
        printf("  |  $$$$$$|| $$  | $$| $$$$$$$|$$$$$$ |  $$$$$$||  $$$$$$$        |$$$$$$|             |$$| $$  | $$|  $$$$$$|  |$$$$$$||  $$$$$$| \n");
        printf("  | $$  | $$| $$  | $$| $$  | $$| $$ __| $$  | $$ |$$    |        /      $$            |  || $$  | $$| $$  | $$ /      $$| $$   |$$ \n");
        printf("  | $$__/ $$| $$__/ $$| $$  | $$| $$|  | $$__/ $$ _|$$$$$$|      |  $$$$$$$            | $$| $$__/ $$| $$__| $$|  $$$$$$$| $$ \n");
        printf("  | $$    $$ |$$    $$| $$  | $$ |$$ $$ |$$    $$|       $$       |$$    $$            | $$ |$$    $$ |$$    $$ |$$    $$| $$ \n");
        printf("  | $$$$$$$   |$$$$$$  |$$   |$$  |$$$$  |$$$$$$  |$$$$$$$         |$$$$$$$       __   | $$  |$$$$$$  _|$$$$$$$  |$$$$$$$ |$$ \n");
        printf("  | $$                                                                           |  |__/ $$          |  |__| $$ \n");
        printf("  | $$                                                                            |$$    $$           |$$    $$ \n");
        printf("   |$$                                                                             |$$$$$$             |$$$$$$  \n");
        printf("                        A                         \n                  -------------                   \n                 |      15     |                  \n                  -------------                   \n \n");
        printf("                        B                         \n                  -------------                   \n                 |      30     |                  \n                  -------------                   \n \n");
        printf(" SELECCIONA LOS PUNTOS A LOS QUE SE JUGARA LA PARTIDA. (A para jugar a 15 puntos o B para jugar a 30 puntos)\n \n");

        if (!leerLinea("Opcion: ", puntos, sizeof(puntos))) {
            continue;
        }

        if (puntos[0] == 'a' || puntos[0] == 'A') {
            *puntajeMaximo = 15;
            break;
        } else if (puntos[0] == 'b' || puntos[0] == 'B') {
            *puntajeMaximo = 30;
            break;
        } else {
            printf("Opcion Invalida.\n");
            pausarSeguro();
        }
    } while (true);
}

void crearMazo(Carta mazo[]) {
    const char palosDeLasCartas[4][15] = {"Espada", "Basto", "Oro", "Copa"};
    int cont = 0;
    int p, n, num, valorEnvido, jerarquia;

    for (p = 0; p < 4; p++) {
        for (n = 1; n <= 10; n++) {
            if (n <= 7) num = n;
            else num = n + 2;

            valorEnvido = (num >= 10) ? 0 : num;
            jerarquia = 0;

            if (num == 4) jerarquia = 1;
            else if (num == 5) jerarquia = 2;
            else if (num == 6) jerarquia = 3;
            else if (num == 7 && (strcmp(palosDeLasCartas[p], "Copa") == 0 || strcmp(palosDeLasCartas[p], "Basto") == 0)) jerarquia = 4;
            else if (num == 10) jerarquia = 5;
            else if (num == 11) jerarquia = 6;
            else if (num == 12) jerarquia = 7;
            else if (num == 1 && (strcmp(palosDeLasCartas[p], "Copa") == 0 || strcmp(palosDeLasCartas[p], "Oro") == 0)) jerarquia = 8;
            else if (num == 2) jerarquia = 9;
            else if (num == 3) jerarquia = 10;
            else if (num == 7 && strcmp(palosDeLasCartas[p], "Oro") == 0) jerarquia = 11;
            else if (num == 7 && strcmp(palosDeLasCartas[p], "Espada") == 0) jerarquia = 12;
            else if (num == 1 && strcmp(palosDeLasCartas[p], "Basto") == 0) jerarquia = 13;
            else if (num == 1 && strcmp(palosDeLasCartas[p], "Espada") == 0) jerarquia = 14;

            mazo[cont].numero = num;
            strcpy(mazo[cont].palo, palosDeLasCartas[p]);
            mazo[cont].valores.jerarquiaTruco = jerarquia;
            mazo[cont].valores.valorEnvido = valorEnvido;
            mazo[cont].disponible = true;
            cont++;
        }
    }
}

void barajarMazo(Carta mazo[]) {
    int i, j;
    for (i = CANT_CARTAS_MAZO - 1; i > 0; i--) {
        j = rand() % (i + 1);
        intercambiarCartas(&mazo[i], &mazo[j]);
    }
}

void intercambiarCartas(Carta *primeraCarta, Carta *segundaCarta) {
    Carta auxiliar = *primeraCarta;
    *primeraCarta = *segundaCarta;
    *segundaCarta = auxiliar;
}

void repartirCartas(Carta mazo[], Carta jCartas[], Carta cCartas[], int *indiceMazo) {
    int i;
    for (i = 0; i < CANT_CARTAS_JUGADOR; i++) {
        jCartas[i] = mazo[*indiceMazo];
        (*indiceMazo)++;
        cCartas[i] = mazo[*indiceMazo];
        (*indiceMazo)++;
    }
}

int calcularEnvido(const Carta cartas[]) {
    int maxEnvido = 0;
    int tempEnv = 0;

    if (strcmp(cartas[0].palo, cartas[1].palo) == 0) {
        tempEnv = 20 + cartas[0].valores.valorEnvido + cartas[1].valores.valorEnvido;
        if (tempEnv > maxEnvido) maxEnvido = tempEnv;
    }
    if (strcmp(cartas[0].palo, cartas[2].palo) == 0) {
        tempEnv = 20 + cartas[0].valores.valorEnvido + cartas[2].valores.valorEnvido;
        if (tempEnv > maxEnvido) maxEnvido = tempEnv;
    }
    if (strcmp(cartas[1].palo, cartas[2].palo) == 0) {
        tempEnv = 20 + cartas[1].valores.valorEnvido + cartas[2].valores.valorEnvido;
        if (tempEnv > maxEnvido) maxEnvido = tempEnv;
    }

    if (maxEnvido == 0) {
        for (int i = 0; i < 3; i++) {
            if (cartas[i].valores.valorEnvido > maxEnvido) {
                maxEnvido = cartas[i].valores.valorEnvido;
            }
        }
    }
    return maxEnvido;
}

bool decidirEnvidoCPU(int envidoC, int dificultadCPU) {
    bool cantar = false;
    if (dificultadCPU == 1 && envidoC >= 30) cantar = true;
    else if (dificultadCPU == 2 && (envidoC >= 28 || (rand() % 100) < 5)) cantar = true;
    else if (dificultadCPU == 3 && (envidoC >= 26 || (rand() % 100) < (dificultadCPU * 5))) cantar = true;
    else if (dificultadCPU >= 4 && (envidoC >= 24 || (rand() % 100) < (dificultadCPU * 6))) cantar = true;
    return cantar;
}

bool decidirTrucoCPU(int fuerzaC, int dificultadCPU) {
    bool cantarT = false;
    if (dificultadCPU == 1 && fuerzaC >= 8) cantarT = true;
    else if (dificultadCPU == 2 && (fuerzaC >= 6 || (rand() % 100) < 5)) cantarT = true;
    else if (dificultadCPU == 3 && (fuerzaC >= 4 || (rand() % 100) < (dificultadCPU * 4))) cantarT = true;
    else if (dificultadCPU >= 4 && (fuerzaC >= 3 || (rand() % 100) < (dificultadCPU * 5))) cantarT = true;
    return cantarT;
}

int elegirCartaCPU(const Carta cCartas[], const bool dispC[], int jerJugador, int responde, int dificultadCPU) {
    int carta = -1;
    int menorDisponible = -1;
    int mejorGanadora = -1;
    int i, cantidad = 0, elegido;
    int opciones[3];

    for (i = 0; i < 3; i++) {
        if (dispC[i]) {
            if (menorDisponible == -1 || cCartas[i].valores.jerarquiaTruco < cCartas[menorDisponible].valores.jerarquiaTruco) {
                menorDisponible = i;
            }
            if (responde == 1 && cCartas[i].valores.jerarquiaTruco > jerJugador) {
                if (mejorGanadora == -1 || cCartas[i].valores.jerarquiaTruco < cCartas[mejorGanadora].valores.jerarquiaTruco) {
                    mejorGanadora = i;
                }
            }
        }
    }

    if (responde == 1 && dificultadCPU >= 3 && mejorGanadora != -1) {
        carta = mejorGanadora;
    } else {
        if (dificultadCPU == 1) {
            for (i = 0; i < 3; i++) {
                if (dispC[i]) {
                    opciones[cantidad] = i;
                    cantidad++;
                }
            }
            elegido = rand() % cantidad;
            carta = opciones[elegido];
        } else {
            if (responde == 1 && mejorGanadora != -1) carta = mejorGanadora;
            else if (menorDisponible != -1) carta = menorDisponible;
        }
    }
    return carta;
}

int fuerzaManoCPU(const Carta cCartas[]) {
    int i, fuerza = 0;
    for (i = 0; i < 3; i++) {
        if (cCartas[i].valores.jerarquiaTruco >= 10) fuerza += 3;
        else if (cCartas[i].valores.jerarquiaTruco >= 7) fuerza += 2;
        else if (cCartas[i].valores.jerarquiaTruco >= 4) fuerza += 1;
    }
    return fuerza;
}

void renderizarMesa(int puntosJ, int puntosC, int pMaximos, const int hJNros[], char hJPalos[][15], const int hCNros[], char hCPalos[][15], const Carta jCartas[], const bool dispJ[], int mano, int ganadasJ, int ganadasC) {
    limpiarPantalla();
    printf("========================================================================================\n");
    printf("|                                 TRUCO - MESA DE JUEGO                                |\n");
    printf("========================================================================================\n");
    printf("|  NOSOTROS: %2d / %-2d pts                    |  CPU: %2d / %-2d pts                         |\n", puntosJ, pMaximos, puntosC, pMaximos);
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Ronda actual: Mano %d de 3  |  Manos ganadas -> Vos: %d  |  CPU: %d\n", mano + 1, ganadasJ, ganadasC);
    printf("----------------------------------------------------------------------------------------\n");

    int cpuJugadas = (hCNros[0] != 0 ? 1 : 0) + (hCNros[1] != 0 ? 1 : 0) + (hCNros[2] != 0 ? 1 : 0);
    int cpuRestantes = 3 - cpuJugadas;
    printf("\n  [ CARTAS DE LA CPU: %d ]\n", cpuRestantes);
    dibujarCartasOcultasCPU(cpuRestantes);

    dibujarMesaHistorial(hJNros, hJPalos, hCNros, hCPalos);

    printf("\n  [ TUS CARTAS ]\n");
    int nrosJ[3] = {jCartas[0].numero, jCartas[1].numero, jCartas[2].numero};
    char palosJ[3][15];
    strcpy(palosJ[0], jCartas[0].palo);
    strcpy(palosJ[1], jCartas[1].palo);
    strcpy(palosJ[2], jCartas[2].palo);

    bool dispJInt[3] = {dispJ[0], dispJ[1], dispJ[2]};
    dibujarCartasJugador(nrosJ, palosJ, dispJInt);
    printf("========================================================================================\n");
}

void gestionarEnvido(int iniciador, int tipoInicial, int envidoJ, int envidoC, int *puntosJ, int *puntosC, int pMaximos, int *envidoCantado, int dificultadCPU, int manoInicial, const int hJNros[], char hJPalos[][15], const int hCNros[], char hCPalos[][15], const Carta jCartas[], const bool dispJ[], int mano, int ganadasJ, int ganadasC) {
    int falta = (*puntosJ > *puntosC) ? (pMaximos - *puntosJ) : (pMaximos - *puntosC);
    if (falta <= 0) falta = 1;

    int nivelEnvido = tipoInicial;
    int ptsAceptados = 2;
    int ptsRechazados = 1;

    if (tipoInicial == 2) {
        ptsAceptados = 3;
        ptsRechazados = 1;
    } else if (tipoInicial == 3) {
        ptsAceptados = falta;
        ptsRechazados = 1;
    }

    int quienResponde = 1 - iniciador;
    bool resuelto = false;

    while (!resuelto) {
        if (quienResponde == 0) {
            renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
            printf("\n========================================================================================\n");
            if (nivelEnvido == 1) printf("                      >>> ¡LA CPU HA CANTADO: ENVIDO! <<<\n");
            else if (nivelEnvido == 2) printf("                    >>> ¡LA CPU HA CANTADO: REAL ENVIDO! <<<\n");
            else printf("                   >>> ¡LA CPU HA CANTADO: FALTA ENVIDO! <<<\n");
            printf("========================================================================================\n");
            printf("  ¿Que deseas responder?\n");
            printf("  1. Quiero (Se define por %d puntos)\n", ptsAceptados);
            printf("  2. No quiero (La CPU suma %d punto%s)\n", ptsRechazados, ptsRechazados == 1 ? "" : "s");
            if (nivelEnvido == 1) {
                printf("  3. Cantar Real Envido (Subir a 5 puntos)\n");
                printf("  4. Cantar Falta Envido (Se juega por %d puntos)\n", falta);
            } else if (nivelEnvido == 2) {
                printf("  3. Cantar Falta Envido (Se juega por %d puntos)\n", falta);
            }

            int opcion = 0;
            if (nivelEnvido == 1) opcion = leerEnteroEntre(1, 4, "Seleccione opcion: ");
            else if (nivelEnvido == 2) opcion = leerEnteroEntre(1, 3, "Seleccione opcion: ");
            else opcion = leerEnteroEntre(1, 2, "Seleccione opcion: ");

            if (opcion == 1) {
                resuelto = true;
                bool ganaJ = (envidoJ > envidoC) || (envidoJ == envidoC && manoInicial == 0);
                if (ganaJ) *puntosJ += ptsAceptados;
                else *puntosC += ptsAceptados;

                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                     >>> ¡ACEPTASTE LA APUESTA! <<<\n");
                printf("========================================================================================\n");
                printf("  Tu Envido: %d  |  Envido de la CPU: %d\n", envidoJ, envidoC);
                if (envidoJ == envidoC) {
                    printf("  ¡Empate en envido! Gana quien es mano (%s).\n", (manoInicial == 0 ? "Vos" : "CPU"));
                }
                if (ganaJ) {
                    printf("  >>> ¡GANASTE EL ENVIDO! (+%d puntos) <<<\n", ptsAceptados);
                } else {
                    printf("  >>> LA CPU GANA EL ENVIDO. (+%d puntos) <<<\n", ptsAceptados);
                }
                printf("  Tanteador actual -> Vos: %d pts | CPU: %d pts (a %d puntos)\n", *puntosJ, *puntosC, pMaximos);
                printf("========================================================================================\n");
                pausarSeguro();
            } else if (opcion == 2) {
                resuelto = true;
                *puntosC += ptsRechazados;
                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                     >>> ¡DIJISTE: NO QUIERO! <<<\n");
                printf("========================================================================================\n");
                printf("  Rechazaste la apuesta: la CPU suma %d punto%s.\n", ptsRechazados, ptsRechazados == 1 ? "" : "s");
                printf("  Tanteador actual -> Vos: %d pts | CPU: %d pts (a %d puntos)\n", *puntosJ, *puntosC, pMaximos);
                printf("========================================================================================\n");
                pausarSeguro();
            } else if (opcion == 3 && nivelEnvido == 1) {
                nivelEnvido = 2;
                ptsRechazados = ptsAceptados;
                ptsAceptados = 5;
                quienResponde = 1;
            } else if ((opcion == 4 && nivelEnvido == 1) || (opcion == 3 && nivelEnvido == 2)) {
                nivelEnvido = 3;
                ptsRechazados = ptsAceptados;
                ptsAceptados = falta;
                quienResponde = 1;
            }
        } else {
            renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
            printf("\n========================================================================================\n");
            if (nivelEnvido == 1) printf("                       >>> ¡CANTASTE: ENVIDO! <<<\n");
            else if (nivelEnvido == 2) printf("                    >>> ¡CANTASTE: REAL ENVIDO! <<<\n");
            else printf("                   >>> ¡CANTASTE: FALTA ENVIDO! <<<\n");
            printf("========================================================================================\n");
            printf("  La CPU esta evaluando su jugada...\n");

            int accionCPU = 2;
            if (nivelEnvido == 1) {
                if (envidoC >= 31 && dificultadCPU >= 3 && (rand() % 100 < 50)) accionCPU = 4;
                else if (envidoC >= 28 && dificultadCPU >= 2 && (rand() % 100 < 60)) accionCPU = 3;
                else if (envidoC >= (24 + (4 - dificultadCPU)) || (rand() % 100 < 20)) accionCPU = 1;
            } else if (nivelEnvido == 2) {
                if (envidoC >= 31 && dificultadCPU >= 3 && (rand() % 100 < 40)) accionCPU = 3;
                else if (envidoC >= 27 || (dificultadCPU <= 2 && envidoC >= 25) || (rand() % 100 < 15)) accionCPU = 1;
            } else {
                if (envidoC >= 30 || (dificultadCPU >= 3 && envidoC >= 29) || (rand() % 100 < 10)) accionCPU = 1;
            }

            if (accionCPU == 4 && nivelEnvido == 1) {
                nivelEnvido = 3;
                ptsRechazados = ptsAceptados;
                ptsAceptados = falta;
                quienResponde = 0;
                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                   >>> ¡EL BOT RESPONDE: FALTA ENVIDO! <<<\n");
                printf("========================================================================================\n");
                pausarSeguro();
            } else if (accionCPU == 3 && nivelEnvido == 1) {
                nivelEnvido = 2;
                ptsRechazados = ptsAceptados;
                ptsAceptados = 5;
                quienResponde = 0;
                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                   >>> ¡EL BOT RESPONDE: REAL ENVIDO! <<<\n");
                printf("========================================================================================\n");
                pausarSeguro();
            } else if (accionCPU == 3 && nivelEnvido == 2) {
                nivelEnvido = 3;
                ptsRechazados = ptsAceptados;
                ptsAceptados = falta;
                quienResponde = 0;
                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                   >>> ¡EL BOT RESPONDE: FALTA ENVIDO! <<<\n");
                printf("========================================================================================\n");
                pausarSeguro();
            } else if (accionCPU == 1) {
                resuelto = true;
                bool ganaJ = (envidoJ > envidoC) || (envidoJ == envidoC && manoInicial == 0);
                if (ganaJ) *puntosJ += ptsAceptados;
                else *puntosC += ptsAceptados;

                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                     >>> ¡EL BOT DIJO: QUIERO! <<<\n");
                printf("========================================================================================\n");
                printf("  Tu Envido: %d  |  Envido de la CPU: %d\n", envidoJ, envidoC);
                if (envidoJ == envidoC) {
                    printf("  ¡Empate en envido! Gana quien es mano (%s).\n", (manoInicial == 0 ? "Vos" : "CPU"));
                }
                if (ganaJ) {
                    printf("  >>> ¡GANASTE EL ENVIDO! (+%d puntos) <<<\n", ptsAceptados);
                } else {
                    printf("  >>> LA CPU GANA EL ENVIDO. (+%d puntos) <<<\n", ptsAceptados);
                }
                printf("  Tanteador actual -> Vos: %d pts | CPU: %d pts (a %d puntos)\n", *puntosJ, *puntosC, pMaximos);
                printf("========================================================================================\n");
                pausarSeguro();
            } else {
                resuelto = true;
                *puntosJ += ptsRechazados;
                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                    >>> ¡EL BOT DIJO: NO QUIERO! <<<\n");
                printf("========================================================================================\n");
                printf("  La CPU rechazo el envido: ¡sumas %d punto%s!\n", ptsRechazados, ptsRechazados == 1 ? "" : "s");
                printf("  Tanteador actual -> Vos: %d pts | CPU: %d pts (a %d puntos)\n", *puntosJ, *puntosC, pMaximos);
                printf("========================================================================================\n");
                pausarSeguro();
            }
        }
    }
    *envidoCantado = 1;
}

void gestionarTruco(int iniciador, int *estTruco, int *ptsTruco, int *ultimoCantoTruco, int *puntosJ, int *puntosC, int pMaximos, bool *alMazo, int dificultadCPU, const Carta cCartas[], const int hJNros[], char hJPalos[][15], const int hCNros[], char hCPalos[][15], const Carta jCartas[], const bool dispJ[], int mano, int ganadasJ, int ganadasC) {
    int nivelActual = *estTruco + 1;
    int ptsAceptados = nivelActual + 1;
    int ptsRechazados = *ptsTruco;
    int quienResponde = 1 - iniciador;
    bool resuelto = false;

    while (!resuelto && !*alMazo) {
        if (quienResponde == 0) {
            renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
            printf("\n========================================================================================\n");
            if (nivelActual == 1) printf("                       >>> ¡LA CPU CANTA: TRUCO! <<<\n");
            else if (nivelActual == 2) printf("                     >>> ¡LA CPU CANTA: RETRUCO! <<<\n");
            else printf("                   >>> ¡LA CPU CANTA: VALE CUATRO! <<<\n");
            printf("========================================================================================\n");
            printf("  ¿Que deseas responder?\n");
            printf("  1. Quiero (Se juega la ronda por %d puntos)\n", ptsAceptados);
            printf("  2. No quiero (Te vas al mazo, la CPU suma %d punto%s)\n", ptsRechazados, ptsRechazados == 1 ? "" : "s");
            if (nivelActual == 1) {
                printf("  3. Cantar Retruco (Subir la apuesta a 3 puntos)\n");
            } else if (nivelActual == 2) {
                printf("  3. Cantar Vale Cuatro (Subir la apuesta a 4 puntos)\n");
            }

            int opcion = 0;
            if (nivelActual <= 2) opcion = leerEnteroEntre(1, 3, "Seleccione opcion: ");
            else opcion = leerEnteroEntre(1, 2, "Seleccione opcion: ");

            if (opcion == 1) {
                resuelto = true;
                *estTruco = nivelActual;
                *ptsTruco = ptsAceptados;
                *ultimoCantoTruco = 1;
                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                     >>> ¡ACEPTASTE EL %s! <<<\n", (nivelActual == 1 ? "TRUCO" : (nivelActual == 2 ? "RETRUCO" : "VALE CUATRO")));
                printf("========================================================================================\n");
                printf("  La ronda continua en juego por %d puntos.\n", *ptsTruco);
                printf("  Tanteador actual -> Vos: %d pts | CPU: %d pts (a %d puntos)\n", *puntosJ, *puntosC, pMaximos);
                printf("========================================================================================\n");
                pausarSeguro();
            } else if (opcion == 2) {
                resuelto = true;
                *alMazo = true;
                *puntosC += ptsRechazados;
                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                     >>> ¡DIJISTE: NO QUIERO! <<<\n");
                printf("========================================================================================\n");
                printf("  Te fuiste al mazo: la CPU suma %d punto%s.\n", ptsRechazados, ptsRechazados == 1 ? "" : "s");
                printf("  Tanteador actual -> Vos: %d pts | CPU: %d pts (a %d puntos)\n", *puntosJ, *puntosC, pMaximos);
                printf("========================================================================================\n");
                pausarSeguro();
            } else if (opcion == 3) {
                nivelActual++;
                ptsRechazados = ptsAceptados;
                ptsAceptados = nivelActual + 1;
                quienResponde = 1;
            }
        } else {
            renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
            printf("\n========================================================================================\n");
            if (nivelActual == 1) printf("                       >>> ¡CANTASTE: TRUCO! <<<\n");
            else if (nivelActual == 2) printf("                     >>> ¡CANTASTE: RETRUCO! <<<\n");
            else printf("                   >>> ¡CANTASTE: VALE CUATRO! <<<\n");
            printf("========================================================================================\n");
            printf("  La CPU esta pensando su respuesta...\n");

            int fuerza = fuerzaManoCPU(cCartas);
            int accionCPU = 2;

            if (nivelActual == 1) {
                if (fuerza >= 6 && dificultadCPU >= 2 && (rand() % 100 < 40)) accionCPU = 3;
                else if (fuerza >= 3 || (dificultadCPU == 1 && (rand() % 100 < 50)) || (rand() % 100 < (20 + dificultadCPU * 15 + fuerza * 5))) accionCPU = 1;
            } else if (nivelActual == 2) {
                if (fuerza >= 7 && dificultadCPU >= 3 && (rand() % 100 < 35)) accionCPU = 3;
                else if (fuerza >= 5 || (rand() % 100 < (15 + dificultadCPU * 12 + fuerza * 4))) accionCPU = 1;
            } else {
                if (fuerza >= 6 || (rand() % 100 < (10 + dificultadCPU * 10 + fuerza * 5))) accionCPU = 1;
            }

            if (accionCPU == 3) {
                nivelActual++;
                ptsRechazados = ptsAceptados;
                ptsAceptados = nivelActual + 1;
                quienResponde = 0;
                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                if (nivelActual == 2) printf("                   >>> ¡EL BOT RESPONDE: RETRUCO! <<<\n");
                else printf("                >>> ¡EL BOT RESPONDE: VALE CUATRO! <<<\n");
                printf("========================================================================================\n");
                pausarSeguro();
            } else if (accionCPU == 1) {
                resuelto = true;
                *estTruco = nivelActual;
                *ptsTruco = ptsAceptados;
                *ultimoCantoTruco = 0;
                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                     >>> ¡EL BOT DIJO: QUIERO! <<<\n");
                printf("========================================================================================\n");
                printf("  ¡La CPU acepto! La ronda continua y se juega por %d puntos.\n", *ptsTruco);
                printf("  Tanteador actual -> Vos: %d pts | CPU: %d pts (a %d puntos)\n", *puntosJ, *puntosC, pMaximos);
                printf("========================================================================================\n");
                pausarSeguro();
            } else {
                resuelto = true;
                *alMazo = true;
                *puntosJ += ptsRechazados;
                renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
                printf("\n========================================================================================\n");
                printf("                    >>> ¡EL BOT DIJO: NO QUIERO! <<<\n");
                printf("========================================================================================\n");
                printf("  La CPU se va al mazo: ¡sumas %d punto%s!\n", ptsRechazados, ptsRechazados == 1 ? "" : "s");
                printf("  Tanteador actual -> Vos: %d pts | CPU: %d pts (a %d puntos)\n", *puntosJ, *puntosC, pMaximos);
                printf("========================================================================================\n");
                pausarSeguro();
            }
        }
    }
}

void procesarTurnoJugador(int *jCarta, bool dispJ[], int hJNros[], char hJPalos[][15], int *turno, bool *primerCarta, int *accion, int *estTruco, int *ptsTruco, int *ultimoCantoTruco, int *puntosJ, int *puntosC, bool *alMazo, int pMaximos, int envidoJ, int envidoC, int *envidoCantado, int dificultadCPU, int manoInicial, const int hCNros[], char hCPalos[][15], const Carta cCartas[], const Carta jCartas[], int mano, int ganadasJ, int ganadasC) {
    int cartaSel;
    bool turnoValido = false;
    bool puedeCantarEnvido = (*primerCarta && *envidoCantado == 0 && dispJ[0] && dispJ[1] && dispJ[2]);
    bool puedeSubirTruco = (*estTruco < 3 && *ultimoCantoTruco != 0);

    printf("\nTe toca jugar:\n");
    printf("1. Jugar carta\n");
    if (puedeCantarEnvido) {
        printf("2. Cantar Envido\n3. Cantar Real Envido\n4. Cantar Falta Envido\n");
    }
    if (puedeSubirTruco) {
        if (*estTruco == 0) printf("5. Cantar Truco\n");
        else if (*estTruco == 1) printf("5. Cantar Retruco\n");
        else if (*estTruco == 2) printf("5. Cantar Vale Cuatro\n");
    }
    printf("6. Irse al mazo\n");

    *accion = leerEnteroEntre(1, 6, "Seleccione opcion: ");

    if (*accion == 1) {
        do {
            cartaSel = leerEnteroEntre(1, 3, "Seleccione carta (1-3): ") - 1;
            turnoValido = (cartaSel >= 0 && cartaSel <= 2 && dispJ[cartaSel]);
            if (!turnoValido) printf("Esa carta no esta disponible.\n");
        } while (!turnoValido);
        *jCarta = cartaSel;
        dispJ[cartaSel] = false;
        hJNros[mano] = jCartas[cartaSel].numero;
        strcpy(hJPalos[mano], jCartas[cartaSel].palo);
        *primerCarta = false;
        *turno = 1;
    } else if (*accion >= 2 && *accion <= 4) {
        if (puedeCantarEnvido) {
            gestionarEnvido(0, *accion - 1, envidoJ, envidoC, puntosJ, puntosC, pMaximos, envidoCantado, dificultadCPU, manoInicial, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
        } else {
            printf("El envido solo se puede cantar antes de jugar la primera carta.\n");
            pausarSeguro();
        }
    } else if (*accion == 5) {
        if (puedeSubirTruco) {
            if (*envidoCantado == 0) *envidoCantado = 1;
            gestionarTruco(0, estTruco, ptsTruco, ultimoCantoTruco, puntosJ, puntosC, pMaximos, alMazo, dificultadCPU, cCartas, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
        } else {
            printf("No puedes cantar o subir el truco en este momento.\n");
            pausarSeguro();
        }
    } else if (*accion == 6) {
        printf("Te fuiste al mazo.\n");
        *puntosC += *ptsTruco;
        *alMazo = true;
        pausarSeguro();
    }
}

void procesarTurnoCPU(int *cCarta, bool dispC[], int hCNros[], char hCPalos[][15], int *turno, bool *primerCarta, bool *alMazo, int jCarta, int envidoC, int envidoJ, int *envidoCantado, int dificultadCPU, int manoInicial, int *puntosJ, int *puntosC, int *estTruco, int *ptsTruco, int *ultimoCantoTruco, const Carta cCartas[], const Carta jCartas[], int mano, int pMaximos, const int hJNros[], char hJPalos[][15], const bool dispJ[], int ganadasJ, int ganadasC) {
    bool puedeEnvido = (*primerCarta && *envidoCantado == 0 && dispC[0] && dispC[1] && dispC[2]);

    if (puedeEnvido && decidirEnvidoCPU(envidoC, dificultadCPU)) {
        gestionarEnvido(1, 1, envidoJ, envidoC, puntosJ, puntosC, pMaximos, envidoCantado, dificultadCPU, manoInicial, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
    }

    if (!*alMazo && *estTruco < 3 && *ultimoCantoTruco != 1) {
        bool cantarTruco = false;
        int fuerza = fuerzaManoCPU(cCartas);
        if (*estTruco == 0 && (jCarta != -1 || manoInicial == 1)) {
            cantarTruco = decidirTrucoCPU(fuerza, dificultadCPU);
        } else if (*estTruco == 1 && fuerza >= 6 && dificultadCPU >= 2) {
            cantarTruco = (rand() % 100 < 35);
        } else if (*estTruco == 2 && fuerza >= 7 && dificultadCPU >= 3) {
            cantarTruco = (rand() % 100 < 30);
        }

        if (cantarTruco) {
            if (*envidoCantado == 0) *envidoCantado = 1;
            gestionarTruco(1, estTruco, ptsTruco, ultimoCantoTruco, puntosJ, puntosC, pMaximos, alMazo, dificultadCPU, cCartas, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
        }
    }

    if (!*alMazo) {
        *cCarta = elegirCartaCPU(cCartas, dispC, (jCarta != -1 ? jCartas[jCarta].valores.jerarquiaTruco : 0), (jCarta != -1 ? 1 : 0), dificultadCPU);
        dispC[*cCarta] = false;
        hCNros[mano] = cCartas[*cCarta].numero;
        strcpy(hCPalos[mano], cCartas[*cCarta].palo);
        *primerCarta = false;
        *turno = 0;
    }
}

void procesarResultadoRonda(int mano, int jCarta, int cCarta, const Carta jCartas[], const Carta cCartas[], int *ganador, int *ganadasJ, int *ganadasC, int resultadoMano[], int resultadosManosMatriz[CANT_MANOS][2], int *jugadorLider) {
    char msjMesa[100];
    sprintf(msjMesa, "Resultado Mano %d: ", mano + 1);
    if (jCartas[jCarta].valores.jerarquiaTruco > cCartas[cCarta].valores.jerarquiaTruco) {
        *ganador = 0;
        (*ganadasJ)++;
        resultadoMano[mano] = 1;
        resultadosManosMatriz[mano][0] = 1;
        strcat(msjMesa, "¡Gana el Jugador!");
    } else if (cCartas[cCarta].valores.jerarquiaTruco > jCartas[jCarta].valores.jerarquiaTruco) {
        *ganador = 1;
        (*ganadasC)++;
        resultadoMano[mano] = 2;
        resultadosManosMatriz[mano][1] = 1;
        strcat(msjMesa, "Gana la CPU.");
    } else {
        *ganador = 2;
        resultadoMano[mano] = 0;
        resultadosManosMatriz[mano][0] = 1;
        resultadosManosMatriz[mano][1] = 1;
        strcat(msjMesa, "Empate (Parda).");
    }
    printf("\n========================================================================================\n");
    printf("    >>> %s <<<\n", msjMesa);
    printf("========================================================================================\n");
    pausarSeguro();
    *jugadorLider = *ganador;
}

void jugarRonda(Carta jCartas[], Carta cCartas[], int *puntosJ, int *puntosC, int pMaximos, int dificultadCPU, int manoInicial) {
    bool dispJ[3] = {true, true, true};
    bool dispC[3] = {true, true, true};
    int resultadoMano[3] = {0, 0, 0};
    int hJNros[3] = {0, 0, 0};
    int hCNros[3] = {0, 0, 0};
    char hJPalos[3][15] = {"", "", ""};
    char hCPalos[3][15] = {"", "", ""};
    int resultadosManosMatriz[CANT_MANOS][2] = {{0}};

    int mano = 0, jugadorLider = manoInicial, turno;
    int jCarta = -1, cCarta = -1, ganador = 0, ganadasJ = 0, ganadasC = 0;
    int envidoCantado = 0, envidoJ = calcularEnvido(jCartas), envidoC = calcularEnvido(cCartas);
    int estTruco = 0, ptsTruco = 1, ultimoCantoTruco = -1, accion;
    bool alMazo = false, primerCarta = true;

    while (mano < 3 && !alMazo && *puntosJ < pMaximos && *puntosC < pMaximos) {
        turno = (jugadorLider == 0) ? 0 : 1;
        jCarta = -1;
        cCarta = -1;

        while ((jCarta == -1 || cCarta == -1) && !alMazo) {
            renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);

            if (turno == 0) {
                procesarTurnoJugador(&jCarta, dispJ, hJNros, hJPalos, &turno, &primerCarta, &accion, &estTruco, &ptsTruco, &ultimoCantoTruco, puntosJ, puntosC, &alMazo, pMaximos, envidoJ, envidoC, &envidoCantado, dificultadCPU, manoInicial, hCNros, hCPalos, cCartas, jCartas, mano, ganadasJ, ganadasC);
            } else {
                procesarTurnoCPU(&cCarta, dispC, hCNros, hCPalos, &turno, &primerCarta, &alMazo, jCarta, envidoC, envidoJ, &envidoCantado, dificultadCPU, manoInicial, puntosJ, puntosC, &estTruco, &ptsTruco, &ultimoCantoTruco, cCartas, jCartas, mano, pMaximos, hJNros, hJPalos, dispJ, ganadasJ, ganadasC);
            }
        }

        if (!alMazo && jCarta != -1 && cCarta != -1) {
            renderizarMesa(*puntosJ, *puntosC, pMaximos, hJNros, hJPalos, hCNros, hCPalos, jCartas, dispJ, mano, ganadasJ, ganadasC);
            procesarResultadoRonda(mano, jCarta, cCarta, jCartas, cCartas, &ganador, &ganadasJ, &ganadasC, resultadoMano, resultadosManosMatriz, &jugadorLider);
            mano++;
        }

        if (ganadasJ >= 2 || ganadasC >= 2) mano = 3;
    }

    if (!alMazo) {
        int ganadorRonda = determinarGanadorRonda(resultadoMano, ganadasJ, ganadasC, manoInicial);
        printf("\n========================================================================================\n");
        if (ganadorRonda == 0) {
            printf("  ¡GANASTE LA RONDA! +%d punto%s.\n", ptsTruco, ptsTruco == 1 ? "" : "s");
            *puntosJ += ptsTruco;
        } else if (ganadorRonda == 1) {
            printf("  LA CPU GANA LA RONDA. +%d punto%s.\n", ptsTruco, ptsTruco == 1 ? "" : "s");
            *puntosC += ptsTruco;
        } else {
            printf("  Las tres manos fueron pardas: gana quien fue mano. +%d punto%s.\n", ptsTruco, ptsTruco == 1 ? "" : "s");
            if (manoInicial == 0) *puntosJ += ptsTruco;
            else *puntosC += ptsTruco;
        }
        printf("  Tanteador actual -> Vos: %d pts | CPU: %d pts (a %d puntos)\n", *puntosJ, *puntosC, pMaximos);
        printf("========================================================================================\n");
        mostrarMatrizResultados(resultadosManosMatriz);
    }
    pausarSeguro();
}

int determinarGanadorRonda(const int resultadoMano[], int ganadasJ, int ganadasC, int manoInicial) {
    int ganador = 2;
    if (resultadoMano[0] != 0) {
        if (resultadoMano[1] == 0 || resultadoMano[1] == resultadoMano[0]) {
            ganador = resultadoMano[0] - 1;
        } else {
            if (resultadoMano[2] == 0) ganador = resultadoMano[0] - 1;
            else ganador = resultadoMano[2] - 1;
        }
    } else {
        if (resultadoMano[1] != 0) ganador = resultadoMano[1] - 1;
        else if (resultadoMano[2] != 0) ganador = resultadoMano[2] - 1;
        else ganador = manoInicial;
    }
    return ganador;
}

void mostrarMatrizResultados(const int resultados[CANT_MANOS][2]) {
    int fila = 0;
    printf("\n  +------+---------+-----+\n");
    printf("  | Mano | Jugador | CPU |\n");
    printf("  +------+---------+-----+\n");
    for (fila = 0; fila < CANT_MANOS; fila++) {
        printf("  |  %d   |    %d    |  %d  |\n", fila + 1, resultados[fila][0], resultados[fila][1]);
    }
    printf("  +------+---------+-----+\n");
}

void ordenarCartasPorJerarquia(Carta cartas[], int cantidadCartas) {
    int i = 0;
    int j = 0;
    for (i = 0; i < cantidadCartas - 1; i++) {
        for (j = i + 1; j < cantidadCartas; j++) {
            if (cartas[i].valores.jerarquiaTruco > cartas[j].valores.jerarquiaTruco) {
                intercambiarCartas(&cartas[i], &cartas[j]);
            }
        }
    }
}

int buscarCartaPorNumero(const Carta cartas[], int cantidadCartas, int numeroBuscado) {
    int i = 0;
    int posicionEncontrada = -1;
    bool encontrada = false;

    while (i < cantidadCartas && !encontrada) {
        if (cartas[i].numero == numeroBuscado) {
            posicionEncontrada = i;
            encontrada = true;
        }
        i++;
    }
    return posicionEncontrada;
}

int buscarCartaPorPaloYNumero(const Carta cartas[], int cantidadCartas, const char paloBuscado[], int numeroBuscado) {
    int i = 0;
    int posicionEncontrada = -1;
    bool encontrada = false;

    while (i < cantidadCartas && !encontrada) {
        if (cartas[i].numero == numeroBuscado && strcmp(cartas[i].palo, paloBuscado) == 0) {
            posicionEncontrada = i;
            encontrada = true;
        }
        i++;
    }
    return posicionEncontrada;
}
