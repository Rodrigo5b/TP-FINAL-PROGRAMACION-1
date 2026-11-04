#define MAX_TAM 10
#define TOTAL_BARCOS 4
#define TOTAL_IMPACTOS 16

typedef struct {
    int fila;
    int col;
} Coordenada;

typedef struct {
    char nombre[30];      
    int tamanio;           
    int orientacion;       
    Coordenada origen;      
    int impactos;          
    bool hundido;           
} Barco;

typedef struct {
    int dimension;
    char casillas[MAX_TAM][MAX_TAM];
} Tablero;

typedef struct {
    Tablero tableroPropio;          
    Tablero tableroDisparos;        
    Barco flota[TOTAL_BARCOS];     
    int totalImpactos;
    int botModo;
    int botFilaAcierto;
    int botColAcierto;
    int botDireccion;
} Jugador;

typedef struct {
    Jugador jugador;               
    Jugador bot;                    
    int tamanioTablero;             
    int dificultad;                
} Partida;

/* Prototipos */
int  batalla_Naval(void);
void esperarTecla(void);
void mostrarPortada(void);
int  menuPrincipal(void);
void jugar(int n);
void instrucciones(void);
void configuracion(void);
void salirDelJuego(void);

void inicializarTablero(Tablero *tab, int dimension);
void inicializarJugador(Jugador *jugador, int dimension);

void dibujarTablero(const Tablero *tab);
void dibujarTableroJugador(const Tablero *tabPropio, const Tablero *disparosRivales);
void dibujarTableroEnemigo(const Tablero *disparos);

bool cabeEnTablero(const Barco *barco, int dimension);
bool estaLibre(const Tablero *tab, const Barco *barco);
void colocarUnBarco(Tablero *tab, const Barco *barco);
void colocarBarcosJugador(Jugador *jugador);
void colocarBarcosBot(Jugador *bot);

void disparoJugador(Jugador *atacante, Jugador *defensor);
void disparoBot(Jugador *bot, Jugador *humano, int dificultad);
bool verificarImpactoEnFlota(Jugador *defensor, Coordenada tiro, bool esJugadorAtacante);
int  contarImpactos(const Tablero *disparos);

int batalla_Naval(void) {
    srand((unsigned int)time(NULL));
    int opcion;

    do {
        limpiarPantalla();
        mostrarPortada();
        opcion = menuPrincipal();

        switch (opcion) {
            case 1:
                jugar(MAX_TAM);
                break;
            case 2:
                instrucciones();
                break;
            case 3:
                configuracion();
                break;
            case 4:
                salirDelJuego();
                break;
            default:
                printf("\nOpcion invalida.\n");
                esperarTecla();
                break;
        }
    } while (opcion != 4);

    return 0;
}

void esperarTecla(void) {
    system("pause");
}

