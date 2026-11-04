#include "utils.h"
#include "programas/BatallaNaval/batalla_naval.c"
#include "programas/Ludo/ludo.c"
#include "programas/Truco/TrucoFinal.c"

#define ARCADE_MAX_PERFILES 100
#define ARCADE_LONGITUD_NOMBRE 64
#define ARCADE_CANT_JUEGOS 3
#define ARCADE_DATOS "perfiles.dat"
#define ARCADE_TEMPORAL "perfiles.tmp"
#define ARCADE_REPORTE "reporte_perfiles.txt"
#define ARCADE_VERSION_ARCHIVO 1U

enum { ARCADE_TRUCO = 0, ARCADE_BATALLA = 1, ARCADE_LUDO = 2 };

typedef struct {
    int partidas;
    int victorias;
    int derrotas;
} EstadisticasPerfil;

typedef struct {
    int id;
    char nombre[ARCADE_LONGITUD_NOMBRE];
    EstadisticasPerfil estadisticas[ARCADE_CANT_JUEGOS];
} PerfilJugador;

typedef struct {
    PerfilJugador perfiles[ARCADE_MAX_PERFILES];
    int cantidad;
    int proximoId;
} RegistroPerfiles;

typedef enum {
    CARGA_NUEVA,
    CARGA_CORRECTA,
    CARGA_ERROR
} EstadoCargaPerfiles;

// Prototipos
void menuInicio(void);
void menuJuegos(void);
int ejecutaTruco(void);
int ejecutaLudo(void);
int ejecutaBatallaNaval(void);
void inicializarPerfiles(RegistroPerfiles *registro);
int buscarPerfilPorId(const RegistroPerfiles *registro, int id);
int buscarPerfilPorNombre(const RegistroPerfiles *registro, const char *nombre);
bool validarPerfil(const PerfilJugador *perfil);
void mostrarPerfil(const PerfilJugador *perfil);
void listarPerfiles(const RegistroPerfiles *registro);
bool leerNombrePerfil(char nombre[ARCADE_LONGITUD_NOMBRE]);
void registrarPerfil(RegistroPerfiles *registro);
void consultarPerfil(const RegistroPerfiles *registro);
void editarPerfil(RegistroPerfiles *registro);
void eliminarPerfil(RegistroPerfiles *registro);
void registrarResultadoEnPerfil(RegistroPerfiles *registro, int id, int juego, bool victoria);
void cargarResultadoManual(RegistroPerfiles *registro);
void mostrarMenuPerfiles(void);
void ordenarPerfilesPorNombre(PerfilJugador perfiles[], int cantidad);
void listarPerfilesOrdenados(const RegistroPerfiles *registro);
void mostrarEstadisticas(const RegistroPerfiles *registro);
void mostrarMenuEstadisticas(void);
void gestionarPerfiles(RegistroPerfiles *registro);
bool exportarReportePerfiles(const RegistroPerfiles *registro);
EstadoCargaPerfiles cargarPerfiles(RegistroPerfiles *registro);
bool guardarPerfiles(const RegistroPerfiles *registro);
void mostrarInicios(const int inicios[3]);
void ejecutarMenuEstadisticas(const RegistroPerfiles *registro, const int inicios[3]);
void gestionarJuegos(RegistroPerfiles *registro, int iniciosJuego[3]);

