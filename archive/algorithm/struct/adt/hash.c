#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#define TABLE_SIZE 41
#define MAX_FIND 4

//利用平方探测法创建散列
typedef struct _hash *HASH_TABLE;

struct _hash
{
    char *key;
    int value;
};

HASH_TABLE hash_create()
{
    //创建一个大小为size的表
    //也就是申请size大小的内存空间
    HASH_TABLE ret = (HASH_TABLE)malloc(TABLE_SIZE * sizeof(struct _hash));
    memset(ret, 0, sizeof(struct _hash) * TABLE_SIZE);
    return ret;
}

int hash(const char *key)
{
    //散列函数,P113
    unsigned int hash_value = 0;
    while (*key != '\0')
    {
        hash_value = (hash_value << 5) + *key++;
    }
    return hash_value % TABLE_SIZE;
}

int hash_insert(HASH_TABLE hash_table, const char *key, int value)
{
    //插入一个值到hash表中
    //先计算hash值
    int sub = hash(key);
    //判断是否有碰撞
    if ((int)hash_table[sub].key != 0)
    {
        //值不是空的,利用平方的方法,探测空的地方
        int tmp_sub = 0;
        for (int i = 0; i < MAX_FIND; i++)
        {
            tmp_sub = sub + i * i;
            if (tmp_sub >= TABLE_SIZE)
            {
                tmp_sub -= TABLE_SIZE;
            }
            if ((int)hash_table[tmp_sub].key == 0 || strcmp(hash_table[tmp_sub].key, key) == 0)
            {
                //找到了空的位置,赋值,跳出循环
                break;
            }
            //如果继续碰撞,继续往后计算
        }
        if (hash_table[tmp_sub].value != value)
        {
            //没有找到空位,返回-1,提示失败
            return -1;
        }
        sub = tmp_sub;
    }
    hash_table[sub].key = (char *)malloc(strlen(key)+1);
    memset(hash_table[sub].key,0,strlen(key)+1);
    memcpy(hash_table[sub].key, key, strlen(key));
    hash_table[sub].value = value;
    return sub;
}

int hash_find(HASH_TABLE hash_table, const char *key, int value)
{
}

#ifndef MAIN_FUNC
int main()
{
    HASH_TABLE hash_table = hash_create();
    hash_insert(hash_table, "one", 1);
    hash_insert(hash_table, "t", 2);
    hash_insert(hash_table, "e", 3);
    hash_insert(hash_table, "q", 4);
    hash_insert(hash_table, "while", 5);
    hash_insert(hash_table, "z", 6);
    hash_insert(hash_table, "f", 7);
    //遍历表
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if ((int)hash_table[i].key == 0)
        {
            continue;
        }
        printf("%s=>%d\n", hash_table[i].key, hash_table[i].value);
    }

    return 0;
}
#endif
