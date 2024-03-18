#include <stdio.h>

//01223042
//01230422
int removeElement(int *nums, int numsSize, int val)
{
    int ret = 0;
    for (int i = 0; i < numsSize - ret; i++)
    {
        if (nums[i] == val)
        {
            for (int n = i; n < numsSize - ret - 1; n++)
            {
                nums[n] = nums[n + 1];
            }
            ret++;
            i--;
        }
    }
    return numsSize - ret;
}
/*
int removeElement(int* nums, int numsSize, int val) {
    int pos = 0;
    for(int i=0; i<numsSize; i++) {
        if(nums[i] != val) {
            nums[pos++] = nums[i];
        }
    }
    return pos;
}
*/
int main()
{
    int a[] = {3, 2, 2, 3};

    removeElement(a, 4, 3);
    getchar();
    return 0;
}