int main(void) {
    RegistroPerfiles registro;
    inicializarPerfiles(&registro);
    EstadoCargaPerfiles estadoCarga = cargarPerfiles(&registro);
    bool puedeGuardar = (estadoCarga != CARGA_ERROR);
    int opcion = 0;
    int iniciosJuego[3] = {0}; 
    srand((unsigned int)time(NULL));

    if (estadoCarga == CARGA_CORRECTA) {
        printf("Se recuperaron %d jugadores desde %s.\n", registro.cantidad, ARCADE_DATOS);
    } else {
        if (estadoCarga == CARGA_NUEVA) {
            printf("Primer inicio: no hay registros previos.\n");
        } else {
            printf("ADVERTENCIA: los datos de disco no se modificaran en esta sesion.\n");
        }
    }
    pausarSeguro();

    while (opcion != 4) {
        menuInicio();
        opcion = leerEnteroEntre(1, 4, "Seleccione la opcion que desee ejecutar: ");
        
        if (opcion == 1) {
            gestionarJuegos(&registro, iniciosJuego);
        } else {
            if (opcion == 2) {
                ejecutarMenuEstadisticas(&registro, iniciosJuego);
            } else {
                if (opcion == 3) {
                    gestionarPerfiles(&registro);
                } else {
                    if (opcion == 4) {
                        printf("Saliendo de la aplicacion...\n");
                    }
                }
            }
        }
    }

    if (puedeGuardar) {
        if (guardarPerfiles(&registro)) {
            printf("Datos guardados correctamente en %s.\n", ARCADE_DATOS);
        } else {
            fprintf(stderr, "No se pudieron guardar los datos. Revise los permisos y el disco.\n");
        }
    } else {
        printf("No se guardo: se protege el archivo original con errores.\n");
    }
    printf("Hasta luego.\n");
    return 0;
}

void menuInicio(void) {
    limpiarPantalla();
    printf(" _______    ______   __        __               ___            _______   ________   ______   __        \n");
    printf("|       |  /      | |  |      |  |             /   |          |       | |        | /      | |  |  \n");
    printf("| $$$$$$$||  $$$$$$|| $$      | $$            |  $$$|         | $$$$$$$|| $$$$$$$$|  $$$$$$|| $$ \n");
    printf("| $$__| $$| $$  | $$| $$      | $$             |$$  $$__      | $$  | $$| $$__    | $$__| $$| $$ \n");
    printf("| $$    $$| $$  | $$| $$      | $$            | |$$$/  |      | $$  | $$| $$  |   | $$    $$| $$ \n");
    printf("| $$$$$$$|| $$  | $$| $$      | $$            | $$|$$ $$      | $$  | $$| $$$$$   | $$$$$$$$| $$ \n");
    printf("| $$  | $$| $$__/ $$| $$_____ | $$_____       | $$_|$$|       | $$__/ $$| $$_____ | $$  | $$| $$_____ \n");
    printf("| $$  | $$ |$$    $$| $$     || $$     |       |$$  |$$|      | $$    $$| $$     || $$  | $$| $$     \\\n");
    printf(" |$$   |$$  |$$$$$$  |$$$$$$$$ |$$$$$$$$        |$$$$ $$       |$$$$$$$  |$$$$$$$$ |$$   |$$ |$$$$$$$$\n");
    printf("\n\n\n");
    printf("                        1                         \n");
    printf("                   -------------                  \n");
    printf("                  |    INICIO   |                  \n");
    printf("                   -------------                  \n");
    printf("\n");
    printf("                        2                         \n");
    printf("                   -------------                  \n");
    printf("                  | ESTADISTICAS |                  \n");
    printf("                   -------------                  \n");
    printf("\n");
    printf("                        3                         \n");
    printf("                   -------------                  \n");
    printf("                  |   USUARIOS  |                  \n");
    printf("                   -------------                  \n");
    printf("\n");
    printf("                        4                         \n");
    printf("                   -------------                  \n");
    printf("                  |    SALIR    |                  \n");
    printf("                   -------------                  \n");
    printf("\n");  
}

void menuJuegos(void) {
    limpiarPantalla();
     printf ("     _____  \n");
    printf ("    |      |\n");
    printf ("     |$$$$$ __    __   ______    ______    ______    _______ \n");
    printf ("       | $$|  |  |  | /      |  /      |  /      |  /       | \n");
    printf ("  __   | $$| $$  | $$|  $$$$$$||  $$$$$$||  $$$$$$||  $$$$$$$ \n");
    printf (" |  |  | $$| $$  | $$| $$    $$| $$  | $$| $$  | $$ |$$    | \n");
    printf (" | $$__| $$| $$__/ $$| $$$$$$$$| $$__| $$| $$__/ $$ _|$$$$$$| \n");
    printf ("  |$$    $$ |$$    $$ |$$     | |$$    $$ |$$    $$|       $$ \n");
    printf ("   |$$$$$$   |$$$$$$   |$$$$$$$ _|$$$$$$$  |$$$$$$  |$$$$$$$ \n");
    printf ("                                |  |__| $$ \n");
    printf ("                                |$$    $$ \n");
    printf ("                                 |$$$$$$ \n");
    printf("\n\n");
    printf("                        1                         \n");
    printf("                   -------------                  \n");
    printf("                  |    TRUCO    |                  \n");
    printf("                   -------------                  \n");
    printf("\n");
    printf("                        2                         \n"); 
    printf("                   -------------                  \n");
    printf("                  |     LUDO    |                  \n");
    printf("                   -------------                  \n");
    printf("\n");
    printf("                        3                         \n"); 
    printf("                   --------------                  \n");
    printf("                  |    BATALLA   |                 \n");
    printf("                  |     NAVAL    |                 \n");
    printf("                   --------------                  \n");
    printf("\n");
    printf("                        4                         \n"); 
    printf("                   -------------                  \n");
    printf("                  |   VOLVER    |                  \n");
    printf("                   -------------                  \n");
    printf("\n");  
}

