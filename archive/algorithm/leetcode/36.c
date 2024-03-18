#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * https://leetcode-cn.com/problems/valid-sudoku/description/
 * 有效的数独
 */

int isValidSudoku(char **board, int boardRowSize, int boardColSize)
{
    for (int i = 0; i < 9; i++)
    {
        int hash[9] = {0};
        for (int n = 0; n < 9; n++)
        {
            if (board[i][n] != '.')
            {
                hash[board[i][n] - 49]++;
                if (hash[board[i][n] - 49] > 1)
                {
                    return 0;
                }
            }
        }
    }
    for (int i = 0; i < 9; i++)
    {
        int hash[9] = {0};
        for (int n = 0; n < 9; n++)
        {
            if (board[n][i] != '.')
            {
                hash[board[n][i] - 49]++;
                if (hash[board[n][i] - 49] > 1)
                {
                    return 0;
                }
            }
        }
    }
    //对小块进行校验
    for (int p1 = 0; p1 < 3; p1++)
    {
        for (int p = 0; p < 3; p++)
        {
            int hash[9] = {0};
            for (int i = 0; i < 3; i++)
            {
                for (int n = 0; n < 3; n++)
                {
                    int x = p1 * 3 + i, y = p * 3 + n;
                    if (board[x][y] != '.')
                    {
                        hash[board[x][y] - 49]++;
                        if (hash[board[x][y] - 49] > 1)
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}
/*
bool isValidSudoku(char** board, int boardRowSize, int boardColSize) {
    int i, j, tmp, l;
    int flag[27][9];
    for (i=0; i<27; i++) {
        for (j=0; j<9; j++) {
            flag[i][j] = 0;
        }
    }
    for (i=0; i<boardRowSize; i++ ) {
        for (j=0; j<boardColSize; j++) {
            if (board[i][j] != '.') {
                tmp = board[i][j] - 48 -1;
                if ( flag[i][tmp] != 0 ) {
                    return false;
                }
                flag[i][tmp] = 1;
                if ( flag[j+9][tmp] != 0 ) {
                    return false;
                }
                flag[j+9][tmp] = 1;
                l = (i/3)*3+j/3+18;
                if ( flag[l][tmp] != 0 ) {
                    return false;
                }
                flag[l][tmp] = 1;
            }
        }
    }
    return true;
}
*/

void print_r(int *a, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d\t", a[i]);
    }
    printf("\n");
}

int main()
{
    printf("%d\n", 9 / 9);
    char a[9][9] = {
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'}, {'6', '.', '.', '1', '9', '5', '.', '.', '.'}, {'.', '9', '8', '.', '.', '.', '.', '6', '.'}, {'8', '.', '.', '.', '6', '.', '.', '.', '3'}, {'4', '.', '.', '8', '.', '3', '.', '.', '1'}, {'7', '.', '.', '.', '2', '.', '.', '.', '6'}, {'.', '6', '.', '.', '.', '.', '2', '8', '.'}, {'.', '.', '.', '4', '1', '9', '.', '.', '5'}, {'.', '.', '.', '.', '8', '.', '.', '7', '9'}};
    isValidSudoku(a, 9, 9);
    getchar();
    return 0;
}