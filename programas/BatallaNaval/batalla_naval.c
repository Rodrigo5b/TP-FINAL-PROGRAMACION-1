
void MostrarPortada();
int MenuPrincipal();
int Jugar(int modo1, int n, char tablero[10][10]);
void Instrucciones();
void Configuracion(int *opcionConfig);
void SalirDelJuego();
void InicializarTablero(char tablero[10][10], int n);
void DibujarTablero(char tablero[10][10], int n);
void pausar();



int batalla_Naval() {
    int opcion1, modo1 = 0, opcionConfig = 0;
    char tableroJugador[10][10];
    
 

    do {
        limpiarPantalla();
        MostrarPortada();
        opcion1 = MenuPrincipal();

        switch (opcion1) {
            case 1:
                Jugar(modo1, 10, tableroJugador);
                break;
            case 2:
                Instrucciones();
                break;
            case 3:
                Configuracion(&opcionConfig);
                break;
            case 4:
                SalirDelJuego();
                break;
            default:
                printf("\nOpcion invalida.\n");
                printf("Ingrese un numero del 1 al 4.\n");
                pausar();
                break;
        }

    } while (opcion1 != 4);

    return 0;
}



void MostrarPortada() {
    printf(".......................................................... ................... .\n");
    printf("....... .....................................-:.................................\n");
    printf("............................. ..............::::..................... ...........\n");
    printf("........................ ..............::::..-:..::::...........................\n");
    printf("................. .. .................:-::::----::::-................. .........\n");
    printf("......................................:----..-: :----...........................\n");
    printf(".............................................-. ............. ..................\n");
    printf(".......... ..................................-....... ..........................\n");
    printf("...................... ..... .............@@@@@@@@. ............................\n");
    printf("............. ..........................@@++++++@@..............................\n");
    printf("......................................%%@@@@@@+*@@@................... ...........\n");
    printf("......................................%%@@@@@@+*@@@..............................\n");
    printf("......................................%%@********@@..............................\n");
    printf("............. ..... ..... ............%%@########@@..............................\n");
    printf("................... ..................%%@########@@..............................\n");
    printf("...... ................. .............%%@********@@..............................\n");
    printf(".... ................. ...............%%@@@@@@@@@@@................. ............\n");
    printf("................................... ..%%@++++++++@@.................. ... .......\n");
    printf("...................................@@@@@++++++++@@@%%............................\n");
    printf(".................................%%@+++++++++++++++@%%...@%%.....@.................\n");
    printf(".......@@@..........@@@.........@@@@@@*+@@@@@+*@*+@%%.%%@#%%@:.@@#@@......... @@@..\n");
    printf(".......@%%#@@@@@.....@@#@@@@@....@#++++++++++++++++@@@@@@@@@@@@@@@@%%...@@@@@#%%@..\n");
    printf("........%%@@@###@%%.....@@@###@@..@@@@@@*+@%%+#@+*@*+@@#############@%%.%%@###@@@%%...\n");
    printf("::::::::@@@@@@@@@::::+@@@@@@@@:+@@################@@@@@@@@@@@@@@@@@:@@@@@@@@@:::\n");
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("=+@@@@@@=:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::@@\n");
    printf("........#@@%%------------------------------------------------------------------@@\n");
    printf(". ..........@#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++@@\n");
    printf(".... ........%%@+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#@..\n");
    printf(".......... ....@@+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#@..\n");
    printf("::::::........:-----++++++=------=++++++-------++++++-------++++++=------=+@%%...\n");
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

int MenuPrincipal() {
    printf("\n");
    printf("==========================================\n");
    printf("             BATALLA NAVAL\n");
    printf("==========================================\n");
    printf("\n");
    printf("             MENU PRINCIPAL\n");
    printf("\n");
    printf("        1. Jugar\n");
    printf("        2. Instrucciones\n");
    printf("        3. Configuracion\n");
    printf("        4. Salir\n");
    printf("\n");
    printf("==========================================\n");

    return leerEntero("Seleccione una opcion: ");
}

int Jugar(int modo1, int n, char tablero[10][10]) {
    limpiarPantalla();

    printf("==========================================\n");
    printf("                  JUGAR\n");
    printf("==========================================\n");
    printf("\n");
    printf("Seleccione el modo de juego:\n");
    printf("\n");
    printf("1. Jugador vs Computadora\n");
    printf("2. Jugador vs Jugador\n");
    printf("3. Volver\n");
    printf("\n");

    modo1 = leerEntero("Seleccione una opcion: ");

    switch (modo1) {
        case 1:
            limpiarPantalla();
            printf("==========================================\n");
            printf("       JUGAR VS COMPUTADORA\n");
            printf("==========================================\n");
            printf("\n");
            printf("Preparando partida en tablero 10x10...\n");
            printf("\n");
            printf("Tablero con tus barcos ubicados ():\n");
            DibujarTablero(tablero, n);
            pausar();
            break;

        case 2:
            limpiarPantalla();
            printf("==========================================\n");
            printf("          JUGADOR VS JUGADOR\n");
            printf("==========================================\n");
            printf("\n");
            printf("Preparando partida...\n");
            printf("\n");
            pausar();
            break;

        case 3:
            
            break;

        default:
            printf("\nOpcion invalida.\n");
            pausar();
            break;
    }

    return 0;
}

void Instrucciones() {
    limpiarPantalla();

    printf("==========================================\n");
    printf("              INSTRUCCIONES\n");
    printf("==========================================\n");
    printf("\n");
    printf("El objetivo de Batalla Naval es hundir\n");
    printf("todos los barcos del oponente.\n");
    printf("\n");
    printf("Cada jugador tendra un tablero de 10x10\n");
    printf("donde colocara sus barcos.\n");
    printf("\n");
    printf("Durante la partida, los jugadores\n");
    printf("realizaran disparos sobre el tablero\n");
    printf("del oponente.\n");
    printf("\n");
    printf("Si un disparo alcanza un barco,\n");
    printf("se produce un impacto.\n");
    printf("\n");
    printf("Gana el jugador que hunda todos\n");
    printf("los barcos del oponente.\n");
    printf("\n");
    printf("==========================================\n");
    printf("\n");
    printf("Presione ENTER para volver al menu.\n");
    pausar();
}

void Configuracion(int *opcionConfig) {
    limpiarPantalla();

    printf("==========================================\n");
    printf("              CONFIGURACION\n");
    printf("==========================================\n");
    printf("\n");
    printf("1. Cantidad de barcos\n");
    printf("2. Dificultad\n");
    printf("3. Volver\n");
    printf("\n");

    *opcionConfig = leerEntero("Seleccione una opcion: ");

    switch (*opcionConfig) {
        case 1:
            limpiarPantalla();
            printf("==========================================\n");
            printf("          CANTIDAD DE BARCOS\n");
            printf("==========================================\n");
            printf("\n");
            printf("Esta opcion permitira elegir la\n");
            printf("cantidad de barcos.\n");
            printf("\n");
            pausar();
            break;

        case 2:
            limpiarPantalla();
            printf("==========================================\n");
            printf("               DIFICULTAD\n");
            printf("==========================================\n");
            printf("\n");
            printf("1. Facil\n");
            printf("2. Normal\n");
            printf("3. Dificil\n");
            printf("\n");
            printf("La dificultad afectara a la\n");
            printf("inteligencia de la computadora.\n");
            printf("\n");
            pausar();
            break;

        case 3:
            // Volver
            break;

        default:
            printf("\nOpcion invalida.\n");
            pausar();
            break;
    }
}

void SalirDelJuego() {
    limpiarPantalla();

    printf("==========================================\n");
    printf("              BATALLA NAVAL\n");
    printf("==========================================\n");
    printf("\n");
    printf("Gracias por jugar.\n");
    printf("\n");
    printf("Saliendo del juego...\n");
}



void DibujarTablero(char tablero[10][10], int n) {
    printf("    ");
    for (int j = 1; j <= n; j++) {
        if (j < 10) {
            printf("%d  ", j);
        } else {
            printf("%d ", j);
        }
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
       
            printf("%d   ", i + 1);
      

        for (int j = 0; j < n; j++) {
            printf("~  ");
        }
        printf("\n");
    }
    printf("\n");
}
void pausar() {
    printf("Presione Enter para continuar...");
    fflush(stdout);
    fflush(stdin);
    getchar();
}
