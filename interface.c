#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "data.c"

// Set console display color
void SetColor(unsigned short ForeColor, unsigned short BackColor)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, (ForeColor % 16) | (BackColor % 16 * 16));
}

// Set console cursor position
void SetPos(int x, int y)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(console, (COORD){y, x});
}

// Page - Main menu
char menu()
{
    char choice;

    system("cls");

    SetColor(2, 0);
    printf("Minesweeper (C language implementation)\n\n");
    SetColor(7, 0);

    printf("1. Start new game\n");
    printf("2. Show heroes\n");
    printf("3. Quit\n\n");

    while (1)
    {
        fflush(stdin);
        printf("Input your choice: ");
        choice = getche();
        if (choice < '0' || choice > '9')
            printf("\nNot a number, try again:\n");
        else if (choice < '1' || choice > '3')
            printf("\nNot in list above, try again:\n");
        else
            break;
    }

    return choice;
}

// Page - Get mines number
int getnum()
{
    int num;

    system("cls");

    while (1)
    {
        printf("How many mines do you want to set? (1-195): ");
        fflush(stdin);
        if (scanf("%d", &num) != 1)
            printf("\nNot a number, try again:\n");
        else if (num < 1)
            printf("\nMust be greater than 0, try again:\n");
        else if (num > 195)
            printf("\nToo many, try again:\n");
        else
            break;
    }

    return num;
}

// Partial - Print main part of the game
void print()
{
    system("cls");
    for (int x = 0; x < UI_LIN; x++)
    {
        for (int y = 0; y < UI_COL; y++)
        {
            // Show mines
            if (show[x][y] == '@')
            {
                SetColor(4, 0);
                printf("%c", show[x][y]);
                SetColor(7, 0);
                continue;
            }
            // Show explored aera (numbers excluding line number)
            else if ('0' <= show[x][y] && show[x][y] <= '8' && y > 2)
            {
                SetColor(2, 0);
                printf("%c", show[x][y]);
                SetColor(7, 0);
                continue;
            }
            // Show unexplored aera
            printf("%c", show[x][y]);
        }
        printf("\n");
    }
}

// Page - Game
void game()
{
    int x = 0, y = 0, first = 1, boom = 0, num = getnum();
    time_t start, end;
    char ch;

    // Initialize the game
    setmines(num);
    memcpy(show, ui, sizeof(show));

    // Start timer
    start = time(NULL);

    // Playing
    while (1)
    {
        // Print main part
        print();
        SetPos(X, Y);

        // Get user input
        ch = getch();
        if ('a' <= ch && ch <= 'z')
            ch -= 32;

        // Process user input
        switch (ch)
        {
        case 'W':
            if (x > 0)
                x--;
            break;
        case 'A':
            if (y > 0)
                y--;
            break;
        case 'S':
            if (x < LIN - 1)
                x++;
            break;
        case 'D':
            if (y < COL - 1)
                y++;
            break;
        case 'Q':
            if (first)
            {
                first = 0;
                firstclick(x, y);
            }
            else
                boom = dig(x, y);
            break;
        case 'E':
            flag(x, y);
            break;
            // FOR DEBUG ONLY - START
            // case 'X':
            //     system("cls");
            //     for (int x = 0; x < UI_LIN; x++)
            //     {
            //         for (int y = 0; y < UI_COL; y++)
            //         {
            //             printf("%c", show[x][y]);
            //         }
            //         printf("\n");
            //     }
            //     printf("\n");
            //     for (int x = 0; x < LIN; x++)
            //     {
            //         for (int y = 0; y < COL; y++)
            //         {
            //             printf("%d ", mines[x][y]);
            //         }
            //         printf("\n");
            //     }
            //     printf("\n");
            //     for (int x = 0; x < LIN; x++)
            //     {
            //         for (int y = 0; y < COL; y++)
            //         {
            //             printf("%d ", stats[x][y]);
            //         }
            //         printf("\n");
            //     }
            //     getch();
            // FOR DEBUG ONLY - END
        }

        if (boom)
            break;
        else if (check(num))
            break;
    }

    // Stop timer and record
    end = time(NULL);
    if (!boom)
    {
        print();
        SetPos(UI_LIN + 1, 0);
        printf("You Win ! ! ! (●'◡'●)\n");
        printf("You finished in %ds.\n\n", (int)difftime(end, start));
        printf("Press any key to continue. . .\n");
        getch();
        savehero((int)difftime(end, start));
    }
    else
    {
        lost();
        print();
        SetPos(UI_LIN + 1, 0);
        printf("You Lost . . . (┬┬﹏┬┬)\n");
        printf("You finished in %ds.\n\n", (int)difftime(end, start));
        printf("Press any key to continue. . .\n");
        getch();
    }
}

// Page - Heroes
void heroes()
{
    FILE *fp = fopen("heroes.dat", "a+");
    if (fp == NULL)
    {
        printf("\nOpen file failed!\n");
        getch();
        exit(1);
    }
    rewind(fp);

    int arr[10] = {0};
    for (int i = 0; fscanf(fp, "%d\n", &arr[i]) == 1;)
        i++;

    fclose(fp);

    system("cls");

    SetColor(2, 0);
    printf("Minesweeper (C language implementation)\n\n");
    SetColor(7, 0);

    SetColor(3, 0);
    printf("Top 10\n\n");
    SetColor(7, 0);

    for (int i = 0; i < 10; i++)
        printf("%02d. %ds\n", i + 1, arr[i]);

    printf("\nPress any key to continue. . .");
    getch();
}