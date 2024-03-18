#include <stdio.h>

/**
 * https://leetcode-cn.com/explore/interview/card/top-interview-questions-easy/1/array/22/
 * 买卖股票的最佳时机 II
 */
int main()
{
    int val[] = {2, 1, 2, 0, 1};

    int max = 0;
    for (int i = 1; i < sizeof(val) / 4; i++)
    {
        int d = val[i] - val[i - 1];
        if (d > 0)
        {
            max += d;
        }
    }
    return max;
}