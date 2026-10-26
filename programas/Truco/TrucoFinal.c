#define CANT_MANOS 3
#define CANT_CARTAS 40
#define CANT_CARTAS_JUGADOR 3

typedef struct
{
int jerarquiaTruco;
int valorEnvido;
}ValoresCarta

typedef struct
{
int numero;
char palo[10];
ValoresCarta valores;
bool disponible;
}Carta

typedef struct
{
int numeroMano;
int resultadoJugador;
int resultadoCpu;
}ResultadoMano

void mostrarMenuDeInicio();
int obtenerOpcionMenu();
int configurarPuntajeMaximo();
void mostrarReglas();
void mostrarInformacionDelTruco();
void mostrarComoSeJuega();
void mostrarValoresDelTruco();
void mostrarValoresDelEnvido();
void mostrarEmpates();
void crearMazo(Carta mazo[]);
void barajarMazo(Carta mazo[]);
void Ronda(cadena palos[], int numerosDelTruco[], cadena paloDeLasCartas[], int numerosDeLasCartas[], int cartasUsadas[], int mazo[CANT_PALOS][CANT_NUMEROS], int cartasJugador[], int cartasDelCpu[], int mano);
void repartirCartas(int cartasUsadas[], int cartasJugador[], int cartasDelCpu[], int mano);
int obtenerSiguienteMano(int mano);
void dibujarCarta(int posicion, cadena paloDeLasCartas[], int numerosDeLasCartas[], int linea);
void mostrarCartasJugador(int cartasJugador[], cadena paloDeLasCartas[], int numerosDeLasCartas[]);
void ordenarCartasJugador(int cartasJugador[], int cantidadCartas, int numerosDeLasCartas[]);
void intercambiarEnteros(int *primerValor, int *segundoValor);
int buscarCartaPorNumero(int numeroBuscado, int numerosDeLasCartas[], int cantidadCartas, int *posicionEncontrada);

int truco()
{
    int opcionMenu = 0;
    int mano = 1;
    char puntoSeleccionados;

    cadena palos[CANT_PALOS] = {0};
    int numerosDelTruco[CANT_NUMEROS] = {0};
    cadena paloDeLasCartas[CANT_CARTAS] = {0};
    int numerosDeLasCartas[CANT_CARTAS] = {0};
    int cartasUsadas[CANT_CARTAS] = {0};
    int cartasJugador[CANT_CARTAS_JUGADOR] = {0};
    int cartasDelCpu[CANT_CARTAS_JUGADOR] = {0};
    int mazo[CANT_PALOS][CANT_NUMEROS] = {0};

    inicializarPalos(palos);
    inicializarNumerosDelTruco(numerosDelTruco);
    inicializarCartasUsadas(cartasUsadas);

    while (opcionMenu != 2)
    {
        mostrarMenuDeInicio();
        opcionMenu = leerEnteroEntre(1, 3, "Ingrese una opcion: ");
        if (opcionMenu == 1)
        {
            puntoSeleccionados = obtenerEleccionDePuntos();
            Ronda(palos, numerosDelTruco, paloDeLasCartas, numerosDeLasCartas, cartasUsadas, mazo, cartasJugador, cartasDelCpu, mano);
            mano = obtenerSiguienteMano(mano);
        }
        else if (opcionMenu == 3)
        {
            mostrarInformacionDelTruco();
        }
        else
        {
            printf("\nSaliendo del juego...\n");
        }
    }
    return 0;
}

void mostrarMenuDeInicio()
{
    printf("\n========== TRUCO ==========\n");
    printf("1. Jugar\n");
    printf("2. Salir\n");
    printf("3. Informacion del Truco\n");
}

void mostrarEleccionDePuntos()
{
    printf("\nSeleccione a cuantos puntos desea jugar:\n");
    printf("A. 15 puntos\n");
    printf("B. 30 puntos\n");
}

char obtenerEleccionDePuntos()
{
    char opcion;
    mostrarEleccionDePuntos();
    do
    {
        opcion = leerCaracter("Ingrese A o B: ");
        opcion = toupper(opcion);
        if (opcion != 'A' && opcion != 'B')
        {
            printf("Opcion invalida. Ingrese A o B.\n");
        }
    } while (opcion != 'A' && opcion != 'B');

    return opcion;
}

void mostrarInformacionDelTruco()
{
    mostrarComoSeJuega();
    mostrarValoresDelTruco();
    mostrarValoresDelEnvido();
    mostrarEmpates();
}

void mostrarComoSeJuega()
{
    printf("\n========== COMO SE JUEGA ==========\n");
    printf("El Truco se juega con una baraja espanola de 40 cartas.\n");
    printf("Cada jugador recibe 3 cartas.\n");
}

void mostrarValoresDelTruco()
{
    printf("\n========== VALORES DEL TRUCO ==========\n");
    printf("La puntuacion depende de las jugadas realizadas.\n");
}

void mostrarValoresDelEnvido()
{
    printf("\n========== VALORES DEL ENVIDO ==========\n");
    printf("Los valores del envido se calculan utilizando las cartas recibidas.\n");
}

void mostrarEmpates()
{
    printf("\n========== EMPATES ==========\n");
    printf("En caso de empate se aplican las reglas correspondientes del Truco.\n");
}

void inicializarPalos(cadena palos[])
{
    strcpy(palos[0], "Espada");
    strcpy(palos[1], "Basto");
    strcpy(palos[2], "Oro");
    strcpy(palos[3], "Copa");
}

