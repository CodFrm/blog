#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* nums;
    int* sum;
} NumArray;

NumArray* numArrayCreate(int* nums, int numsSize) {
    NumArray* obj = malloc(sizeof(NumArray));
    obj->nums = nums;
    obj->sum = malloc(sizeof(int) * numsSize);
    obj->sum[0] = nums[0];
    for (int i = 1; i < numsSize; i++) {
        obj->sum[i] = obj->sum[i - 1] + nums[i];
    }
    return obj;
}

int numArraySumRange(NumArray* obj, int i, int j) {
    return (obj->sum[j] - obj->sum[i])+obj->nums[i];
}

void numArrayFree(NumArray* obj) { free(obj); }
/*

typedef struct {
    int *nums;
    int numsSize;

} NumArray;

NumArray *numArrayCreate(int *nums, int numsSize) {


    int *a = malloc(numsSize * sizeof(int) + 1);
    a[0] = 0;
    for (int k = 0; k < numsSize; k++) {
        a[k + 1] = a[k] + nums[k];
    }

    NumArray *arr = malloc(sizeof(NumArray));
    arr->nums = a;
    arr->numsSize = numsSize;
    return arr;
}

int numArraySumRange(NumArray *obj, int i, int j) {
    return obj->nums[j + 1] - obj->nums[i];
}

void numArrayFree(NumArray *obj) {
    free(obj->nums);
    free(obj);
}
*/

/**
 * Your NumArray struct will be instantiated and called as such:
 * struct NumArray* obj = numArrayCreate(nums, numsSize);
 * int param_1 = numArraySumRange(obj, i, j);
 * numArrayFree(obj);
 */