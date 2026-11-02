#include "utils.h"
#include "programas/BatallaNaval/batalla_naval.c"
#include "programas/Ludo/ludo.c"
#include "programas/Truco/TrucoFinal.c"
void menuInicio();
void MenuJuegos();
int eleccion();
int JuegaTruco();
int JuegaLudo();
int JuegaBatallaNaval();
int main()
{
    int juego, accion;
    do
    {
        menuInicio();
        accion = eleccion();
        switch (accion)
              {
        case 1:
            MenuJuegos();
            juego = eleccion();
            switch (juego)
            {
            case 1:
                JuegaTruco();
                break;
            case 2:
                JuegaBatallaNaval();
                break;
            case 3:
                JuegaLudo();
                break;
            default:
                printf("Saliendo...");
            }
            break;
        case 2:
            //estadisticas archivo de texto
        case 3:
            //usuarios archivo binario

        default:
            printf("Saliendo...");  
            }
    } while (accion != 4);

return 0;
}
void menuInicio()
{
    printf("   ________          __            __           __     __  \n");
    printf("  |        \        |  \          |  \         |  \   |  \  \n");
    printf("   \$$$$$$$$______   \$$  ______  | $$  ______ | $$   | $$  ______    ______    _______   ______  \n");
    printf("     | $$  /      \ |  \ /      \ | $$ /      \| $$   | $$ /      \  /      \  /       \ /      \ \n");
    printf("     | $$ |  $$$$$$\| $$|  $$$$$$\| $$|  $$$$$$\\$$\ /  $$|  $$$$$$\|  $$$$$$\|  $$$$$$$|  $$$$$$\ \n");
    printf("     | $$ | $$   \$$| $$| $$  | $$| $$| $$    $$ \$$\  $$ | $$    $$| $$   \$$ \$$    \ | $$    $$ \n");
    printf("     | $$ | $$      | $$| $$__/ $$| $$| $$$$$$$$  \$$ $$  | $$$$$$$$| $$       _\$$$$$$\| $$$$$$$$ \n");
    printf("     | $$ | $$      | $$| $$    $$| $$ \$$     \   \$$$    \$$     \| $$      |       $$ \$$     \ \n");
    printf("     | $$ | $$      | $$| $$    $$| $$ \$$     \   \$$$    \$$     \| $$      |       $$ \$$     \ \n");
    printf("      \$$  \$$       \$$| $$$$$$$  \$$  \$$$$$$$    \$      \$$$$$$$ \$$       \$$$$$$$   \$$$$$$$ \n");
    printf("                        | $$ \n");
    printf("                        | $$ \n");
    printf("                         \$$ \n");
    printf("\n");
	printf("\n");
	printf ( "                        1                         \n");
	printf ("                   -------------                  \n");
	printf ("                  |    INICIO   |                  \n");
	printf ("                   -------------                  \n");
	printf("\n");
	Escribir "                        2                         " 
	printf ("                   -------------                  \n");
	printf ("                  | ESTADISTICAS |                  \n");
	printf ("                   -------------                  \n");
	printf("\n");
	Escribir "                        3                         " 
	printf ("                   -------------                  \n");
	printf ("                  |   USUARIOS  |                  \n");
	printf ("                   -------------                  \n");
	printf("\n");
	Escribir "                        4                         " 
	printf ("                   -------------                  \n");
	printf ("                  |    SALIR    |                  \n");
	printf ("                   -------------                  \n");
	printf("\n");  
}
int eleccion()
{
    int res;
    res = leerEnteroEntre(1, 4, "Seleccione la opcion que desee ejecutar: ");
    return res;
}
void MenuJuegos(){
    printf ("     _____  \n");
    printf ("    |     \ \n");
    printf ("     \$$$$$ __    __   ______    ______    ______    _______ \n");
    printf ("       | $$|  \  |  \ /      \  /      \  /      \  /       \ \n");
    printf ("  __   | $$| $$  | $$|  $$$$$$\|  $$$$$$\|  $$$$$$\|  $$$$$$$ \n");
    printf (" |  \  | $$| $$  | $$| $$    $$| $$  | $$| $$  | $$ \$$    \ \n");
    printf (" | $$__| $$| $$__/ $$| $$$$$$$$| $$__| $$| $$__/ $$ _\$$$$$$\ \n");
    printf ("  \$$    $$ \$$    $$ \$$     \ \$$    $$ \$$    $$|       $$ \n");
    printf ("   \$$$$$$   \$$$$$$   \$$$$$$$ _\$$$$$$$  \$$$$$$  \$$$$$$$ \n");
    printf ("                                |  \__| $$ \n");
    printf ("                                \$$    $$ \n");
    printf ("                                 \$$$$$$ \n");
    printf ("\n");
    printf ("\n");
	printf ( "                        1                         \n");
	printf ("                   -------------                  \n");
	printf ("                  |    TRUCO    |                  \n");
	printf ("                   -------------                  \n");
	printf("\n");
	Escribir "                        2                         " 
	printf ("                   -------------                  \n");
	printf ("                  |     LUDO    |                  \n");
	printf ("                   -------------                  \n");
	printf ("\n");
	printf ("                         3                         \n"); 
	printf ("                   --------------                  \n");
	printf ("                  |    BATALLA   |                 \n");
	printf ("                  |     NAVAL    |                 \n");
	printf ("                   --------------                  \n");
	printf("\n");
	printf ("                         4                         \n"); 
	printf ("                   -------------                  \n");
	printf ("                  |   VOLVER    |                  \n");
	printf ("                   -------------                  \n");
	printf("\n");  
}
int JuegaTruco()
{
    truco();
}
int JuegaLudo()
{
    ludo();
}
int JuegaBatallaNaval()
{
    batalla_Naval();
}