int ejecutaTruco(void) {
    return truco();
}

int ejecutaLudo(void) {
    return ludo();
}

int ejecutaBatallaNaval(void) {
    return batalla_Naval();
}

void gestionarJuegos(RegistroPerfiles *registro, int iniciosJuego[3]) {
    int opcionJuego = 0;
    while (opcionJuego != 4) {
        menuJuegos();
        opcionJuego = leerEnteroEntre(1, 4, "Seleccione una opcion de juego: ");
        if (opcionJuego == 1) {
            iniciosJuego[ARCADE_TRUCO]++;
            ejecutaTruco();
            if (registro->cantidad > 0) {
                if (confirmaUsuario("Desea registrar el resultado en un perfil?")) {
                    cargarResultadoManual(registro);
                }
            }
        } else {
            if (opcionJuego == 2) {
                iniciosJuego[ARCADE_LUDO]++;
                ejecutaLudo();
                if (registro->cantidad > 0) {
                    if (confirmaUsuario("Desea registrar el resultado en un perfil?")) {
                        cargarResultadoManual(registro);
                    }
                }
            } else {
                if (opcionJuego == 3) {
                    iniciosJuego[ARCADE_BATALLA]++;
                    ejecutaBatallaNaval();
                    if (registro->cantidad > 0) {
                        if (confirmaUsuario("Desea registrar el resultado en un perfil?")) {
                            cargarResultadoManual(registro);
                        }
                    }
                } else {
                    if (opcionJuego == 4) {
                        printf("Volviendo al menu principal...\n");
                    }
                }
            }
        }
        pausarSeguro();
    }
}

void inicializarPerfiles(RegistroPerfiles *registro) {
    memset(registro, 0, sizeof(*registro));
    registro->proximoId = 1;
}

int buscarPerfilPorId(const RegistroPerfiles *registro, int id) {
    int indice = 0;
    while (indice < registro->cantidad && registro->perfiles[indice].id != id) {
        indice++;
    }
    return indice < registro->cantidad ? indice : -1;
}

int buscarPerfilPorNombre(const RegistroPerfiles *registro, const char *nombre) {
    int indice = 0;
    while (indice < registro->cantidad && strcmp(registro->perfiles[indice].nombre, nombre) != 0) {
        indice++;
    }
    return indice < registro->cantidad ? indice : -1;
}

bool validarPerfil(const PerfilJugador *perfil) {
    bool valido = perfil->id > 0 && memchr(perfil->nombre, '\0', sizeof perfil->nombre) != NULL;
    if (valido) {
        valido = perfil->nombre[0] != '\0';
    }
    for (int i = 0; i < ARCADE_CANT_JUEGOS; i++) {
        if (perfil->estadisticas[i].partidas < 0 || perfil->estadisticas[i].victorias < 0 ||
            perfil->estadisticas[i].derrotas < 0 ||
            perfil->estadisticas[i].victorias > perfil->estadisticas[i].partidas ||
            perfil->estadisticas[i].derrotas != perfil->estadisticas[i].partidas - perfil->estadisticas[i].victorias) {
            valido = false;
        }
    }
    return valido;
}

