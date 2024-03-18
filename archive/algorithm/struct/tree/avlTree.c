#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
/**
 * AVL树
 */
struct tree;
typedef struct tree *prt_tree;
#define bool int
#define false 0
#define true 1
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

struct tree
{
    int data;    //数据
    int height;  //树深度
    prt_tree lt; //左节点
    prt_tree rt; //右节点
};

/**
 * 创建一个节点
 */
prt_tree create_tree(int data)
{
    prt_tree tmp = malloc(sizeof(struct tree));
    memset(tmp, 0, sizeof(struct tree));
    tmp->data = data;
    return tmp;
}
int height(prt_tree tree)
{
    if (tree != NULL)
    {
        return tree->height;
    }
    return 0;
}
/**
 * 右单旋
 */
prt_tree rightSingleRotation(prt_tree root)
{
    //1->2->3   交换1 2,也就是1 2 旋转
    prt_tree tmp = root->rt; //root=1 tmp=2
    root->rt = tmp->lt;      //root->rt=tmp->lt    吧2的右节点接到1的左节点
    tmp->lt = root;          //1的左节点为2节点
    // tmp->lt=
    root->height = (height(root->lt) > height(root->rt) ? height(root->lt) : height(root->rt)) + 1;
    tmp->height = (height(tmp->lt) > height(tmp->rt) ? height(tmp->lt) : height(tmp->rt)) + 1;
    return tmp;
}
/**
 * 左单旋
 */
prt_tree leftSingleRotation(prt_tree root)
{
    //1->2->3   交换1 2,也就是1 2 旋转
    prt_tree tmp = root->lt; //root=1 tmp=2
    root->lt = tmp->rt;      //root->lt=tmp->rt    吧2的右节点接到1的左节点
    tmp->rt = root;          //1的左节点为2节点
    // tmp->rt=
    root->height = (height(root->rt) > height(root->lt) ? height(root->rt) : height(root->lt)) + 1;
    tmp->height = (height(tmp->rt) > height(tmp->lt) ? height(tmp->rt) : height(tmp->lt)) + 1;
    return tmp;
}
/**
 * 左双旋
 */
prt_tree leftDoubleRotation(prt_tree root)
{
    root->lt = rightSingleRotation(root->lt);
    return leftSingleRotation(root);
}
/**
 * 右双旋
 */
prt_tree rightDoubleRotation(prt_tree root)
{
    root->rt = leftSingleRotation(root->rt);
    return rightSingleRotation(root);
}

/**
 * 插入树
 */
prt_tree insert(int data, prt_tree root)
{
    if (root == NULL) //如果节点为空,则认为将插入到该点
    {
        root = create_tree(data);
    }
    else if (root->data > data) //    判断往哪个方向节点插入,数据比当前小,往左
    {
        root->lt = insert(data, root->lt);
        if (height(root->lt) - height(root->rt) == 2) //重新平衡
        {
            if (data < root->lt->data)
            {
                root = leftSingleRotation(root);
            }
            else
            {
                root = leftDoubleRotation(root);
            }
        }
    }
    else if (root->data < data) //数据比当前大,往右
    {
        root->rt = insert(data, root->rt);
        if (height(root->rt) - height(root->lt) == 2) //重新平衡
        {
            if (data > root->rt->data)
            {
                root = rightSingleRotation(root);
            }
            else
            {
                root = rightDoubleRotation(root);
            }
        }
    }
    root->height = max(height(root->lt), height(root->rt)) + 1;
    return root;
}
void print_tree(prt_tree root) //中序输出就是排序啦
{
    if (root == NULL)
    {
        return;
    }
    print_tree(root->lt);
    printf("%d\t", root->data);
    print_tree(root->rt);
}
int main()
{
    prt_tree root = insert(201, NULL);
    for (int i = 0; i < 20; i++)
    {
        root = insert(rand(), root);
    }
    print_tree(root);
    getchar();
    return 0;
}