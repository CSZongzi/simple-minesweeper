#include <stdlib.h>
#include <string.h>
#include <time.h>

#define UI_LIN 16
#define UI_COL 30
#define LIN 14
#define COL 14

// Define macros to unify the coordinates of the user interface and the minefield
#define X (x + 2)
#define Y (2 * y + 3)

// Definition of User Interface
const char ui[16][30] = {
    {"---------Mine sweeper---------"},
    {"------------------------------"},
    {"01|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"02|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"03|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"04|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"05|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"06|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"07|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"08|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"09|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"10|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"11|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"12|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"13|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
    {"14|? ? ? ? ? ? ? ? ? ? ? ? ? ?"},
};

// Actual User Interface (Copy of the definition)
char show[16][30];

// Minefield map (01 binarization, marking mines)
short mines[LIN][COL] = {0};

// Mines statistics graph (total number of surrounding mines)
short stats[LIN][COL] = {0};

// Generally, the compiler will initialize the array to 0, as required by the language
// specification. But the following method is more portable.
// short stats[14][14] = {
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
// };

// Determine whether the point has mine
int ismine(int x, int y)
{
    if (x < 0 || y < 0 || x > LIN - 1 || y > COL - 1)
        return 0;
    else if (mines[x][y] == 1)
        return 1;
    else
        return 0;
}

// Count the number of mines in each adjacent grid
void countmines()
{
    int cnt;
    for (int x = 0; x < LIN; x++)
        for (int y = 0; y < COL; y++)
        {
            cnt = 0;

            cnt += ismine(x - 1, y - 1);
            cnt += ismine(x - 1, y);
            cnt += ismine(x - 1, y + 1);
            cnt += ismine(x, y + 1);
            cnt += ismine(x, y - 1);
            cnt += ismine(x + 1, y + 1);
            cnt += ismine(x + 1, y);
            cnt += ismine(x + 1, y - 1);

            stats[x][y] = cnt;
        }
}

// Random minefield generator
void setmines(int num)
{
    int x, y, cnt = 0;

    // Clear old minefield
    memset(mines, 0, sizeof(mines));
    memset(stats, 0, sizeof(mines));

    // Generate new minefield
    srand((unsigned long)time(NULL));
    while (cnt < num)
    {
        x = rand() % LIN;
        y = rand() % COL;

        if (mines[x][y] != 1)
        {
            mines[x][y] = 1;
            cnt++;
        }
    }
    countmines();
}

// Determine whether the point near mine
int nearmine(int x, int y)
{
    if (x < 0 || y < 0 || x > LIN - 1 || y > COL - 1)
        return 0;
    else if (stats[x][y] != 0)
        return 1;
    else
        return 0;
}

// Expand the "Zero" Area
void expand(int x, int y)
{
    // Abort when reach outside of the table
    if (x < 0 || x > LIN - 1 || y < 0 || y > COL - 1)
        return;

    // Abort when reaching an showed point
    if (show[X][Y] != '?')
        return;

    // Show the "Zero" Area and its boundaries
    show[X][Y] = stats[x][y] + '0';

    // Recursive solution
    if (!nearmine(x, y))
    {
        expand(x - 1, y - 1);
        expand(x - 1, y);
        expand(x - 1, y + 1);
        expand(x, y + 1);
        expand(x, y - 1);
        expand(x + 1, y + 1);
        expand(x + 1, y);
        expand(x + 1, y - 1);
    }
}

// Dig a point
int dig(int x, int y)
{
    if (mines[x][y] == 1)
        return 1;
    else
        expand(x, y);
    return 0;
}

// Flag a point
void flag(int x, int y)
{
    if (show[X][Y] == '?')
        show[X][Y] = '!';
    else if (show[X][Y] == '!')
        show[X][Y] = '?';
}

// Check if you have won
int check(int num)
{
    int correct = 0, total = 0;
    for (int x = 0; x < LIN; x++)
        for (int y = 0; y < COL; y++)
            if (show[X][Y] == '!')
            {
                if (mines[x][y] == 1)
                    correct++;
                total++;
            }
    if (total == num && correct == num)
        return 1;
    else
        return 0;
}

// First click
void firstclick(int x, int y)
{
    int new_x, new_y;
    srand((unsigned long)time(NULL));

    if (mines[x][y] == 1)
    {
        // Find space
        while (1)
        {
            new_x = rand() % LIN;
            new_y = rand() % COL;

            if (mines[new_x][new_y] == 0)
            {
                mines[x][y] = 0;
                mines[new_x][new_y] = 1;
                break;
            }
        }

        // Re-count
        countmines();
    }

    expand(x, y);
}

// After boom (Game over)
void lost()
{
    for (int x = 0; x < LIN; x++)
        for (int y = 0; y < COL; y++)
            if (mines[x][y] == 1)
                show[X][Y] = '@';
}

// Save heroes information
void savehero(int time)
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

    for (int i = 0; i < 10; i++)
        if (time < arr[i] || arr[i] == 0)
        {
            for (int j = 9; j > i; j--)
                arr[j] = arr[j - 1];
            arr[i] = time;
            break;
        }

    freopen("heroes.dat", "w", fp);
    for (int i = 0; i < 10; i++)
        fprintf(fp, "%d\n", arr[i]);

    fclose(fp);
}