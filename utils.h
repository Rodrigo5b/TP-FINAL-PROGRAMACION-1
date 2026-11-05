#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#endif
typedef char cadena[200];

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
void pausarSeguro();
void esperarEnter(const char *mensaje);
bool leerLinea(const char *mensaje, char *destino, size_t capacidad);
void esperarMilisegundos(int milisegundos);

int leerEntero(cadena mensaje) {
    int numero;
    printf("%s\n", mensaje);
    fflush(stdin);
    scanf("%d", &numero);
    return numero;
}

float leerFloat(cadena mensaje) {
    float numero;
    printf("%s:\n", mensaje);
    fflush(stdin);
    scanf("%f", &numero);
    return numero;
}

char leerCaracter(cadena mensaje) {
    char caracter;
    printf("%s: ", mensaje);
    fflush(stdin);
    scanf(" %c", &caracter);
    return caracter;
}

int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje) {
    int numero;
    numero = leerEntero(mensaje);
    while (numero < valorMin || numero > valorMax) {
        printf("Error. ");
        numero = leerEntero(mensaje);
    }
    return numero;
}

float leerFloatEntre(float valorMin, float valorMax, cadena mensaje) {
    float valor;
    valor = leerFloat(mensaje);
    while (valor < valorMin || valor > valorMax) {
        printf("Error. ");
        valor = leerFloat(mensaje);
    }
    return valor;   
}

bool confirmaUsuario(cadena mensaje) {
    char res;
    res = leerCaracter(mensaje);
    res = toupper(res);
    while (res != 'S' && res != 'N') {
        printf("Error.\n");
        res = leerCaracter(mensaje);
        res = toupper(res);
    }
    return res == 'S';
}

void mostrarVectorDeEnteros(int elVector[], int cant) {
    for (int i = 0; i < cant; i++) {
        printf("[%d]: %d\n", i, elVector[i]);   
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
    while (j < iActual && vector[j] != charBuscado) {
        j++;
    }
    return j < iActual;
}

void leerMatrizDeEnteros(cadena mensaje, int filas, int columnas, int matriz[filas][columnas]) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("[%d][%d] ", i, j);
            matriz[i][j] = leerEntero(mensaje);
        }
        printf("\n");
    }
}

void mostrarMatrizDeEnteros(int filas, int columnas, int matriz[filas][columnas]) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
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

void pausarSeguro() {
    int caracter;
    printf("\nPresione ENTER para continuar...");
    caracter = getchar();
    while (caracter != '\n' && caracter != EOF) {
        caracter = getchar();
    }
}

bool leerLinea(const char *mensaje, char *destino, size_t capacidad) {
    size_t longitud;
    int caracter;
    if (mensaje != NULL) printf("%s", mensaje);
    if (capacidad == 0 || fgets(destino, (int)capacidad, stdin) == NULL) return false;
    longitud = strlen(destino);
    if (longitud > 0 && destino[longitud - 1] == '\n') {
        destino[longitud - 1] = '\0';
    } else {
        caracter = getchar();
        while (caracter != '\n' && caracter != EOF) caracter = getchar();
    }
    longitud = strlen(destino);
    if (longitud > 0 && destino[longitud - 1] == '\r') destino[longitud - 1] = '\0';
    return true;
}

void esperarEnter(const char *mensaje) {
    char linea[8];
    (void)leerLinea(mensaje, linea, sizeof linea);
}

void esperarMilisegundos(int milisegundos) {
#ifdef _WIN32
    Sleep((DWORD)milisegundos);
#else
    usleep((useconds_t)milisegundos * 1000);
#endif
}

#endif