void mostrarPortada(void) {
    printf(".......................................................... ................... .\n");
    printf("....... .....................................-:.................................\n");
    printf("............................. ..............::::..................... ..........\n");
    printf("........................ ..............::::..-:..::::...........................\n");
    printf("................. .. .................:-::::----::::-................. .........\n");
    printf("......................................:----..-: :----...........................\n");
    printf(".............................................-. ............. ..................\n");
    printf(".......... ..................................-....... ..........................\n");
    printf("...................... ..... .............@@@@@@@@. ............................\n");
    printf("............. ..........................@@++++++@@..............................\n");
    printf(".....................................@@@@@@+*@@@................... ..........\n");
    printf("......................................@@@@@@+*@@@..............................\n");
    printf("......................................@********@@..............................\n");
    printf("............. ..... ..... ............@########@@..............................\n");
    printf("................... ..................@########@@..............................\n");
    printf("...... ................. .............@********@@..............................\n");
    printf(".... ................. ...............@@@@@@@@@@@................. ............\n");
    printf("................................... ..@++++++++@@.................. ... .......\n");
    printf("...................................@@@@@++++++++@@@............................\n");
    printf(".................................@+++++++++++++++@...@......@.................\n");
    printf(".......@@@..........@@@.........@@@@@@*+@@@@@+*@*+@.@#@:.@@#@@......... @@@..\n");
    printf(".......@#@@@@@     @@#@@@@@    @#++++++++++++++++@@@@@@@@@@@@@@@@...@@@@@#@..\n");
    printf("........@@@###@     @@@###@@  @@@@@@*+@+#@+*@*+@@#############@.@###@@@...\n");
    printf("::::::::@@@@@@@@@::::+@@@@@@@@:+@@################@@@@@@@@@@@@@@@@@:@@@@@@@@@:::\n");
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("=+@@@@@@=:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::@@\n");
    printf("........#@@------------------------------------------------------------------@@\n");
    printf(". ..........@#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++@@\n");
    printf(".... ........@+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#@..\n");
    printf(".......... ....@@+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#@..\n");
    printf("::::::........:-----++++++=------=++++++-------++++++-------++++++=------=+@..\n");
    printf("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("######*------*######------+######+------######*------*######------+######+------\n");
    printf("################################################################################\n");
    printf("################################################################################\n");
    printf("................................................................................\n");
    printf("............ .................................................... ..............\n");
    printf(".................... ....... .............................................. ....\n");
    printf(".. ................................. ................................. .........\n");
    printf("...... .......... ......................................................... ....\n");
}

int menuPrincipal(void) {
    printf("\n==========================================\n");
    printf("             BATALLA NAVAL\n");
    printf("==========================================\n\n");
    printf("             MENU PRINCIPAL\n\n");
    printf("        1. Jugar\n");
    printf("        2. Instrucciones\n");
    printf("        3. Configuracion\n");
    printf("        4. Salir\n\n");
    printf("==========================================\n");
    return leerEnteroEntre(1, 4, "Seleccione una opcion");
}

void inicializarTablero(Tablero *tab, int dimension) {
    tab->dimension = dimension;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            tab->casillas[i][j] = '~';
        }
    }
}

void inicializarJugador(Jugador *jugador, int dimension) {
    jugador->totalImpactos = 0;
    jugador->botModo = 0;
    jugador->botFilaAcierto = 0;
    jugador->botColAcierto = 0;
    jugador->botDireccion = 0;

    inicializarTablero(&jugador->tableroPropio, dimension);
    inicializarTablero(&jugador->tableroDisparos, dimension);

    int tamanios[TOTAL_BARCOS] = {5, 4, 4, 3};
    const char *nombres[TOTAL_BARCOS] = {
        "Portaaviones (5 casillas)",
        "Acorazado 1 (4 casillas)",
        "Acorazado 2 (4 casillas)",
        "Destructor (3 casillas)"
    };

    for (int k = 0; k < TOTAL_BARCOS; k++) {
        strncpy(jugador->flota[k].nombre, nombres[k], sizeof(jugador->flota[k].nombre) - 1);
        jugador->flota[k].nombre[sizeof(jugador->flota[k].nombre) - 1] = '\0';
        jugador->flota[k].tamanio = tamanios[k];
        jugador->flota[k].orientacion = 1;
        jugador->flota[k].origen.fila = -1;
        jugador->flota[k].origen.col = -1;
        jugador->flota[k].impactos = 0;
        jugador->flota[k].hundido = false;
    }
}