void mostrarPerfil(const PerfilJugador *perfil) {
    printf("ID: %-4d Nombre: %s\n", perfil->id, perfil->nombre);
    const char *juegos[ARCADE_CANT_JUEGOS] = {"Truco", "Batalla Naval", "Ludo"};
    for (int j = 0; j < ARCADE_CANT_JUEGOS; j++) {
        printf("  %-13s Partidas: %-3d Ganadas: %-3d Perdidas: %d\n", juegos[j],
               perfil->estadisticas[j].partidas,
               perfil->estadisticas[j].victorias,
               perfil->estadisticas[j].derrotas);
    }
}

void listarPerfiles(const RegistroPerfiles *registro) {
    printf("\n===== JUGADORES REGISTRADOS (%d) =====\n", registro->cantidad);
    if (registro->cantidad == 0) {
        printf("Aun no hay jugadores registrados.\n");
    }
    for (int i = 0; i < registro->cantidad; i++) {
        mostrarPerfil(&registro->perfiles[i]);
    }
}

bool leerNombrePerfil(char nombre[ARCADE_LONGITUD_NOMBRE]) {
    char entrada[200];
    while (true) {
        if (!leerLinea("Nombre del jugador: ", entrada, sizeof entrada)) return false;
        size_t longitud = strlen(entrada);
        if (longitud == 0) {
            printf("El nombre no puede estar vacio.\n");
        } else if (longitud >= ARCADE_LONGITUD_NOMBRE) {
            printf("El nombre no puede superar %d caracteres.\n", ARCADE_LONGITUD_NOMBRE - 1);
        } else {
            strcpy(nombre, entrada);
            return true;
        }
    }
}

void registrarPerfil(RegistroPerfiles *registro) {
    PerfilJugador perfil = {0};
    if (registro->cantidad >= ARCADE_MAX_PERFILES || registro->proximoId == INT_MAX) {
        printf("No se pueden registrar mas jugadores.\n");
        return;
    }
    if (!leerNombrePerfil(perfil.nombre)) return;
    if (buscarPerfilPorNombre(registro, perfil.nombre) >= 0) {
        printf("Ya existe un jugador con ese nombre.\n");
        return;
    }
    perfil.id = registro->proximoId;
    registro->perfiles[registro->cantidad] = perfil;
    registro->cantidad++;
    registro->proximoId++;
    printf("Jugador registrado con ID %d.\n", perfil.id);
}

void consultarPerfil(const RegistroPerfiles *registro) {
    int id = leerEntero("ID a buscar: ");
    int indice = buscarPerfilPorId(registro, id);
    if (indice >= 0) {
        mostrarPerfil(&registro->perfiles[indice]);
    } else {
        printf("No existe un jugador con ese ID.\n");
    }
}

void editarPerfil(RegistroPerfiles *registro) {
    int id = leerEntero("ID del jugador a modificar: ");
    int indice = buscarPerfilPorId(registro, id);
    char nombreNuevo[ARCADE_LONGITUD_NOMBRE];
    if (indice < 0) {
        printf("Jugador inexistente.\n");
        return;
    }
    mostrarPerfil(&registro->perfiles[indice]);
    if (!leerNombrePerfil(nombreNuevo)) return;
    int repetido = buscarPerfilPorNombre(registro, nombreNuevo);
    if (repetido >= 0 && repetido != indice) {
        printf("Ese nombre ya esta registrado.\n");
        return;
    }
    strcpy(registro->perfiles[indice].nombre, nombreNuevo);
    printf("Perfil actualizado.\n");
}

void eliminarPerfil(RegistroPerfiles *registro) {
    int id = leerEntero("ID del jugador a eliminar: ");
    int indice = buscarPerfilPorId(registro, id);
    if (indice < 0) {
        printf("Jugador inexistente.\n");
        return;
    }
    mostrarPerfil(&registro->perfiles[indice]);
    if (!confirmaUsuario("Confirma la eliminacion?")) return;
    for (int i = indice; i < registro->cantidad - 1; i++) {
        registro->perfiles[i] = registro->perfiles[i + 1];
    }
    registro->cantidad--;
    memset(&registro->perfiles[registro->cantidad], 0, sizeof(PerfilJugador));
    printf("Perfil eliminado.\n");
}

