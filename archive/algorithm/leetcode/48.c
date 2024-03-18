#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/**
 * https://leetcode-cn.com/problems/rotate-image/description/
 * 旋转图像
 */

//123
//456
//789

//963
//852
//741

//741
//852
//963

//1,0
//2,1

//0,0 2,2
//2,0 2,0
//1,0 2,1
//0,1 1,2

void rotate(int matrix[3][3], int matrixRowSize, int *matrixColSizes)
{
    //先换对角
    int count = matrixRowSize;
    for (int i = 0; i < count; i++) //对角线交换元素
    {
        for (int n = 0; n < count - i; n++)
        {
            if (i == count - 1 - n &&n == count - 1 - i)
            {
                continue;
            }
            matrix[i][n] ^= matrix[count - 1 - n][count - 1 - i];
            matrix[count - 1 - n][count - 1 - i] ^= matrix[i][n];
            matrix[i][n] ^= matrix[count - 1 - n][count - 1 - i];
        }
    }
    //以中间为轴换
    for (int i = 0; i < count / 2; i++)
    {
        for (int n = 0; n < count; n++)
        {
            matrix[i][n] ^= matrix[count-1 - i][n];
            matrix[count-1 - i][n] ^= matrix[i][n];
            matrix[i][n] ^= matrix[count-1 - i][n];
        }
    }
}
/*
void rotate(int** matrix, int matrixRowSize, int *matrixColSizes) {
    int i = 0, j = 0, key;
    int n =matrixRowSize;
    for(i = 0; i < n / 2; i++){
        for(j = i; j < n-1-i; j++){
            key = matrix[i][j];
            matrix[i][j] = matrix[n - 1 - j][i];
            matrix[n - 1 - j][i] = matrix[n - 1 - i][n - 1 - j];
            matrix[n - 1 - i][n - 1 - j] = matrix[j][n - 1 - i];
            matrix[j][n - 1 - i] = key;
        }
    }

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
    int a[3][3] = {{1, 2, 3},
                   {4, 5, 6},
                   {7, 8, 9}};
    int *b = malloc(4);
    rotate(a, 3, b);
    printf("%d\n", 6 ^ 6);
    getchar();
    return 0;
}