void jugar(int n) {
    Partida partida;

    limpiarPantalla();
    printf("==========================================\n");
    printf("           SELECCION DE DIFICULTAD\n");
    printf("==========================================\n\n");
    printf("1. Facil   - El bot dispara al azar\n");
    printf("2. Dificil - El bot busca hundir tus barcos\n");
    printf("3. Volver\n\n");
    
    int opcion = leerEnteroEntre(1, 3, "Seleccione una opcion");
    if (opcion == 3) {
        return;
    }

    partida.dificultad = opcion;
    partida.tamanioTablero = n;

    inicializarJugador(&partida.jugador, n);
    inicializarJugador(&partida.bot, n);

    limpiarPantalla();
    printf("Coloca tus barcos para comenzar la partida.\n");
    esperarTecla();

    colocarBarcosJugador(&partida.jugador);
    colocarBarcosBot(&partida.bot);

    while (partida.jugador.totalImpactos < TOTAL_IMPACTOS &&
           partida.bot.totalImpactos < TOTAL_IMPACTOS) {

        limpiarPantalla();
        printf("==========================================\n");
        printf("              BATALLA NAVAL\n");
        printf("==========================================\n\n");
        
        if (partida.dificultad == 1) {
            printf("Dificultad: FACIL\n\n");
        } else {
            printf("Dificultad: DIFICIL\n\n");
        }

        printf("TU TABLERO (X = barco impactado, O = agua):\n");
        dibujarTableroJugador(&partida.jugador.tableroPropio, &partida.bot.tableroDisparos);

        printf("TABLERO ENEMIGO (X = impacto, O = agua):\n");
        dibujarTableroEnemigo(&partida.jugador.tableroDisparos);

        printf("TU TURNO\n");
        disparoJugador(&partida.jugador, &partida.bot);

        partida.jugador.totalImpactos = contarImpactos(&partida.jugador.tableroDisparos);

        if (partida.jugador.totalImpactos < TOTAL_IMPACTOS) {
            printf("\nPresione una tecla para el turno del bot...\n");
            esperarTecla();

            limpiarPantalla();
            printf("TURNO DE LA COMPUTADORA\n");
            
            disparoBot(&partida.bot, &partida.jugador, partida.dificultad);
            partida.bot.totalImpactos = contarImpactos(&partida.bot.tableroDisparos);

            printf("\nPresione una tecla para continuar...\n");
            esperarTecla();
        }
    }

    limpiarPantalla();
    printf("==========================================\n");
    printf("             FIN DE LA PARTIDA\n");
    printf("==========================================\n\n");
    printf("TU TABLERO:\n");
    dibujarTableroJugador(&partida.jugador.tableroPropio, &partida.bot.tableroDisparos);
    printf("TABLERO ENEMIGO:\n");
    dibujarTableroEnemigo(&partida.jugador.tableroDisparos);

    if (partida.jugador.totalImpactos >= TOTAL_IMPACTOS) {
        printf("FELICIDADES! Hundiste todos los barcos del bot.\n");
    } else {
        printf("La computadora hundio todos tus barcos.\n");
    }

    printf("\nPresione una tecla para volver al menu...\n");
    esperarTecla();
}

void instrucciones(void) {
    limpiarPantalla();
    printf("==========================================\n");
    printf("              INSTRUCCIONES\n");
    printf("==========================================\n\n");
    printf("El objetivo de Batalla Naval es hundir\n");
    printf("todos los barcos del oponente.\n\n");
    printf("El tablero es de un tamanio fijo de 10x10.\n");
    printf("Cada jugador posiciona su flota de 4 barcos:\n");
    printf("- 1 barco de 5 espacios\n");
    printf("- 2 barcos de 4 espacios\n");
    printf("- 1 barco de 3 espacios\n\n");
    printf("DIFICULTADES:\n");
    printf("- Facil: el bot dispara al azar.\n");
    printf("- Dificil: cuando el bot acierta, busca\n");
    printf("  alrededor para hundir el barco completo.\n\n");
    printf("==========================================\n\n");
    printf("Presione una tecla para volver al menu.\n");
    esperarTecla();
}

