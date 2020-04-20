#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

char immediate = 1;
char puzzle[9][9] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}};

void draw()
{
    // clear the screen
    printf("\033[H\033[J");

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%d ", puzzle[i][j]);
            if ((j + 1) % 3 == 0)
                printf("  ");
        }
        printf("\n");
        if ((i + 1) % 3 == 0)
            printf("\n");
    }
}

char find_free(int *x, int *y)
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (puzzle[i][j] == 0)
            {
                *x = i;
                *y = j;
                return 1;
            }
    return 0;
}

char verify(int n, int x, int y)
{
    for (int i = 0; i < 9; i++)
        if (puzzle[x][i] == n || puzzle[i][y] == n)
            return 0;

    int x_block = (x / 3) * 3;
    int y_block = (y / 3) * 3;
    for (int i = x_block; i < (x_block + 3); i++)
        for (int j = y_block; j < (y_block + 3); j++)
            if (puzzle[i][j] == n)
                return 0;

    return 1;
}

void usleep(__int64 usec)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    // Convert to 100 nanosecond interval,
    // negative value indicates relative time
    ft.QuadPart = -(10 * usec);

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

void draw_one(char n, int x, int y)
{
    printf("\033[%d;%dH", 15, 15);
    printf("%d in %d,%d", n, x, y);
    int i, j;
    i = x + (x / 3) + 1;
    j = (y * 2) + 1 + (((((y + 1) / 3 - 1) + (y + 1) % 3)) * 2);
    printf("\033[%d;%dH", (i), (j));
    printf("%d", n);
    usleep(50000);
}

int solve()
{
    int x, y;
    if (find_free(&x, &y) == 0)
        return 1;

    for (int i = 1; i <= 9; i++)
        if (verify(i, x, y))
        {
            puzzle[x][y] = i;
            if (!immediate)
                draw_one(i, x, y);
            if (solve())
                return 1;
            puzzle[x][y] = 0;
        }

    return 0;
}

int main(int argc, char *argv[])
{
    printf("argc %d\n", argc);
    if (argc > 1)
        if (strcmp(argv[1], "--slow") == 0)
            immediate = 0;
    else
        immediate = 1;

    draw();
    solve();
    draw();

    return 0;
}
