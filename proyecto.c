#include "utils.h"
#include "programas/BatallaNaval/batalla_naval.c"
#include "programas/Ludo/ludo.c"
#include "programas/Truco/TrucoFinal.c"
void menu();
int elegirJuego();
int JuegaTruco();
int JuegaLudo();
int JuegaBatallaNaval();
int main()
{
    int juego;
    do
    {
        menu();
        juego = elegirJuego();
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
    
    } while (juego != 4);

return 0;
}
void menu()
{
    printf("=====MENU=====\n");
    printf("1 TRUCO\n");
    printf("2 BATALLA NAVAL\n");
    printf("3 LUDO\n");
    printf("4 SALIR\n");
}
int elegirJuego()
{
    int res;
    res = leerEnteroEntre(1, 4, "Seleccione el juego: ");
    return res;
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