void configuracion(void) {
    int opcionConfig;
    limpiarPantalla();
    printf("==========================================\n");
    printf("              CONFIGURACION\n");
    printf("==========================================\n\n");
    printf("Nota: El tablero es fijo de 10x10.\n\n");
    printf("1. Cantidad de barcos (Informativo)\n");
    printf("2. Dificultad (Se elige al jugar)\n");
    printf("3. Volver\n\n");

    opcionConfig = leerEnteroEntre(1, 3, "Seleccione una opcion");

    switch (opcionConfig) {
        case 1:
            limpiarPantalla();
            printf("==========================================\n");
            printf("          CANTIDAD DE BARCOS\n");
            printf("==========================================\n\n");
            printf("La flota actual consta de:\n");
            printf("- 1 barco de 5 casillas\n");
            printf("- 2 barcos de 4 casillas\n");
            printf("- 1 barco de 3 casillas\n\n");
            esperarTecla();
            break;
        case 2:
            limpiarPantalla();
            printf("==========================================\n");
            printf("               DIFICULTAD\n");
            printf("==========================================\n\n");
            printf("1. Facil - El bot dispara al azar\n");
            printf("2. Dificil - El bot busca hundir barcos\n\n");
            printf("La dificultad se selecciona al iniciar\n");
            printf("una partida nueva desde el menu Jugar.\n\n");
            esperarTecla();
            break;
    }
}

void salirDelJuego(void) {
    limpiarPantalla();
    printf("==========================================\n");
    printf("              BATALLA NAVAL\n");
    printf("==========================================\n\n");
    printf("Gracias por jugar.\n\n");
    printf("Saliendo del juego...\n");
}

