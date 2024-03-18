#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define bool char
#define true 1
#define false 0
void setZeroes(int matrix[7][1], int matrixRowSize, int matrixColSize)
{
    bool h = false, l = false;
    //扫描首行有没有0
    for (int i = 0; i < matrixColSize; i++)
    {
        if (matrix[0][i] == 0)
        {
            h = true;
            break;
        }
    }
    //扫描首列有没有0
    for (int i = 0; i < matrixRowSize; i++)
    {
        if (matrix[i][0] == 0)
        {
            l = true;
            break;
        }
    }
    //扫描0
    for (int i = 1; i < matrixRowSize; i++)
    {
        for (int n = 1; n < matrixColSize; n++)
        {
            if (matrix[i][n] == 0)
            {
                matrix[0][n] = 0; //首行置0
                matrix[i][0] = 0; //首列置0
            }
        }
    }
    //开始填0,扫描首行
    for (int i = 0; i < matrixColSize; i++)
    {
        if (matrix[0][i] == 0)
        {
            for (int n = 1; i > 0 && n < matrixRowSize; n++)
            {
                matrix[n][i] = 0;
            }
        }
    }
    //开始填0,扫描首列
    for (int i = 0; i < matrixRowSize; i++)
    {
        if (matrix[i][0] == 0)
        {
            for (int n = 1; i > 0 && n < matrixColSize; n++)
            {
                matrix[i][n] = 0;
            }
        }
    }
    //对首行/列判断
    for (int i = 0; h && i < matrixColSize; i++)
    {
        matrix[0][i] = 0;
    }
    for (int i = 0; l && i < matrixRowSize; i++)
    {
        matrix[i][0] = 0;
    }
}
/*
void solution(int** matrix, int i, int j, int *row, int*col, int r, int c);//每个元素的操作
void setZeroes(int** matrix, int matrixRowSize, int matrixColSize) {
	int r = matrixRowSize, c = matrixColSize, i, j;
	int*row = (int*)malloc(sizeof(int)*r), *col = (int*)malloc(sizeof(int)*c);//标记某行、某列是否已经已经有过0，用于减枝所用空间O(m+n)，不减枝则所用额外空间为常数级
	for (i = 0; i < r; i++) {
		for (j = 0; j < c; j++) {
			solution(matrix, i, j, row, col, r, c);//
		}
	}
	for (i = 0; i < r; i++)
		for (j = 0; j < c; j++)
			matrix[i][j] = matrix[i][j] == -20 ? 0 : matrix[i][j];//更新标记值-20为0
}
void solution(int** matrix, int i, int j, int *row, int*col, int r, int c) {
	if (matrix[i][j] == 0) {//满足操作条件的元素
		if (col[j] != 20) {
			col[j] = 20;//列标记
			for (int q = 0; q < r; q++)
				matrix[q][j] =  matrix[q][j] == 0 ? matrix[q][j] : -20;//该列上0保持不变，其余改为标记值-20
		}
		if (row[i] != 20) {
			row[i] = 20;//行标记
			for (int q = 0; q<c; q++)
				matrix[i][q] = (matrix[i][q] == -20 || matrix[i][q] == 0) ? matrix[i][q] : -20;
		}
	}
}
*/
int main()
{
    int x[7][1] = {{5}, {2}, {0}, {3}, {6}, {7}, {2}};
    setZeroes(x, 7, 1);
    getchar();
    return 0;
}
