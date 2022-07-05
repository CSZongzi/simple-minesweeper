#include "interface.c"

int main()
{
    // Welcome message
    SetColor(2, 0);
    printf("Minesweeper (C language implementation)\n\n");
    printf("Usage: W-up S-down A-left D-right Q-dig E-flag/unflag\n\n");
    SetColor(7, 0);
    printf("Category A question in Engineering Practice 1\n");
    printf("Solved by CSZ.\n");
    getch();

    while (1)
    {
        switch (menu())
        {
        case '1':
            game();
            break;
        case '2':
            heroes();
            break;
        case '3':
            return 0;
        default:
            return 1;
        }
    }

    return 0;
}