void dibujarTablero(const Tablero *tab) {
    int i, j;
    int n = tab->dimension;

    printf("    ");
    for (j = 0; j < n; j++) {
        printf("%-3d", j + 1);
    }
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("%-4d", i + 1);
        for (j = 0; j < n; j++) {
            printf("%c  ", tab->casillas[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void dibujarTableroJugador(const Tablero *tabPropio, const Tablero *disparosRivales) {
    int i, j;
    int n = tabPropio->dimension;

    printf("    ");
    for (j = 0; j < n; j++) {
        printf("%-3d", j + 1);
    }
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("%-4d", i + 1);
        for (j = 0; j < n; j++) {
            if (disparosRivales->casillas[i][j] == '~') {
                printf("%c  ", tabPropio->casillas[i][j]);
            } else {
                printf("%c  ", disparosRivales->casillas[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void dibujarTableroEnemigo(const Tablero *disparos) {
    int i, j;
    int n = disparos->dimension;

    printf("    ");
    for (j = 0; j < n; j++) {
        printf("%-3d", j + 1);
    }
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("%-4d", i + 1);
        for (j = 0; j < n; j++) {
            printf("%c  ", disparos->casillas[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool cabeEnTablero(const Barco *barco, int dimension) {
    if (barco->orientacion == 1) {
        return (barco->origen.fila >= 0 && barco->origen.fila < dimension &&
                barco->origen.col >= 0  && barco->origen.col + barco->tamanio - 1 < dimension);
    } else {
        return (barco->origen.fila >= 0 && barco->origen.fila + barco->tamanio - 1 < dimension &&
                barco->origen.col >= 0  && barco->origen.col < dimension);
    }
}

bool estaLibre(const Tablero *tab, const Barco *barco) {
    if (barco->orientacion == 1) {
        for (int i = barco->origen.col; i < barco->origen.col + barco->tamanio; i++) {
            if (tab->casillas[barco->origen.fila][i] == 'B') return false;
        }
    } else {
        for (int i = barco->origen.fila; i < barco->origen.fila + barco->tamanio; i++) {
            if (tab->casillas[i][barco->origen.col] == 'B') return false;
        }
    }
    return true;
}

void colocarUnBarco(Tablero *tab, const Barco *barco) {
    if (barco->orientacion == 1) {
        for (int i = barco->origen.col; i < barco->origen.col + barco->tamanio; i++) {
            tab->casillas[barco->origen.fila][i] = 'B';
        }
    } else {
        for (int i = barco->origen.fila; i < barco->origen.fila + barco->tamanio; i++) {
            tab->casillas[i][barco->origen.col] = 'B';
        }
    }
}

void colocarBarcosJugador(Jugador *jugador) {
    int n = jugador->tableroPropio.dimension;
    
    for (int k = 0; k < TOTAL_BARCOS; k++) {
        bool colocado = false;
        while (!colocado) {
            limpiarPantalla();
            printf("==========================================\n");
            printf("          COLOCACION DE BARCOS\n");
            printf("==========================================\n\n");
            dibujarTablero(&jugador->tableroPropio);
            printf("Colocando %s (barco %d de %d)\n\n", jugador->flota[k].nombre, k + 1, TOTAL_BARCOS);

            jugador->flota[k].origen.fila = leerEnteroEntre(1, n, "Ingrese fila inicial (1-10)") - 1;
            jugador->flota[k].origen.col  = leerEnteroEntre(1, n, "Ingrese columna inicial (1-10)") - 1;
            jugador->flota[k].orientacion = leerEnteroEntre(1, 2, "Orientacion (1=Horizontal, 2=Vertical)");

            if (cabeEnTablero(&jugador->flota[k], n)) {
                if (estaLibre(&jugador->tableroPropio, &jugador->flota[k])) {
                    colocarUnBarco(&jugador->tableroPropio, &jugador->flota[k]);
                    colocado = true;
                } else {
                    printf("\nError: Hay otro barco en esa posicion.\n");
                    esperarTecla();
                }
            } else {
                printf("\nError: El barco se sale del tablero.\n");
                esperarTecla();
            }
        }
    }
}

void colocarBarcosBot(Jugador *bot) {
    int n = bot->tableroPropio.dimension;
    for (int k = 0; k < TOTAL_BARCOS; k++) {
        bool colocado = false;
        while (!colocado) {
            bot->flota[k].origen.fila = rand() % n;
            bot->flota[k].origen.col  = rand() % n;
            bot->flota[k].orientacion = (rand() % 2) + 1;

            if (cabeEnTablero(&bot->flota[k], n)) {
                if (estaLibre(&bot->tableroPropio, &bot->flota[k])) {
                    colocarUnBarco(&bot->tableroPropio, &bot->flota[k]);
                    colocado = true;
                }
            }
        }
    }
}

bool verificarImpactoEnFlota(Jugador *defensor, Coordenada tiro, bool esJugadorAtacante) {
    for (int k = 0; k < TOTAL_BARCOS; k++) {
        Barco *b = &defensor->flota[k];
        bool acerto = false;

        if (b->orientacion == 1) {
            if (tiro.fila == b->origen.fila && tiro.col >= b->origen.col && tiro.col < b->origen.col + b->tamanio) {
                acerto = true;
            }
        } else {
            if (tiro.col == b->origen.col && tiro.fila >= b->origen.fila && tiro.fila < b->origen.fila + b->tamanio) {
                acerto = true;
            }
        }

        if (acerto) {
            b->impactos++;
            if (b->impactos >= b->tamanio && !b->hundido) {
                b->hundido = true;
                if (esJugadorAtacante) {
                    printf("¡Hundiste el %s enemigo!\n", b->nombre);
                } else {
                    printf("ALERTA: ¡El bot hundio tu %s!\n", b->nombre);
                }
                return true;
            }
            return false;
        }
    }
    return false;
}

void disparoJugador(Jugador *atacante, Jugador *defensor) {
    int n = atacante->tableroDisparos.dimension;
    Coordenada tiro;
    bool posicionValida = false;

    while (!posicionValida) {
        tiro.fila = leerEnteroEntre(1, n, "Ingrese la fila a la que quiere disparar") - 1;
        tiro.col  = leerEnteroEntre(1, n, "Ingrese la columna a la que quiere disparar") - 1;

        if (atacante->tableroDisparos.casillas[tiro.fila][tiro.col] != '~') {
            printf("Ya disparaste a esa casilla. Elegi otra.\n");
        } else {
            posicionValida = true;
        }
    }

    if (defensor->tableroPropio.casillas[tiro.fila][tiro.col] == 'B') {
        atacante->tableroDisparos.casillas[tiro.fila][tiro.col] = 'X';
        printf("Impacto! Le diste a un barco enemigo.\n");
        verificarImpactoEnFlota(defensor, tiro, true);
    } else {
        atacante->tableroDisparos.casillas[tiro.fila][tiro.col] = 'O';
        printf("Agua! No habia ningun barco en esa casilla.\n");
    }
}

void disparoBot(Jugador *bot, Jugador *humano, int dificultad) {
    int n = bot->tableroDisparos.dimension;
    Coordenada tiro;
    bool disparoHecho = false;

    if (dificultad == 2 && bot->botModo == 1) {
        while (!disparoHecho && bot->botModo == 1) {
            
            if (bot->botDireccion == 0) {
                tiro.fila = bot->botFilaAcierto - 1;
                tiro.col = bot->botColAcierto;
            } else if (bot->botDireccion == 1) {
                tiro.fila = bot->botFilaAcierto;
                tiro.col = bot->botColAcierto + 1;
            } else if (bot->botDireccion == 2) {
                tiro.fila = bot->botFilaAcierto + 1;
                tiro.col = bot->botColAcierto;
            } else if (bot->botDireccion == 3) {
                tiro.fila = bot->botFilaAcierto;
                tiro.col = bot->botColAcierto - 1;
            }

            if (tiro.fila >= 0 && tiro.fila < n && tiro.col >= 0 && tiro.col < n) {
                if (bot->tableroDisparos.casillas[tiro.fila][tiro.col] == '~') {
                    disparoHecho = true;
                } else {
                    bot->botDireccion++;
                    if (bot->botDireccion > 3) {
                        bot->botModo = 0;
                    }
                }
            } else {
                bot->botDireccion++;
                if (bot->botDireccion > 3) {
                    bot->botModo = 0;
                }
            }
        }
    }

    if (!disparoHecho) {
        do {
            tiro.fila = rand() % n;
            tiro.col  = rand() % n;
        } while (bot->tableroDisparos.casillas[tiro.fila][tiro.col] != '~');
    }

    if (humano->tableroPropio.casillas[tiro.fila][tiro.col] == 'B') {
        bot->tableroDisparos.casillas[tiro.fila][tiro.col] = 'X';
        printf("El bot acerto en la fila %d y columna %d\n", tiro.fila + 1, tiro.col + 1);
        
        bool hundioBarco = verificarImpactoEnFlota(humano, tiro, false);

        if (hundioBarco) {
            bot->botModo = 0;
        } else {
            if (bot->botModo == 0) {
                bot->botModo = 1;
                bot->botFilaAcierto = tiro.fila;
                bot->botColAcierto = tiro.col;
                bot->botDireccion = 0;
            } else {
                bot->botFilaAcierto = tiro.fila;
                bot->botColAcierto = tiro.col;
            }
        }
    } else {
        bot->tableroDisparos.casillas[tiro.fila][tiro.col] = 'O';
        printf("El bot fallo en la fila %d y columna %d\n", tiro.fila + 1, tiro.col + 1);

        if (bot->botModo == 1) {
            bot->botDireccion++;
            if (bot->botDireccion > 3) {
                bot->botModo = 0;
            }
        }
    }

    printf("\nTu tablero despues del disparo del bot:\n");
    dibujarTableroJugador(&humano->tableroPropio, &bot->tableroDisparos);
}

int contarImpactos(const Tablero *disparos) {
    int i, j;
    int cantidad = 0;
    int n = disparos->dimension;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (disparos->casillas[i][j] == 'X') cantidad++;
        }
    }
    return cantidad;
}