void registrarResultadoEnPerfil(RegistroPerfiles *registro, int id, int juego, bool victoria) {
    int indice = buscarPerfilPorId(registro, id);
    if (indice < 0 || juego < 0 || juego >= ARCADE_CANT_JUEGOS) {
        printf("Jugador inexistente o juego invalido.\n");
        return;
    }
    EstadisticasPerfil *estadisticas = &registro->perfiles[indice].estadisticas[juego];
    if (estadisticas->partidas == INT_MAX || (victoria && estadisticas->victorias == INT_MAX) ||
        (!victoria && estadisticas->derrotas == INT_MAX)) {
        printf("No se puede incrementar el contador de partidas.\n");
        return;
    }
    estadisticas->partidas++;
    if (victoria) {
        estadisticas->victorias++;
    } else {
        estadisticas->derrotas++;
    }
    printf("Resultado asociado al jugador %s.\n", registro->perfiles[indice].nombre);
}

void cargarResultadoManual(RegistroPerfiles *registro) {
    int id = leerEntero("ID del jugador: ");
    if (buscarPerfilPorId(registro, id) < 0) {
        printf("Jugador inexistente.\n");
        return;
    }
    printf("1. Truco\n2. Batalla Naval\n3. Ludo\n");
    int juego = leerEnteroEntre(1, 3, "Juego: ") - 1;
    bool gano = confirmaUsuario("Gano la partida?");
    registrarResultadoEnPerfil(registro, id, juego, gano);
}

void mostrarMenuPerfiles(void) {
    printf("\n========== GESTION DE JUGADORES ==========\n");
    printf("1. Registrar jugador\n");
    printf("2. Listar jugadores\n");
    printf("3. Buscar jugador por ID\n");
    printf("4. Modificar nombre\n");
    printf("5. Eliminar jugador\n");
    printf("6. Registrar resultado manual\n");
    printf("7. Listado ordenado por nombre\n");
    printf("8. Exportar reporte de texto\n");
    printf("9. Volver\n");
}

void ordenarPerfilesPorNombre(PerfilJugador perfiles[], int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        int menor = i;
        for (int j = i + 1; j < cantidad; j++) {
            if (strcmp(perfiles[j].nombre, perfiles[menor].nombre) < 0) {
                menor = j;
            }
        }
        if (menor != i) {
            PerfilJugador auxiliar = perfiles[i];
            perfiles[i] = perfiles[menor];
            perfiles[menor] = auxiliar;
        }
    }
}

void listarPerfilesOrdenados(const RegistroPerfiles *registro) {
    PerfilJugador copia[ARCADE_MAX_PERFILES];
    for (int i = 0; i < registro->cantidad; i++) {
        copia[i] = registro->perfiles[i];
    }
    ordenarPerfilesPorNombre(copia, registro->cantidad);
    printf("\n===== JUGADORES EN ORDEN ALFABETICO =====\n");
    for (int i = 0; i < registro->cantidad; i++) {
        mostrarPerfil(&copia[i]);
    }
}

void mostrarEstadisticas(const RegistroPerfiles *registro) {
    long long partidas[ARCADE_CANT_JUEGOS] = {0};
    long long victorias[ARCADE_CANT_JUEGOS] = {0};
    const char *juegos[ARCADE_CANT_JUEGOS] = {"Truco", "Batalla Naval", "Ludo"};
    for (int i = 0; i < registro->cantidad; i++) {
        for (int j = 0; j < ARCADE_CANT_JUEGOS; j++) {
            partidas[j] += registro->perfiles[i].estadisticas[j].partidas;
            victorias[j] += registro->perfiles[i].estadisticas[j].victorias;
        }
    }
    printf("\n========= ESTADISTICAS =========\n");
    printf("Perfiles registrados: %d\n", registro->cantidad);
    for (int j = 0; j < ARCADE_CANT_JUEGOS; j++) {
#ifdef _WIN32
        printf("%-13s | Participaciones: %-5I64d Victorias: %I64d\n", juegos[j], partidas[j], victorias[j]);
#else
        printf("%-13s | Participaciones: %-5lld Victorias: %lld\n", juegos[j], partidas[j], victorias[j]);
#endif
    }
}

void mostrarMenuEstadisticas(void) {
    printf("\n========= ESTADISTICAS =========\n");
    printf("1. Mostrar resumen\n2. Exportar reporte .txt\n3. Volver\n");
}

