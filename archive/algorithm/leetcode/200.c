#include <stdio.h>
#include <stdlib.h>
/**
 * 岛屿的个数
 * https://lingkou.com/problems/number-of-islands/description/ 
 */

/**
 * 标记岛屿
 */
void mark(char **grid, int h, int l, int row, int col)
{
    if (h < 0 || h >= row || l < 0 || l >= col || grid[h][l] != '1')
    {
        return;
    }
    grid[h][l] = '2';
    //标记旁边的岛 上,下,左,右
    mark(grid, h + 1, l, row, col);
    mark(grid, h - 1, l, row, col);
    mark(grid, h, l + 1, row, col);
    mark(grid, h, l - 1, row, col);
}

int numIslands(char **grid, int gridRowSize, int gridColSize)
{
    int num = 0;
    for (int i = 0; i < gridRowSize; i++)
    {
        for (int n = 0; n < gridColSize; n++)
        {
            if (grid[i][n] == '1')
            {
                num++;
                mark(grid, i, n, gridRowSize, gridColSize);
            }
        }
    }
    return num;
}

/*
void mark(char** grid, int gridRowSize, int gridColSize, int i, int j) {
    if(grid[i][j] == '0')
        return;
    grid[i][j] = '0';
    if(i > 0)
        mark(grid, gridRowSize, gridColSize, i - 1, j);
    if(j > 0)
        mark(grid, gridRowSize, gridColSize, i, j - 1);
    if(i < gridRowSize - 1)
        mark(grid, gridRowSize, gridColSize, i + 1, j);
    if(j < gridColSize - 1)
        mark(grid, gridRowSize, gridColSize, i, j + 1);
}
int numIslands(char** grid, int gridRowSize, int gridColSize) {
    int i = 0, j = 0;
    int result = 0; 
    for(i = 0; i < gridRowSize; i++) {
        for(j = 0; j < gridColSize; j++) {
            if(grid[i][j] == '1') {
                result++;
                mark(grid, gridRowSize, gridColSize, i, j);   
            }  
        }
    }
    return result;  
}
*/

int main()
{
    char a[4][5] = {{'1', '1', '1', '1', '0'}, {'1', '1', '0', '1', '0'}, {'1', '1', '0', '0', '0'}, {'0', '0', '0', '0', '0'}};
    printf("%p\n", a);
    numIslands(a, 4, 5);
    getchar();
    return 0;
}