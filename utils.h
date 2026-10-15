#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Para usar el tipo de dato bool
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <string.h>
typedef char cadena[200]; // Para tratar a los arrays de chars como 'cadena'

///////////////////////////////////////////////////////////////////////////
////////////////////////////  PROTOTIPOS  /////////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(cadena mensaje);
float leerFloat(cadena mensaje);
char leerCaracter(cadena mensaje);
int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje);
float leerFloatEntre(float valorMin, float valorMax, cadena mensaje);
bool confirmaUsuario(cadena mensaje);
void mostrarVectorDeEnteros(int elVector[], int cant);
void leerVectorDeEnteros(cadena mensaje, int vec[], int cant);
int calcSumatoria(int vecEnteros[], int cant);
bool yaSalioAntes(char vector[], int iActual);
void leerMatrizDeEnteros(cadena mensaje, int filas, int columnas, int matriz[filas][columnas]);
void mostrarMatrizDeEnteros(int filas, int columnas, int matriz[filas][columnas]);
void limpiarPantalla();
///////////////////////////////////////////////////////////////////////////
/////////////////////////  IMPLEMENTACIONES  //////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un entero y lo retorna */
    int numero;
    printf("%s\n", mensaje);
    fflush(stdin);
    scanf("%d", &numero);
    return numero;
}
float leerFloat(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un float y lo retorna */
    // ¡Esta va resuelta de regalo! 😉
    float numero;
    printf("%s:\n", mensaje);
    fflush(stdin); // No es obligatorio pero por las dudas...
    scanf("%f", &numero);
    return numero;
}

char leerCaracter(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un char y lo retorna */
    char caracter;
    printf("%s: ", mensaje);
    fflush(stdin);
    scanf(" %c", &caracter);
    return caracter;
}

int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje) {
    /* Muestra el mensaje al usuario. Luego lee enteros mientras éstos estén
       fuera del rango [valorMin, valorMax] informando al usuario de
       su error. Cuando finalmente lea un entero válido, lo retorna */
       int numero;
    numero=leerEntero(mensaje);
    while(numero<valorMin || numero>valorMax)
    {
    printf("Error. ");
    numero=leerEntero(mensaje);
    }
    return numero;
}

float leerFloatEntre(float valorMin, float valorMax, cadena mensaje) {
    /* Muestra el mensaje al usuario. Luego, lee floats mientras éstos estén
       fuera del rango [valorMin, valorMax] informando al usuario de
       su error. Cuando finalmente lea un float válido, lo retorna */
    float valor;
    valor=leerFloat(mensaje);
    while(valor<valorMin || valor>valorMax)
    {
    printf("Error. ");
    valor=leerFloat(mensaje);
    }
    return valor;   
}

bool confirmaUsuario(cadena mensaje) {
    /* Muestra el mensaje al usuario junto a la leyenda "[S/N]" que
       representa "Si" o "No". Luego, lee chars mientras éstos no sean los
       previstos ('S' o 'N', incluyendo minúsculas) informando al
       usuario de su error. Retorna si el usuario seleccionó que sí. */
    char res;
    res = leerCaracter(mensaje);
    res = toupper(res);
    while (res != 'S' && res != 'N')
    {
        printf("Eror.\n");
        res=leerCaracter(mensaje);
        res=toupper(res);
    }
    return res == 'S';
}
void mostrarVectorDeEnteros (int elVector[], int cant) {
    for (int i = 0; i < cant; i++) {
        printf("[%d]: %d\n",i, elVector[i]);   
    }
}

void leerVectorDeEnteros(cadena mensaje, int vec[], int cant) {
    for (int i = 0; i < cant; i++) {
        printf("[%d] ", i);
        vec[i] = leerEntero(mensaje);
    }
}

int calcSumatoria(int vecEnteros[], int cant) {
    int acu = 0;
    for (int i = 0; i < cant; i++) {
        acu += vecEnteros[i];
    }
    return acu;
}

bool yaSalioAntes(char vector[], int iActual) {
    char charBuscado = vector[iActual];
    int j = 0;
    while( j < iActual && vector[j] != charBuscado ) {
        j++;
    }
    return j < iActual;
}

void leerMatrizDeEnteros(cadena mensaje, int filas, int columnas, int matriz[filas][columnas])
{
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            printf("[%d][%d] ", i, j);
            matriz[i][j] = leerEntero(mensaje);
        }
        printf("\n");
    }
}

void mostrarMatrizDeEnteros(int filas, int columnas, int matriz[filas][columnas])
{
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }
}
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}