void gestionarPerfiles(RegistroPerfiles *registro) {
    int opcion = 0;
    while (opcion != 9) {
        mostrarMenuPerfiles();
        opcion = leerEnteroEntre(1, 9, "Seleccione una opcion: ");
        
        if (opcion == 1) {
            registrarPerfil(registro);
        } else {
            if (opcion == 2) {
                listarPerfiles(registro);
            } else {
                if (opcion == 3) {
                    consultarPerfil(registro);
                } else {
                    if (opcion == 4) {
                        editarPerfil(registro);
                    } else {
                        if (opcion == 5) {
                            eliminarPerfil(registro);
                        } else {
                            if (opcion == 6) {
                                cargarResultadoManual(registro);
                            } else {
                                if (opcion == 7) {
                                    listarPerfilesOrdenados(registro);
                                } else {
                                    if (opcion == 8) {
                                        if (exportarReportePerfiles(registro)) {
                                            printf("Reporte exportado: %s\n", ARCADE_REPORTE);
                                        }
                                    } else {
                                        if (opcion == 9) {
                                            printf("Volviendo...\n");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        pausarSeguro();
    }
}

EstadoCargaPerfiles cargarPerfiles(RegistroPerfiles *registro) {
    FILE *archivo = fopen(ARCADE_DATOS, "rb");
    if (archivo == NULL) {
        if (errno == ENOENT) return CARGA_NUEVA;
        perror("No se pudo abrir perfiles.dat");
        return CARGA_ERROR;
    }
    const char firmaEsperada[4] = {'F','S','T','L'};
    char firma[4];
    uint32_t version = 0;
    int cantidad = 0;
    int proximoId = 0;
    RegistroPerfiles temporal;
    inicializarPerfiles(&temporal);
    
    bool correcto = (fread(firma, sizeof firma, 1, archivo) == 1);
    if (correcto) correcto = (memcmp(firma, firmaEsperada, sizeof firma) == 0);
    if (correcto) correcto = (fread(&version, sizeof version, 1, archivo) == 1 && version == ARCADE_VERSION_ARCHIVO);
    if (correcto) correcto = (fread(&cantidad, sizeof cantidad, 1, archivo) == 1 && cantidad >= 0 && cantidad <= ARCADE_MAX_PERFILES);
    if (correcto) correcto = (fread(&proximoId, sizeof proximoId, 1, archivo) == 1 && proximoId > 0);
    if (correcto && cantidad > 0) {
        correcto = (fread(temporal.perfiles, sizeof(PerfilJugador), (size_t)cantidad, archivo) == (size_t)cantidad);
    }
    if (correcto) correcto = (fgetc(archivo) == EOF && !ferror(archivo));
    if (fclose(archivo) == EOF) correcto = false;
    
    if (correcto) {
        int maximoId = 0;
        for (int i = 0; i < cantidad; i++) {
            if (!validarPerfil(&temporal.perfiles[i])) {
                correcto = false;
            }
            if (temporal.perfiles[i].id > maximoId) {
                maximoId = temporal.perfiles[i].id;
            }
            for (int j = 0; j < i; j++) {
                if (temporal.perfiles[i].id == temporal.perfiles[j].id ||
                    strncmp(temporal.perfiles[i].nombre, temporal.perfiles[j].nombre, ARCADE_LONGITUD_NOMBRE) == 0) {
                    correcto = false;
                }
            }
        }
        if (proximoId <= maximoId) correcto = false;
    }
    
    if (!correcto) {
        fprintf(stderr, "Archivo %s invalido, truncado o incompatible. No se sobrescribira.\n", ARCADE_DATOS);
        return CARGA_ERROR;
    }
    temporal.cantidad = cantidad;
    temporal.proximoId = proximoId;
    *registro = temporal;
    return CARGA_CORRECTA;
}

bool guardarPerfiles(const RegistroPerfiles *registro) {
    FILE *archivo = fopen(ARCADE_TEMPORAL, "wb");
    if (archivo == NULL) {
        perror("No se pudo crear el archivo temporal");
        return false;
    }
    const char firma[4] = {'F','S','T','L'};
    uint32_t version = ARCADE_VERSION_ARCHIVO;
    bool correcto = (fwrite(firma, sizeof firma, 1, archivo) == 1);
    if (correcto) correcto = (fwrite(&version, sizeof version, 1, archivo) == 1);
    if (correcto) correcto = (fwrite(&registro->cantidad, sizeof registro->cantidad, 1, archivo) == 1);
    if (correcto) correcto = (fwrite(&registro->proximoId, sizeof registro->proximoId, 1, archivo) == 1);
    if (correcto && registro->cantidad > 0) {
        correcto = (fwrite(registro->perfiles, sizeof(PerfilJugador), (size_t)registro->cantidad, archivo) == (size_t)registro->cantidad);
    }
    if (fclose(archivo) == EOF) correcto = false;
    if (correcto) {
#ifdef _WIN32
        correcto = (MoveFileExA(ARCADE_TEMPORAL, ARCADE_DATOS,
                              MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0);
#else
        correcto = (rename(ARCADE_TEMPORAL, ARCADE_DATOS) == 0);
#endif
    }
    if (!correcto) {
        perror("No se pudieron guardar los perfiles");
        remove(ARCADE_TEMPORAL);
    }
    return correcto;
}

bool exportarReportePerfiles(const RegistroPerfiles *registro) {
    PerfilJugador copia[ARCADE_MAX_PERFILES];
    for (int i = 0; i < registro->cantidad; i++) {
        copia[i] = registro->perfiles[i];
    }
    ordenarPerfilesPorNombre(copia, registro->cantidad);
    FILE *archivo = fopen(ARCADE_REPORTE, "w");
    if (archivo == NULL) {
        perror("No se pudo crear el reporte");
        return false;
    }
    bool correcto = (fprintf(archivo, "FESTIVAL - REPORTE DE PERFILES\nOrden: alfabetico por nombre\n\n") >= 0);
    if (correcto) {
        correcto = (fprintf(archivo, "ID   NOMBRE                       TRUCO (P/G)  BATALLA (P/G)  LUDO (P/G)\n") >= 0);
    }
    for (int i = 0; i < registro->cantidad && correcto; i++) {
        const PerfilJugador *p = &copia[i];
        correcto = (fprintf(archivo, "%-4d %-28.28s %4d/%-4d      %4d/%-4d       %4d/%-4d\n",
                           p->id, p->nombre,
                           p->estadisticas[ARCADE_TRUCO].partidas, p->estadisticas[ARCADE_TRUCO].victorias,
                           p->estadisticas[ARCADE_BATALLA].partidas, p->estadisticas[ARCADE_BATALLA].victorias,
                           p->estadisticas[ARCADE_LUDO].partidas, p->estadisticas[ARCADE_LUDO].victorias) >= 0);
    }
    if (correcto) {
        correcto = (fprintf(archivo, "\nTotal de perfiles: %d\n", registro->cantidad) >= 0);
    }
    if (fclose(archivo) == EOF) {
        correcto = false;
    }
    if (!correcto) {
        fprintf(stderr, "No se pudo completar el reporte de texto.\n");
    }
    return correcto;
}

void mostrarInicios(const int inicios[3]) {
    printf("\n========= ESTADISTICAS DE ESTA SESION =========\n");
    printf("Partidas iniciadas: %d\n", inicios[0] + inicios[1] + inicios[2]);
    printf("Truco: %d | Ludo: %d | Batalla Naval: %d\n", inicios[0], inicios[1], inicios[2]);
    printf("Nota: se cuentan inicios del juego, no victorias.\n");
}

void ejecutarMenuEstadisticas(const RegistroPerfiles *registro, const int inicios[3]) {
    int opcion = 0;
    while (opcion != 3) {
        mostrarMenuEstadisticas();
        opcion = leerEnteroEntre(1, 3, "Seleccione una opcion: ");
        if (opcion == 1) {
            mostrarInicios(inicios);
            mostrarEstadisticas(registro);
        } else {
            if (opcion == 2) {
                if (exportarReportePerfiles(registro)) {
                    printf("Reporte guardado en %s.\n", ARCADE_REPORTE);
                }
            } else {
                if (opcion == 3) {
                    printf("Volviendo...\n");
                }
            }
        }
        pausarSeguro();
    }
}