void inicializarNumerosDelTruco(int numerosDelTruco[])
{
    int i = 0;

    for (i = 0; i < CANT_NUMEROS; i++)
    {
        numerosDelTruco[i] = 0;
    }
    numerosDelTruco[0] = 1;
    numerosDelTruco[1] = 2;
    numerosDelTruco[2] = 3;
    numerosDelTruco[3] = 4;
    numerosDelTruco[4] = 5;
    numerosDelTruco[5] = 6;
    numerosDelTruco[6] = 7;
    numerosDelTruco[7] = 10;
    numerosDelTruco[8] = 11;
    numerosDelTruco[9] = 12;
}

void inicializarCartasUsadas(int cartasUsadas[])
{
    int i = 0;

    for (i = 0; i < CANT_CARTAS; i++)
    {
        cartasUsadas[i] = 0;
    }
}

void crearMazo(cadena palos[], int numerosDelTruco[], cadena paloDeLasCartas[], int numerosDeLasCartas[], int cartasUsadas[], int mazo[CANT_PALOS][CANT_NUMEROS])
{
    int fila = 0;
    int columna = 0;
    int posicion = 0;

    for (fila = 0; fila < CANT_PALOS; fila++)
    {
        for (columna = 0; columna < CANT_NUMEROS; columna++)
        {
            strcpy(paloDeLasCartas[posicion], palos[fila]);

            numerosDeLasCartas[posicion] = numerosDelTruco[columna];

            cartasUsadas[posicion] = 0;

            mazo[fila][columna] = posicion;

            posicion++;
        }
    }
}

void Ronda(cadena palos[], int numerosDelTruco[], cadena paloDeLasCartas[], int numerosDeLasCartas[], int cartasUsadas[], int mazo[CANT_PALOS][CANT_NUMEROS], int cartasJugador[], int cartasDelCpu[], int mano)
{
    crearMazo(palos, numerosDelTruco, paloDeLasCartas, numerosDeLasCartas, cartasUsadas, mazo);
    repartirCartas(cartasUsadas, cartasJugador, cartasDelCpu, mano);
    ordenarCartasJugador(cartasJugador, CANT_CARTAS_JUGADOR, numerosDeLasCartas);
    mostrarCartasJugador(cartasJugador, paloDeLasCartas, numerosDeLasCartas);
}

void repartirCartas(int cartasUsadas[], int cartasJugador[], int cartasDelCpu[], int mano)
{
    int i = 0;
    for (i = 0; i < CANT_CARTAS_JUGADOR; i++)
    {
        if (mano == 1)
        {
            cartasJugador[i] = i;
            cartasDelCpu[i] = i + CANT_CARTAS_JUGADOR;
        }
        else
        {
            cartasDelCpu[i] = i;
            cartasJugador[i] = i + CANT_CARTAS_JUGADOR;
        }
        cartasUsadas[cartasJugador[i]] = 1;
        cartasUsadas[cartasDelCpu[i]] = 1;
    }
}

int obtenerSiguienteMano(int mano)
{
    int siguienteMano;
    if (mano == 1)
    {
        siguienteMano = 2;
    }
    else
    {
        siguienteMano = 1;
    }
    return siguienteMano;
}

void dibujarCarta(int posicion, cadena paloDeLasCartas[], int numerosDeLasCartas[], int linea)
{
    if (linea == 1)
    {
        printf("+-------+ ");
    }
    else if (linea == 2)
    {
        printf("| %-5s | ", paloDeLasCartas[posicion]);
    }
    else if (linea == 3)
    {
        printf("|   %2d  | ", numerosDeLasCartas[posicion]);
    }
    else if (linea == 4)
    {
        printf("+-------+ ");
    }
}

void mostrarCartasJugador(int cartasJugador[], cadena paloDeLasCartas[], int numerosDeLasCartas[])
{
    int linea = 0;
    int carta = 0;

    printf("\n========== TUS CARTAS ==========\n");

    for (linea = 1; linea <= 4; linea++)
    {
        for (carta = 0; carta < CANT_CARTAS_JUGADOR; carta++)
        {
            dibujarCarta(cartasJugador[carta], paloDeLasCartas, numerosDeLasCartas, linea);
        }

        printf("\n");
    }
}

void ordenarCartasJugador(int cartasJugador[], int cantidadCartas, int numerosDeLasCartas[])
{
    int i = 0;
    int j = 0;

    for (i = 0; i < cantidadCartas - 1; i++)
    {
        for (j = i + 1; j < cantidadCartas; j++)
        {
            if (numerosDeLasCartas[cartasJugador[i]] > numerosDeLasCartas[cartasJugador[j]])
            {
                intercambiarEnteros(&cartasJugador[i], &cartasJugador[j]);
            }
        }
    }
}

void intercambiarEnteros(int *primerValor, int *segundoValor)
{
    int auxiliar = 0;

    auxiliar = *primerValor;
    *primerValor = *segundoValor;
    *segundoValor = auxiliar;
}

int buscarCartaPorNumero(int numeroBuscado, int numerosDeLasCartas[], int cantidadCartas, int *posicionEncontrada)
{
    int i = 0;
    int encontrada = 0;

    while (i < cantidadCartas && !encontrada)
    {
        if (numerosDeLasCartas[i] == numeroBuscado)
        {
            *posicionEncontrada = i;
            encontrada = 1;
        }

        i++;
    }

    return encontrada;
}
